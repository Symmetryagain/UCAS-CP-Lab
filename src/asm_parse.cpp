#include "asm_gen.h"
#include "ir.h"
#include "context.h"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Global data structures: populated by parse_ir(), read by asm_codegen
vector<GlobalVar>   gv_list;
map<string, size_t> gv_map;
vector<GlobalArr>   ga_list;
map<string, size_t> ga_map;
vector<Funct>        f_list;
map<string, size_t> f_map;

string normalize_name(string prev_name)
{
	string new_name;
	for(char ch : prev_name)
	{
		if(ch != '%')
		{
			new_name.push_back(ch);
		}
	}
	return new_name;
}

vector<string> split_string(string const & str)
{
	istringstream  iss(str);
	vector<string> tokens;
	string         token;

	while(iss >> token)
	{
		tokens.push_back(token);
	}

	return tokens;
}

GType get_gtype_from_char(char ch)
{
	switch(ch)
	{
	case 'i':
		return G_INT;
	case 'b':
		return G_BOOL;
	case 'f':
		return G_FLOAT;
	case 'd':
		return G_DOUBLE;
	default:
		assert(false);
	}
}

FType get_ftype_from_char(char ch)
{
	switch(ch)
	{
	case 'i':
		return F_INT;
	case 'b':
		return F_BOOL;
	case 'f':
		return F_FLOAT;
	case 'd':
		return F_DOUBLE;
	case 'v':
		return F_VOID;
	default:
		assert(false);
	}
}

PType get_ptype_from_string(string str)
{
	switch(str[0])
	{
	case '$':
		return P_REG;
	case '%':
		switch(str[1])
		{
		case 'i':
			return P_INT;
		case 'b':
			return P_BOOL;
		case 'f':
			return P_FLOAT;
		case 'd':
			return P_DOUBLE;
		case 'a':
			switch(str[2])
			{
			case 'i':
				return P_PTR_INT;
			case 'b':
				return P_PTR_BOOL;
			case 'f':
				return P_PTR_FLOAT;
			case 'd':
				return P_PTR_DOUBLE;
			default:
				assert(false);
			}
		default:
			assert(false);
		}
	default:
		assert(false);
	}
}

uint64_t str_to_bin(GType gtype, string str)
{
	uint64_t val = 0;
	switch(gtype)
	{
	case G_INT:
		{
			int ival = stoi(str, 0, 0);
			memcpy(&val, &ival, sizeof(int));
		}
		break;
	case G_BOOL:
		{
			bool bval = (str == "true");
			memcpy(&val, &bval, sizeof(bool));
		}
		break;
	case G_FLOAT:
		{
			float fval = stof(str);
			memcpy(&val, &fval, sizeof(float));
		}
		break;
	case G_DOUBLE:
		{
			double dval = stod(str);
			memcpy(&val, &dval, sizeof(double));
		}
		break;
	default:
		assert(false);
	}
	return val;
}

void add_global_var(GlobalVar gv)
{
	assert(gv_map.count(gv.name) == 0);
	gv_map[gv.name] = gv_list.size();
	gv_list.push_back(gv);
}

void add_global_arr(GlobalArr ga)
{
	assert(ga_map.count(ga.name) == 0);
	ga_map[ga.name] = ga_list.size();
	ga_list.push_back(ga);
}

void add_func(Funct f)
{
	assert(f_map.count(f.name) == 0);
	f_map[f.name] = f_list.size();
	f_list.push_back(f);
}

void func_add_param(Funct& f, string p_name)
{
	assert(f.p_map.count(p_name) == 0);
	PType  ptype = get_ptype_from_string(p_name);
	size_t reg_pos;
	if(ptype == P_FLOAT || ptype == P_DOUBLE)
	{
		assert(f.fpr_a < 8);
		reg_pos = 8 + f.fpr_a++;
	}
	else
	{
		assert(f.gpr_a < 8);
		reg_pos = f.gpr_a++;
	}
	Param p         = {.name = p_name, .ptype = ptype, .reg_pos = reg_pos};
	f.p_map[p_name] = f.p_list.size();
	f.p_list.push_back(p);
}

void func_add_local_var(Funct& f, string lv_name)
{
	assert(f.lv_map.count(lv_name) == 0);
	PType  ptype  = get_ptype_from_string(lv_name);
	size_t size   = TypeSize[ptype];
	size_t sp_pos = f.sp_size;
	while(sp_pos % size != 0)
	{
		sp_pos++;
	}
	LocalVar lv       = {.name = lv_name, .ptype = ptype, .sp_pos = sp_pos};
	f.lv_map[lv_name] = f.lv_list.size();
	f.lv_list.push_back(lv);
	f.sp_size = sp_pos + size;
}

void func_add_local_arr(Funct& f, string la_name, size_t length)
{
	assert(f.la_map.count(la_name) == 0);
	assert(la_name[0] == '%');
	assert(la_name[1] == 'a');
	GType  gtype  = get_gtype_from_char(la_name[2]);
	size_t sp_pos = f.sp_size;
	while(sp_pos % TypeSize[gtype] != 0)
	{
		sp_pos++;
	}
	LocalArr la = {
	    .name = la_name, .gtype = gtype, .length = length, .sp_pos = sp_pos};
	f.la_map[la_name] = f.la_list.size();
	f.la_list.push_back(la);
	f.sp_size = sp_pos + TypeSize[gtype] * length;
}

void init_extern_func()
{
	Funct print_int = {.name      = "%print_int",
	                  .ftype     = F_VOID,
	                  .p_list    = vector<Param>{{.name    = "%iiii",
	                                              .ptype   = P_INT,
	                                              .reg_pos = 0}}, // a0
	                  .p_map     = map<string, size_t>{{"%iiii", 0}},
	                  .is_extern = true,
	                  .is_used   = (optimize_level == 0)};
	add_func(print_int);
	Funct print_bool = {.name      = "%print_bool",
	                   .ftype     = F_VOID,
	                   .p_list    = vector<Param>{{.name    = "%bbbb",
	                                               .ptype   = P_BOOL,
	                                               .reg_pos = 0}}, // a0
	                   .p_map     = map<string, size_t>{{"%bbbb", 0}},
	                   .is_extern = true,
	                   .is_used   = (optimize_level == 0)};
	add_func(print_bool);
	Funct print_float = {.name      = "%print_float",
	                    .ftype     = F_VOID,
	                    .p_list    = vector<Param>{{.name    = "%ffff",
	                                                .ptype   = P_FLOAT,
	                                                .reg_pos = 8}}, // fa0
	                    .p_map     = map<string, size_t>{{"%ffff", 0}},
	                    .is_extern = true,
	                    .is_used   = (optimize_level == 0)};
	add_func(print_float);
	Funct print_double = {.name      = "%print_double",
	                     .ftype     = F_VOID,
	                     .p_list    = vector<Param>{{.name    = "%dddd",
	                                                 .ptype   = P_DOUBLE,
	                                                 .reg_pos = 8}}, // fa0
	                     .p_map     = map<string, size_t>{{"%dddd", 0}},
	                     .is_extern = true,
	                     .is_used   = (optimize_level == 0)};
	add_func(print_double);
	Funct get_int = {.name      = "%get_int",
	                .ftype     = F_INT,
	                .p_list    = vector<Param>{},
	                .p_map     = map<string, size_t>{},
	                .is_extern = true,
	                .is_used   = (optimize_level == 0)};
	add_func(get_int);
	Funct get_float = {.name      = "%get_float",
	                  .ftype     = F_FLOAT,
	                  .p_list    = vector<Param>{},
	                  .p_map     = map<string, size_t>{},
	                  .is_extern = true,
	                  .is_used   = (optimize_level == 0)};
	add_func(get_float);
	Funct get_double = {.name      = "%get_double",
	                   .ftype     = F_DOUBLE,
	                   .p_list    = vector<Param>{},
	                   .p_map     = map<string, size_t>{},
	                   .is_extern = true,
	                   .is_used   = (optimize_level == 0)};
	add_func(get_double);
}

extern map<string, set<string>> o1_func_rely;

void parse_ir()
{
	for(int ir_line_idx = 0; ir_line_idx < ir_lines.size(); ir_line_idx++)
	{
		vector<string> tokens = split_string(ir_lines[ir_line_idx]);
		if(tokens.size() == 0)
		{
			continue;
		}
		if(tokens[0] == "!global")
		{
			if(tokens[1] == "@var")
			{
				GlobalVar gv;
				gv.name = tokens[2];
				assert(tokens[2][0] == '%');
				gv.gtype     = get_gtype_from_char(tokens[2][1]);
				gv.is_const  = 0;
				gv.is_unused = 0;
				gv.is_bss    = (optimize_level > 0);
				gv.value     = 0;
				add_global_var(gv);
				continue;
			}
			if(tokens[1] == "@array")
			{
				GlobalArr ga;
				ga.name = tokens[2];
				assert(tokens[2][0] == '%');
				assert(tokens[2][1] == 'a');
				ga.gtype     = get_gtype_from_char(tokens[2][2]);
				ga.length    = stoi(tokens[3], 0, 0);
				ga.is_const  = 0;
				ga.is_unused = 0;
				ga.is_bss    = (optimize_level > 0);
				ga.values    = vector<uint64_t>(ga.length, 0);
				add_global_arr(ga);
				continue;
			}
			assert(tokens[1] == "assign");
			string tmp_name = tokens[2];
			if(tmp_name.back() == ']')
			{
				assert(tmp_name[0] == '%');
				assert(tmp_name[1] == 'a');
				size_t name_idx;
				for(name_idx = 0;
				    name_idx < tmp_name.size() && tmp_name[name_idx] != '[';
				    name_idx++)
					;
				assert(name_idx < tmp_name.size());
				string   arr_name = tmp_name.substr(0, name_idx);
				size_t   arr_idx  = stoull(tmp_name.substr(
				    name_idx + 1, tmp_name.size() - name_idx - 2));
				uint64_t val =
				    str_to_bin(ga_list[ga_map[arr_name]].gtype, tokens[3]);
				assert(get_gtype_from_char(tmp_name[2]) ==
				       ga_list[ga_map[arr_name]].gtype);
				ga_list[ga_map[arr_name]].values[arr_idx] = val;
				if(val != 0)
				{
					ga_list[ga_map[arr_name]].is_bss = false;
				}
				continue;
			}
			assert(tmp_name[0] == '%');
			assert(get_gtype_from_char(tmp_name[1]) ==
			       gv_list[gv_map[tmp_name]].gtype);
			uint64_t val =
			    str_to_bin(gv_list[gv_map[tmp_name]].gtype, tokens[3]);
			gv_list[gv_map[tmp_name]].value = val;
			if(val != 0)
			{
				gv_list[gv_map[tmp_name]].is_bss = false;
			}
			continue;
		}
		assert(tokens[0] == "@func");
		Funct f;
		f.name = tokens[1];
		assert(f.name[0] == '%');
		if(f.name == "%main")
		{
			f.ftype = F_INT;
		}
		else
		{
			f.ftype = get_ftype_from_char(f.name[1]);
		}

		f.gpr_a     = 0;
		f.fpr_a     = 0;
		f.sp_size   = 0;
		f.is_used   = (optimize_level == 0);
		f.is_extern = false;
		func_add_local_var(f, "$ra");

		for(size_t func_decl_idx = 3; func_decl_idx < tokens.size() - 1;
		    func_decl_idx++)
		{
			if(tokens[func_decl_idx] == ",")
			{
				continue;
			}
			func_add_param(f, tokens[func_decl_idx]);
			func_add_local_var(f, tokens[func_decl_idx]);
		}
		int ir_line_jdx;
		for(ir_line_jdx = ir_line_idx; ir_line_jdx < ir_lines.size();
		    ir_line_jdx++)
		{
			if(ir_lines[ir_line_jdx].size() == 0)
			{
				continue;
			}
			f.ir_lines.push_back(ir_lines[ir_line_jdx]);
			vector<string> tokens_jdx = split_string(ir_lines[ir_line_jdx]);
			assert(tokens_jdx.size() > 0);
			if(tokens_jdx[0] == "@endfunc")
			{
				break;
			}
			if(tokens_jdx[0] == "@var")
			{
				func_add_local_var(f, tokens_jdx[1]);
			}
			if(tokens_jdx[0] == "@array")
			{
				func_add_local_arr(f, tokens_jdx[1], stoi(tokens_jdx[2], 0, 0));
			}

			if(tokens_jdx[0] == "call")
			{
				o1_func_rely[f.name].insert(tokens_jdx[2]);
			}
		}
		while(f.sp_size % 16 != 0)
		{
			f.sp_size++;
		}
		add_func(f);
		ir_line_idx = ir_line_jdx;
	}
}

void show_ir()
{
	for(GlobalVar gv : gv_list)
	{
		cerr << "global var " << gv.name << " " << gv.gtype << " " << gv.value
		     << endl;
		cerr << "  is_const " << (gv.is_const ? "true" : "false") << endl;
		cerr << "  is_unused " << (gv.is_unused ? "true" : "false") << endl;
		cerr << "  is_bss " << (gv.is_bss ? "true" : "false") << endl;
	}
	for(GlobalArr ga : ga_list)
	{
		cerr << "global arr " << ga.name << " " << ga.gtype << " " << ga.length
		     << endl;
		cerr << "  is_const " << (ga.is_const ? "true" : "false") << endl;
		cerr << "  is_unused " << (ga.is_unused ? "true" : "false") << endl;
		cerr << "  is_bss " << (ga.is_bss ? "true" : "false") << endl;
		cerr << "  values " << endl;
		for(uint64_t val : ga.values)
		{
			cerr << "    " << val << endl;
		}
	}
	for(Funct f : f_list)
	{
		cerr << "func " << f.name << endl;
		cerr << "  type " << f.ftype << endl;
		cerr << "  gpr_a " << f.gpr_a << endl;
		cerr << "  fpr_a " << f.fpr_a << endl;
		cerr << "  sp_size " << f.sp_size << endl;
		cerr << "  p_list " << endl;
		for(Param p : f.p_list)
		{
			cerr << "    " << p.name << " " << p.ptype << " " << p.reg_pos
			     << endl;
		}
		cerr << "  p_map " << endl;
		for(auto p : f.p_map)
		{
			cerr << "    " << p.first << " " << p.second << endl;
		}
		cerr << "  lv_list " << endl;
		for(LocalVar lv : f.lv_list)
		{
			cerr << "    " << lv.name << " " << lv.ptype << " " << lv.sp_pos
			     << endl;
		}
		cerr << "  lv_map " << endl;
		for(auto lv : f.lv_map)
		{
			cerr << "    " << lv.first << " " << lv.second << endl;
		}
		cerr << "  la_list " << endl;
		for(LocalArr la : f.la_list)
		{
			cerr << "    " << la.name << " " << la.gtype << " " << la.length
			     << " " << la.sp_pos << endl;
		}
		cerr << "  la_map " << endl;
		for(auto la : f.la_map)
		{
			cerr << "    " << la.first << " " << la.second << endl;
		}
		cerr << "  ir_lines " << endl;
		for(string ir_line : f.ir_lines)
		{
			cerr << "    " << ir_line << endl;
		}
		cerr << "  is_extern " << (f.is_extern ? "true" : "false") << endl;
		cerr << "  is_used " << (f.is_used ? "true" : "false") << endl;
		cerr << "  asm_lines " << endl;
		for(string asm_line : f.asm_lines)
		{
			cerr << "    " << asm_line << endl;
		}
	}
}
