#include "asmgen.h"

#include "main.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

vector<GlobalVar>   gv_list;
map<string, size_t> gv_map;
vector<GlobalArr>   ga_list;
map<string, size_t> ga_map;
vector<Func>        f_list;
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

string inst_load_i(size_t size)
{
	switch(size)
	{
	case 1:
		return "lb";
	case 2:
		return "lh";
	case 4:
		return "lw";
	case 8:
		return "ld";
	default:
		assert(false);
	}
}

string inst_store_i(size_t size)
{
	switch(size)
	{
	case 1:
		return "sb";
	case 2:
		return "sh";
	case 4:
		return "sw";
	case 8:
		return "sd";
	default:
		assert(false);
	}
}

string inst_load_f(size_t size)
{
	switch(size)
	{
	case 4:
		return "flw";
	case 8:
		return "fld";
	default:
		assert(false);
	}
}

string inst_store_f(size_t size)
{
	switch(size)
	{
	case 4:
		return "fsw";
	case 8:
		return "fsd";
	default:
		assert(false);
	}
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

void add_func(Func f)
{
	assert(f_map.count(f.name) == 0);
	f_map[f.name] = f_list.size();
	f_list.push_back(f);
}

void func_add_param(Func& f, string p_name)
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

void func_add_local_var(Func& f, string lv_name)
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

void func_add_local_arr(Func& f, string la_name, size_t length)
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

void gen0r(Func& f, string op)
{
	f.asm_lines.push_back(op);
}

void gen1r(Func& f, string op, string rd)
{
	f.asm_lines.push_back(op + " " + rd);
}

void gen2r(Func& f, string op, string rd, string rs)
{
	f.asm_lines.push_back(op + " " + rd + ", " + rs);
}

void gen3r(Func& f, string op, string rd, string rs, string rt)
{
	f.asm_lines.push_back(op + " " + rd + ", " + rs + ", " + rt);
}

void gen_sp_expand(Func& f)
{
	gen3r(f, "addi", "sp", "sp", "-" + to_string(f.sp_size));
}

void gen_sp_shrink(Func& f)
{
	gen3r(f, "addi", "sp", "sp", to_string(f.sp_size));
}

void func_call(Func& f, string func)
{
	gen1r(f, "call", normalize_name(func));
}

void ir2m(Func& f, string ireg, string memvar)
{
	assert(ireg == "ra" || ireg == "sp" || ireg[0] == 's' || ireg[0] == 'a');
	PType  ptype = get_ptype_from_string(memvar);
	size_t size  = TypeSize[ptype];
	if(f.lv_map.count(memvar) != 0)
	{
		size_t sp_pos = f.lv_list[f.lv_map[memvar]].sp_pos;
		gen2r(f, inst_store_i(size), ireg, to_string(sp_pos) + "(sp)");
	}
	else
	{
		assert(gv_map.count(memvar) != 0);
		gen2r(f, "la", "t0", normalize_name(memvar));
		gen2r(f, inst_store_i(size), ireg, "0(t0)");
	}
}

void fr2m(Func& f, string freg, string memvar)
{
	assert(freg[0] == 'f' && (freg[1] == 's' || freg[1] == 'a'));
	PType  ptype = get_ptype_from_string(memvar);
	size_t size  = TypeSize[ptype];
	assert(ptype == P_FLOAT || ptype == P_DOUBLE);
	if(f.lv_map.count(memvar) != 0)
	{
		size_t sp_pos = f.lv_list[f.lv_map[memvar]].sp_pos;
		gen2r(f, inst_store_f(size), freg, to_string(sp_pos) + "(sp)");
	}
	else
	{
		assert(gv_map.count(memvar) != 0);
		gen2r(f, "la", "t0", normalize_name(memvar));
		gen2r(f, inst_store_f(size), freg, "0(t0)");
	}
}

void m2ir(Func& f, PType ptype, string memvar, string ireg)
{
	assert(ireg == "ra" || ireg == "sp" || ireg[0] == 's' || ireg[0] == 'a');
	if(memvar[0] == '%' || memvar[0] == '$')
	{
		PType  ptype_mem = get_ptype_from_string(memvar);
		size_t size      = TypeSize[ptype];
		assert(ptype_mem == ptype);
		if(f.lv_map.count(memvar) != 0)
		{
			size_t sp_pos = f.lv_list[f.lv_map[memvar]].sp_pos;
			gen2r(f, inst_load_i(size), ireg, to_string(sp_pos) + "(sp)");
		}
		else if(f.la_map.count(memvar) != 0)
		{
			size_t sp_pos = f.la_list[f.la_map[memvar]].sp_pos;
			gen3r(f, "addi", ireg, "sp", to_string(sp_pos));
		}
		else if(gv_map.count(memvar) != 0)
		{
			gen2r(f, "la", "t0", normalize_name(memvar));
			gen2r(f, inst_load_i(size), ireg, "0(t0)");
		}
		else
		{
			assert(ga_map.count(memvar) != 0);
			gen2r(f, "la", ireg, normalize_name(memvar));
		}
	}
	else
	{
		switch(ptype)
		{
		case P_BOOL:
			{
				assert((memvar == "true") || (memvar == "false"));
				if(memvar == "true")
				{
					gen2r(f, "li", ireg, "1");
				}
				else
				{
					gen2r(f, "li", ireg, "0");
				}
			}
			break;
		case P_INT:
			{
				int ival = stoi(memvar, 0, 0);
				gen2r(f, "li", ireg, to_string(ival));
			}
			break;
		default:
			assert(false);
		}
	}
}

void m2fr(Func& f, PType ptype, string memvar, string freg)
{
	assert(freg[0] == 'f' && (freg[1] == 's' || freg[1] == 'a'));
	assert(ptype == P_FLOAT || ptype == P_DOUBLE);
	if(memvar[0] == '%')
	{
		PType  ptype_mem = get_ptype_from_string(memvar);
		size_t size      = TypeSize[ptype];
		assert(ptype_mem == ptype);
		if(f.lv_map.count(memvar) != 0)
		{
			size_t sp_pos = f.lv_list[f.lv_map[memvar]].sp_pos;
			gen2r(f, inst_load_f(size), freg, to_string(sp_pos) + "(sp)");
		}
		else
		{
			assert(gv_map.count(memvar) != 0);
			gen2r(f, "la", "t0", normalize_name(memvar));
			gen2r(f, inst_load_f(size), freg, "0(t0)");
		}
	}
	else
	{
		uint64_t val = 0;
		switch(ptype)
		{
		case P_FLOAT:
			val = str_to_bin(G_FLOAT, memvar);
			gen2r(f, "li", "t0", to_string(val));
			gen2r(f, "fmv.w.x", freg, "t0");
			break;
		case P_DOUBLE:
			val = str_to_bin(G_DOUBLE, memvar);
			gen2r(f, "li", "t0", to_string(val));
			gen2r(f, "fmv.d.x", freg, "t0");
			break;
		default:
			assert(false);
		}
	}
}

void gen_return(Func& f, string memvar)
{
	switch(f.ftype)
	{
	case F_VOID:
		assert(memvar == "");
		break;
	case F_INT:
		m2ir(f, P_INT, memvar, "a0");
		break;
	case F_BOOL:
		m2ir(f, P_BOOL, memvar, "a0");
		break;
	case F_FLOAT:
		m2fr(f, P_FLOAT, memvar, "fa0");
		break;
	case F_DOUBLE:
		m2fr(f, P_DOUBLE, memvar, "fa0");
		break;
	default:
		assert(0);
	}
	m2ir(f, P_REG, "$ra", "ra");
	gen_sp_shrink(f);
	gen0r(f, "ret");
}

void gen_branch(Func& f, string label, string memvar)
{
	if(memvar[0] == '%')
	{
		assert(get_ptype_from_string(memvar) == P_BOOL);
		if(f.lv_map.count(memvar) != 0)
		{
			size_t sp_pos = f.lv_list[f.lv_map[memvar]].sp_pos;
			gen2r(f, "lb", "t1", to_string(sp_pos) + "(sp)");
		}
		else
		{
			assert(gv_map.count(memvar) != 0);
			gen2r(f, "la", "t0", normalize_name(memvar));
			gen2r(f, "lb", "t1", "0(t0)");
		}
		gen2r(f, "bnez", "t1", normalize_name(label));
	}
	else
	{
		assert((memvar == "true") || (memvar == "false"));
		if(memvar == "true")
		{
			gen1r(f, "j", normalize_name(label));
		}
	}
}

void gen_label(Func& f, string label)
{
	gen0r(f, normalize_name(label) + ":");
}

void gen_assign(Func& f, string dst, string src)
{
	assert(dst[0] == '%' || dst[0] == '$');
	PType dsttype = get_ptype_from_string(dst);
	GType dstvtype;
	switch(dsttype)
	{
	case P_INT:
	case P_PTR_INT:
		dstvtype = G_INT;
		break;
	case P_BOOL:
	case P_PTR_BOOL:
		dstvtype = G_BOOL;
		break;
	case P_FLOAT:
	case P_PTR_FLOAT:
		dstvtype = G_FLOAT;
		break;
	case P_DOUBLE:
	case P_PTR_DOUBLE:
		dstvtype = G_DOUBLE;
		break;
	default:
		assert(false);
	}

	GType srcvtype;
	if(src[0] == '%' || src[0] == '$')
	{
		PType srctype;
		srctype = get_ptype_from_string(src);
		switch(srctype)
		{
		case P_INT:
		case P_PTR_INT:
			srcvtype = G_INT;
			break;
		case P_BOOL:
		case P_PTR_BOOL:
			srcvtype = G_BOOL;
			break;
		case P_FLOAT:
		case P_PTR_FLOAT:
			srcvtype = G_FLOAT;
			break;
		case P_DOUBLE:
		case P_PTR_DOUBLE:
			srcvtype = G_DOUBLE;
			break;
		default:
			assert(false);
		}

		switch(srctype)
		{
		case P_INT:
		case P_BOOL:
		case P_FLOAT:
		case P_DOUBLE:
			m2ir(f, srctype, src, "s5");
			break;
		case P_PTR_INT:
		case P_PTR_BOOL:
		case P_PTR_FLOAT:
		case P_PTR_DOUBLE:
			{
				assert(src.back() == ']');
				size_t name_idx;
				for(name_idx = 0; name_idx < src.size() && src[name_idx] != '[';
				    name_idx++)
					;
				assert(name_idx != src.size());
				string src_arr = src.substr(0, name_idx);
				string src_idx =
				    src.substr(name_idx + 1, src.size() - name_idx - 2);
				m2ir(f, P_INT, src_idx, "s0");
				size_t size = TypeSize[srcvtype];
				m2ir(f, P_INT, to_string(size), "s1");
				gen3r(f, "mulw", "s2", "s0", "s1");
				m2ir(f, srctype, src_arr, "s3");
				gen3r(f, "add", "s4", "s3", "s2");
				gen2r(f, inst_load_i(size), "s5", "0(s4)");
			}
			break;
		default:
			assert(false);
		}
	}
	else
	{
		srcvtype = dstvtype;
		uint64_t val;
		val = str_to_bin(dstvtype, src);
		gen2r(f, "li", "s5", to_string(val));
	}
	assert(srcvtype == dstvtype);

	switch(dsttype)
	{
	case P_INT:
	case P_BOOL:
	case P_FLOAT:
	case P_DOUBLE:
		{
			ir2m(f, "s5", dst);
		}
		break;
	case P_PTR_INT:
	case P_PTR_BOOL:
	case P_PTR_FLOAT:
	case P_PTR_DOUBLE:
		{
			assert(dst.back() = ']');
			size_t name_idx;
			for(name_idx = 0; name_idx < dst.size() && dst[name_idx] != '[';
			    name_idx++)
				;
			assert(name_idx != dst.size());
			string dst_arr = dst.substr(0, name_idx);
			string dst_idx =
			    dst.substr(name_idx + 1, dst.size() - name_idx - 2);
			m2ir(f, P_INT, dst_idx, "s6");
			size_t size = TypeSize[dstvtype];
			m2ir(f, P_INT, to_string(size), "s7");
			gen3r(f, "mulw", "s8", "s6", "s7");
			m2ir(f, dsttype, dst_arr, "s9");
			gen3r(f, "add", "s10", "s9", "s8");
			gen2r(f, inst_store_i(size), "s5", "0(s10)");
		}
		break;
	default:
		assert(false);
	}
}

void gen_addr(Func& f, string dst_p, string src_p, string offset)
{
	m2ir(f, P_INT, offset, "s0");
	PType ptrtype = get_ptype_from_string(src_p);
	assert(get_ptype_from_string(dst_p) == ptrtype);
	PType ptype;
	switch(ptrtype)
	{
	case P_PTR_BOOL:
		ptype = P_BOOL;
		break;
	case P_PTR_INT:
		ptype = P_INT;
		break;
	case P_PTR_FLOAT:
		ptype = P_FLOAT;
		break;
	case P_PTR_DOUBLE:
		ptype = P_DOUBLE;
		break;
	default:
		assert(0);
	}
	size_t size = TypeSize[ptype];
	m2ir(f, P_INT, to_string(size), "s1");
	gen3r(f, "mulw", "s2", "s0", "s1");
	m2ir(f, ptrtype, src_p, "s3");
	gen3r(f, "add", "s4", "s3", "s2");
	ir2m(f, "s4", dst_p);
}

void init_extern_func()
{
	Func print_int = {.name      = "%print_int",
	                  .ftype     = F_VOID,
	                  .p_list    = vector<Param>{{.name    = "%iiii",
	                                              .ptype   = P_INT,
	                                              .reg_pos = 0}}, // a0
	                  .p_map     = map<string, size_t>{{"%iiii", 0}},
	                  .is_extern = true,
	                  .is_used   = (optimize_level == 0)};
	add_func(print_int);
	Func print_bool = {.name      = "%print_bool",
	                   .ftype     = F_VOID,
	                   .p_list    = vector<Param>{{.name    = "%bbbb",
	                                               .ptype   = P_BOOL,
	                                               .reg_pos = 0}}, // a0
	                   .p_map     = map<string, size_t>{{"%bbbb", 0}},
	                   .is_extern = true,
	                   .is_used   = (optimize_level == 0)};
	add_func(print_bool);
	Func print_float = {.name      = "%print_float",
	                    .ftype     = F_VOID,
	                    .p_list    = vector<Param>{{.name    = "%ffff",
	                                                .ptype   = P_FLOAT,
	                                                .reg_pos = 8}}, // fa0
	                    .p_map     = map<string, size_t>{{"%ffff", 0}},
	                    .is_extern = true,
	                    .is_used   = (optimize_level == 0)};
	add_func(print_float);
	Func print_double = {.name      = "%print_double",
	                     .ftype     = F_VOID,
	                     .p_list    = vector<Param>{{.name    = "%dddd",
	                                                 .ptype   = P_DOUBLE,
	                                                 .reg_pos = 8}}, // fa0
	                     .p_map     = map<string, size_t>{{"%dddd", 0}},
	                     .is_extern = true,
	                     .is_used   = (optimize_level == 0)};
	add_func(print_double);
	Func get_int = {.name      = "%get_int",
	                .ftype     = F_INT,
	                .p_list    = vector<Param>{},
	                .p_map     = map<string, size_t>{},
	                .is_extern = true,
	                .is_used   = (optimize_level == 0)};
	add_func(get_int);
	Func get_float = {.name      = "%get_float",
	                  .ftype     = F_FLOAT,
	                  .p_list    = vector<Param>{},
	                  .p_map     = map<string, size_t>{},
	                  .is_extern = true,
	                  .is_used   = (optimize_level == 0)};
	add_func(get_float);
	Func get_double = {.name      = "%get_double",
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
				gv.is_const  = 0; // TODO
				gv.is_unused = 0; // TODO
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
				ga.is_const  = 0; // TODO
				ga.is_unused = 0; // TODO
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
		Func f;
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
	for(Func f : f_list)
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

void o0_gen_asm_func(Func& f)
{
	for(string ir_line : f.ir_lines)
	{
		vector<string> tokens = split_string(ir_line);

		if(tokens.size() == 0)
		{
			continue;
		}
		else if(tokens[0] == "@func")
		{
			// move sp
			gen_sp_expand(f);
			// store ra
			ir2m(f, "ra", "$ra");
			// store gpr and fpr
			for(Param p : f.p_list)
			{
				if(p.reg_pos < 8)
				{
					ir2m(f, "a" + to_string(p.reg_pos), p.name);
				}
				else
				{
					fr2m(f, "fa" + to_string(p.reg_pos - 8), p.name);
				}
			}
		}
		else if(tokens[0] == "@endfunc")
		{
			if(f.ftype == F_VOID)
			{
				gen_return(f, "");
			}
			return;
		}
		else if(tokens[0] == "call")
		{
			/* call %i_55 %i_5 ( %i_58 , %ai_61 ) */
			// load gpr and fpr
			string res_var   = tokens[1];
			string func_name = tokens[2];
			Func   g         = f_list[f_map[func_name]];
			size_t pas       = 0;
			for(size_t pos = 4; pos < tokens.size() - 1; pos++)
			{
				if(tokens[pos] == ",")
				{
					continue;
				}
				if(g.p_list[pas].reg_pos < 8)
				{
					m2ir(f,
					     g.p_list[pas].ptype,
					     tokens[pos],
					     "a" + to_string(g.p_list[pas].reg_pos));
				}
				else
				{
					m2fr(f,
					     g.p_list[pas].ptype,
					     tokens[pos],
					     "fa" + to_string(g.p_list[pas].reg_pos - 8));
				}
				pas++;
			}
			// call
			func_call(f, func_name);
			// store res
			switch(g.ftype)
			{
			case F_INT:
			case F_BOOL:
				ir2m(f, "a0", res_var);
				break;
			case F_FLOAT:
			case F_DOUBLE:
				fr2m(f, "fa0", res_var);
				break;
			case F_VOID:
				assert(res_var == "void");
				break;
			default:
				assert(false);
			}
		}
		else if(tokens[0] == "!global")
		{
			cerr << "global in function: " << f.name << ": " << ir_line << endl;
			assert(false);
		}
		else if(tokens[0] == "assign")
		{
			gen_assign(f, tokens[1], tokens[2]);
		}
		else if(tokens[0] == "@array")
		{
			continue;
		}
		else if(tokens[0] == "@var")
		{
			continue;
		}
		else if(tokens[0] == "DAnd")
		{
			m2ir(f, P_BOOL, tokens[2], "s0");
			m2ir(f, P_BOOL, tokens[3], "s1");
			gen3r(f, "and", "s2", "s0", "s1");
			ir2m(f, "s2", tokens[1]);
		}
		else if(tokens[0] == "DOr")
		{
			m2ir(f, P_BOOL, tokens[2], "s0");
			m2ir(f, P_BOOL, tokens[3], "s1");
			gen3r(f, "or", "s2", "s0", "s1");
			ir2m(f, "s2", tokens[1]);
		}
		else if(tokens[0] == "Add")
		{
			switch(tokens[1][0])
			{
			case 'i':
				{
					m2ir(f, P_INT, tokens[3], "s0");
					m2ir(f, P_INT, tokens[4], "s1");
					gen3r(f, "addw", "s2", "s0", "s1");
					ir2m(f, "s2", tokens[2]);
				}
				break;
			case 'f':
				{
					m2fr(f, P_FLOAT, tokens[3], "fs0");
					m2fr(f, P_FLOAT, tokens[4], "fs1");
					gen3r(f, "fadd.s", "fs2", "fs0", "fs1");
					fr2m(f, "fs2", tokens[2]);
				}
				break;
			case 'd':
				{
					m2fr(f, P_DOUBLE, tokens[3], "fs0");
					m2fr(f, P_DOUBLE, tokens[4], "fs1");
					gen3r(f, "fadd.d", "fs2", "fs0", "fs1");
					fr2m(f, "fs2", tokens[2]);
				}
				break;
			default:
				assert(false);
			}
		}
		else if(tokens[0] == "Addr")
		{
			gen_addr(f, tokens[1], tokens[2], tokens[3]);
		}
		else if(tokens[0] == "Sub")
		{
			switch(tokens[1][0])
			{
			case 'i':
				{
					m2ir(f, P_INT, tokens[3], "s0");
					m2ir(f, P_INT, tokens[4], "s1");
					gen3r(f, "sub", "s2", "s0", "s1");
					ir2m(f, "s2", tokens[2]);
				}
				break;
			case 'f':
				{
					m2fr(f, P_FLOAT, tokens[3], "fs0");
					m2fr(f, P_FLOAT, tokens[4], "fs1");
					gen3r(f, "fsub.s", "fs2", "fs0", "fs1");
					fr2m(f, "fs2", tokens[2]);
				}
				break;
			case 'd':
				{
					m2fr(f, P_DOUBLE, tokens[3], "fs0");
					m2fr(f, P_DOUBLE, tokens[4], "fs1");
					gen3r(f, "fsub.d", "fs2", "fs0", "fs1");
					fr2m(f, "fs2", tokens[2]);
				}
				break;
			default:
				assert(false);
			}
		}
		else if(tokens[0] == "Mul")
		{
			switch(tokens[1][0])
			{
			case 'i':
				{
					m2ir(f, P_INT, tokens[3], "s0");
					m2ir(f, P_INT, tokens[4], "s1");
					gen3r(f, "mulw", "s2", "s0", "s1");
					ir2m(f, "s2", tokens[2]);
				}
				break;
			case 'f':
				{
					m2fr(f, P_FLOAT, tokens[3], "fs0");
					m2fr(f, P_FLOAT, tokens[4], "fs1");
					gen3r(f, "fmul.s", "fs2", "fs0", "fs1");
					fr2m(f, "fs2", tokens[2]);
				}
				break;
			case 'd':
				{
					m2fr(f, P_DOUBLE, tokens[3], "fs0");
					m2fr(f, P_DOUBLE, tokens[4], "fs1");
					gen3r(f, "fmul.d", "fs2", "fs0", "fs1");
					fr2m(f, "fs2", tokens[2]);
				}
				break;
			default:
				assert(false);
			}
		}
		else if(tokens[0] == "Div")
		{
			switch(tokens[1][0])
			{
			case 'i':
				{
					m2ir(f, P_INT, tokens[3], "s0");
					m2ir(f, P_INT, tokens[4], "s1");
					gen3r(f, "divw", "s2", "s0", "s1");
					ir2m(f, "s2", tokens[2]);
				}
				break;
			case 'f':
				{
					m2fr(f, P_FLOAT, tokens[3], "fs0");
					m2fr(f, P_FLOAT, tokens[4], "fs1");
					gen3r(f, "fdiv.s", "fs2", "fs0", "fs1");
					fr2m(f, "fs2", tokens[2]);
				}
				break;
			case 'd':
				{
					m2fr(f, P_DOUBLE, tokens[3], "fs0");
					m2fr(f, P_DOUBLE, tokens[4], "fs1");
					gen3r(f, "fdiv.d", "fs2", "fs0", "fs1");
					fr2m(f, "fs2", tokens[2]);
				}
				break;
			default:
				assert(false);
			}
		}
		else if(tokens[0] == "Rem")
		{
			m2ir(f, P_INT, tokens[2], "s0");
			m2ir(f, P_INT, tokens[3], "s1");
			gen3r(f, "remw", "s2", "s0", "s1");
			ir2m(f, "s2", tokens[1]);
		}
		else if(tokens[0] == "LT")
		{
			switch(tokens[1][0])
			{
			case 'i':
				{
					m2ir(f, P_INT, tokens[3], "s0");
					m2ir(f, P_INT, tokens[4], "s1");
					gen3r(f, "slt", "s2", "s0", "s1");
				}
				break;
			case 'f':
				{
					m2fr(f, P_FLOAT, tokens[3], "fs0");
					m2fr(f, P_FLOAT, tokens[4], "fs1");
					gen3r(f, "flt.s", "s2", "fs0", "fs1");
				}
				break;
			case 'd':
				{
					m2fr(f, P_DOUBLE, tokens[3], "fs0");
					m2fr(f, P_DOUBLE, tokens[4], "fs1");
					gen3r(f, "flt.d", "s2", "fs0", "fs1");
				}
				break;
			default:
				assert(false);
			}
			ir2m(f, "s2", tokens[2]);
		}
		else if(tokens[0] == "LE")
		{
			switch(tokens[1][0])
			{
			case 'i':
				{
					m2ir(f, P_INT, tokens[3], "s0");
					m2ir(f, P_INT, tokens[4], "s1");
					gen3r(f, "slt", "s2", "s1", "s0");
					gen2r(f, "seqz", "s2", "s2");
				}
				break;
			case 'f':
				{
					m2fr(f, P_FLOAT, tokens[3], "fs0");
					m2fr(f, P_FLOAT, tokens[4], "fs1");
					gen3r(f, "fle.s", "s2", "fs0", "fs1");
				}
				break;
			case 'd':
				{
					m2fr(f, P_DOUBLE, tokens[3], "fs0");
					m2fr(f, P_DOUBLE, tokens[4], "fs1");
					gen3r(f, "fle.d", "s2", "fs0", "fs1");
				}
				break;
			default:
				assert(false);
			}
			ir2m(f, "s2", tokens[2]);
		}
		else if(tokens[0] == "GT")
		{
			switch(tokens[1][0])
			{
			case 'i':
				{
					m2ir(f, P_INT, tokens[3], "s0");
					m2ir(f, P_INT, tokens[4], "s1");
					gen3r(f, "slt", "s2", "s1", "s0");
				}
				break;
			case 'f':
				{
					m2fr(f, P_FLOAT, tokens[3], "fs0");
					m2fr(f, P_FLOAT, tokens[4], "fs1");
					gen3r(f, "flt.s", "s2", "fs1", "fs0");
				}
				break;
			case 'd':
				{
					m2fr(f, P_DOUBLE, tokens[3], "fs0");
					m2fr(f, P_DOUBLE, tokens[4], "fs1");
					gen3r(f, "flt.d", "s2", "fs1", "fs0");
				}
				break;
			default:
				assert(false);
			}
			ir2m(f, "s2", tokens[2]);
		}
		else if(tokens[0] == "GE")
		{
			switch(tokens[1][0])
			{
			case 'i':
				{
					m2ir(f, P_INT, tokens[3], "s0");
					m2ir(f, P_INT, tokens[4], "s1");
					gen3r(f, "slt", "s2", "s0", "s1");
					gen2r(f, "seqz", "s2", "s2");
				}
				break;
			case 'f':
				{
					m2fr(f, P_FLOAT, tokens[3], "fs0");
					m2fr(f, P_FLOAT, tokens[4], "fs1");
					gen3r(f, "fle.s", "s2", "fs1", "fs0");
				}
				break;
			case 'd':
				{
					m2fr(f, P_DOUBLE, tokens[3], "fs0");
					m2fr(f, P_DOUBLE, tokens[4], "fs1");
					gen3r(f, "fle.d", "s2", "fs1", "fs0");
				}
				break;
			default:
				assert(false);
			}
			ir2m(f, "s2", tokens[2]);
		}
		else if(tokens[0] == "EQ")
		{
			switch(tokens[1][0])
			{
			case 'i':
				{
					m2ir(f, P_INT, tokens[3], "s0");
					m2ir(f, P_INT, tokens[4], "s1");
					gen3r(f, "xor", "s2", "s0", "s1");
					gen2r(f, "seqz", "s3", "s2");
				}
				break;
			case 'f':
				{
					m2fr(f, P_FLOAT, tokens[3], "fs0");
					m2fr(f, P_FLOAT, tokens[4], "fs1");
					gen3r(f, "feq.s", "s3", "fs1", "fs0");
				}
				break;
			case 'd':
				{
					m2fr(f, P_DOUBLE, tokens[3], "fs0");
					m2fr(f, P_DOUBLE, tokens[4], "fs1");
					gen3r(f, "feq.d", "s3", "fs1", "fs0");
				}
				break;
			default:
				assert(false);
			}
			ir2m(f, "s3", tokens[2]);
		}
		else if(tokens[0] == "NE")
		{
			switch(tokens[1][0])
			{
			case 'i':
				{
					m2ir(f, P_INT, tokens[3], "s0");
					m2ir(f, P_INT, tokens[4], "s1");
					gen3r(f, "xor", "s2", "s0", "s1");
					gen2r(f, "snez", "s3", "s2");
				}
				break;
			case 'f':
				{
					m2fr(f, P_FLOAT, tokens[3], "fs0");
					m2fr(f, P_FLOAT, tokens[4], "fs1");
					gen3r(f, "feq.s", "s2", "fs1", "fs0");
					gen2r(f, "seqz", "s3", "s2");
				}
				break;
			case 'd':
				{
					m2fr(f, P_DOUBLE, tokens[3], "fs0");
					m2fr(f, P_DOUBLE, tokens[4], "fs1");
					gen3r(f, "feq.d", "s2", "fs1", "fs0");
					gen2r(f, "seqz", "s3", "s2");
				}
				break;
			default:
				assert(false);
			}
			ir2m(f, "s3", tokens[2]);
		}
		else if(tokens[0] == "Neg")
		{
			switch(tokens[1][0])
			{
			case 'i':
				{
					m2ir(f, P_INT, tokens[3], "s0");
					gen3r(f, "sub", "s1", "zero", "s0");
					ir2m(f, "s1", tokens[2]);
				}
				break;
			case 'f':
				{
					m2fr(f, P_FLOAT, tokens[3], "fs0");
					gen2r(f, "fneg.s", "fs1", "fs0");
					fr2m(f, "fs1", tokens[2]);
				}
				break;
			case 'd':
				{
					m2fr(f, P_DOUBLE, tokens[3], "fs0");
					gen2r(f, "fneg.d", "fs1", "fs0");
					fr2m(f, "fs1", tokens[2]);
				}
				break;
			default:
				assert(false);
			}
		}
		else if(tokens[0] == "Not")
		{
			m2ir(f, P_BOOL, tokens[2], "s0");
			gen2r(f, "seqz", "s1", "s0");
			ir2m(f, "s1", tokens[1]);
		}
		else if(tokens[0] == "Pos")
		{
			assert(false); // not exist
		}
		else if(tokens[0] == "branch")
		{
			gen_branch(f, tokens[1], tokens[2]);
		}
		else if(tokens[0] == "label")
		{
			gen_label(f, tokens[1]);
		}
		else if(tokens[0] == "return")
		{
			gen_return(f, tokens[1]);
		}
		else if(tokens[0] == "retire")
		{
			continue;
		}
		else
		{
			cerr << "unknown ir: " << ir_line << endl;
		}
	}
}

void o0_gen_asm()
{
	for(Func& f : f_list)
	{
		if(!f.is_used || f.is_extern)
		{
			continue;
		}
		o0_gen_asm_func(f);
	}
}

extern set<string> o1_var_set;

void put_global()
{
	for(GlobalVar gv : gv_list)
	{
		if(o1_var_set.count(gv.name) == 0)
		{
			continue;
		}
		string new_name = normalize_name(gv.name);
		size_t var_size = TypeSize[gv.gtype];
		if(gv.is_unused)
		{
			continue;
		}
		if(gv.is_bss)
		{
			asm_lines.push_back(".bss");
		}
		else if(gv.is_const)
		{
			asm_lines.push_back(".rodata");
		}
		else
		{
			asm_lines.push_back(".data");
		}
		asm_lines.push_back(".balign " + to_string(var_size));
		if(gv.is_bss)
		{
			asm_lines.push_back(".lcomm " + new_name + ", " +
			                    to_string(var_size));
			continue;
		}
		asm_lines.push_back(".global " + new_name);
		asm_lines.push_back(".size " + new_name + ", " + to_string(var_size));
		asm_lines.push_back(new_name + ":");
		for(int i = 0; i < var_size; i++)
		{
			asm_lines.push_back("\t.byte " +
			                    to_string((gv.value >> (i << 3)) & 0xFF));
		}
		asm_lines.push_back("");
	}
	for(GlobalArr ga : ga_list)
	{
		if(o1_var_set.count(ga.name) == 0)
		{
			continue;
		}
		string new_name = normalize_name(ga.name);
		size_t arr_lign = TypeSize[ga.gtype];
		size_t arr_lens = ga.length;
		if(ga.is_unused)
		{
			continue;
		}
		if(ga.is_bss)
		{
			asm_lines.push_back(".bss");
		}
		else if(ga.is_const)
		{
			asm_lines.push_back(".rodata");
		}
		else
		{
			asm_lines.push_back(".data");
		}
		asm_lines.push_back(".balign " + to_string(arr_lign));
		if(ga.is_bss)
		{
			asm_lines.push_back(".lcomm " + new_name + ", " +
			                    to_string(arr_lens * arr_lign));
			continue;
		}
		asm_lines.push_back(".global " + new_name);
		asm_lines.push_back(".size " + new_name + ", " +
		                    to_string(arr_lens * arr_lign));

		asm_lines.push_back(new_name + ":");
		for(int i = 0; i < arr_lens; i++)
		{
			for(int j = 0; j < arr_lign; j++)
			{
				asm_lines.push_back(
				    "\t.byte " + to_string((ga.values[i] >> (j << 3)) & 0xFF));
			}
		}
		asm_lines.push_back("");
	}
}

void put_func()
{
	for(Func f : f_list)
	{
		string new_name = normalize_name(f.name);
		if(!f.is_used)
		{
			continue;
		}
		if(f.is_extern)
		{
			asm_lines.push_back(".extern " + new_name);
			continue;
		}
		asm_lines.push_back(".text");
		asm_lines.push_back(".balign 2");
		asm_lines.push_back(".global " + new_name);
		asm_lines.push_back(new_name + ":");
		for(string asm_line : f.asm_lines)
		{
			asm_lines.push_back("\t" + asm_line);
		}
		asm_lines.push_back(".type " + new_name + ", @function");
		asm_lines.push_back(".size " + new_name + ", .-" + new_name);
	}
}

map<string, set<string>> o1_func_rely;

void o1_handle_func_rely()
{
	queue<string> qf;
	qf.push("%main");
	f_list[f_map["%main"]].is_used = true;

	while(!qf.empty())
	{
		string func_name = qf.front();
		assert(f_map.count(func_name) != 0);
		qf.pop();

		for(string sub_name : o1_func_rely[func_name])
		{
			assert(f_map.count(sub_name) != 0);
			if(f_list[f_map[sub_name]].is_used == false)
			{
				f_list[f_map[sub_name]].is_used = true;
				qf.push(sub_name);
			}
		}
	}
}

map<string, set<string>> o1_var_rely;
set<string>              o1_var_set;

void o1_build_var_rely()
{
	for(Func f : f_list)
	{
		string new_name = normalize_name(f.name);
		if(!f.is_used || f.is_extern)
		{
			continue;
		}
		for(string ir_line : f.ir_lines)
		{
			string edited_line;
			for(auto ch : ir_line)
			{
				if(ch == '[' || ch == ']')
				{
					ch = ' ';
				}
				edited_line += ch;
			}
			vector<string> tokens = split_string(edited_line);

			if(tokens.size() == 0)
			{
				continue;
			}
			else if(tokens[0] == "@func")
			{
				continue;
			}
			else if(tokens[0] == "@endfunc")
			{
				continue;
			}
			else if(tokens[0] == "call")
			{
				for(size_t pos = 4; pos < tokens.size() - 1; pos++)
				{
					if(tokens[pos] == ",")
					{
						continue;
					}
					//assert(tokens[pos][0] == '%');
					o1_var_set.insert(tokens[pos]);
				}
			}
			else if(tokens[0] == "!global")
			{
				assert(false);
			}
			else if(tokens[0] == "assign")
			{
				assert(tokens[1][0] == '%');
				for(size_t pos = 2; pos < tokens.size(); pos++)
				{
					if(tokens[pos][0] == '%')
					{
						o1_var_rely[tokens[1]].insert(tokens[pos]);
					}
				}
			}
			else if(tokens[0] == "@array")
			{
				continue;
			}
			else if(tokens[0] == "@var")
			{
				continue;
			}
			else if(tokens[0] == "DAnd")
			{
				if(tokens[2][0] == '%')
				{
					o1_var_rely[tokens[1]].insert(tokens[2]);
				}
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[1]].insert(tokens[3]);
				}
			}
			else if(tokens[0] == "DOr")
			{
				if(tokens[2][0] == '%')
				{
					o1_var_rely[tokens[1]].insert(tokens[2]);
				}
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[1]].insert(tokens[3]);
				}
			}
			else if(tokens[0] == "Add")
			{
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[3]);
				}
				if(tokens[4][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[4]);
				}
			}
			else if(tokens[0] == "Addr")
			{
				if(tokens[2][0] == '%')
				{
					o1_var_rely[tokens[1]].insert(tokens[2]);
				}
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[1]].insert(tokens[3]);
				}
			}
			else if(tokens[0] == "Sub")
			{
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[3]);
				}
				if(tokens[4][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[4]);
				}
			}
			else if(tokens[0] == "Mul")
			{
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[3]);
				}
				if(tokens[4][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[4]);
				}
			}
			else if(tokens[0] == "Div")
			{
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[3]);
				}
				if(tokens[4][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[4]);
				}
			}
			else if(tokens[0] == "Rem")
			{
				if(tokens[2][0] == '%')
				{
					o1_var_rely[tokens[1]].insert(tokens[2]);
				}
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[1]].insert(tokens[3]);
				}
			}
			else if(tokens[0] == "LT")
			{
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[3]);
				}
				if(tokens[4][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[4]);
				}
			}
			else if(tokens[0] == "LE")
			{
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[3]);
				}
				if(tokens[4][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[4]);
				}
			}
			else if(tokens[0] == "GT")
			{
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[3]);
				}
				if(tokens[4][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[4]);
				}
			}
			else if(tokens[0] == "GE")
			{
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[3]);
				}
				if(tokens[4][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[4]);
				}
			}
			else if(tokens[0] == "EQ")
			{
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[3]);
				}
				if(tokens[4][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[4]);
				}
			}
			else if(tokens[0] == "NE")
			{
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[3]);
				}
				if(tokens[4][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[4]);
				}
			}
			else if(tokens[0] == "Neg")
			{
				if(tokens[3][0] == '%')
				{
					o1_var_rely[tokens[2]].insert(tokens[3]);
				}
			}
			else if(tokens[0] == "Not")
			{
				if(tokens[2][0] == '%')
				{
					o1_var_rely[tokens[1]].insert(tokens[2]);
				}
			}
			else if(tokens[0] == "Pos")
			{
				assert(false);
			}
			else if(tokens[0] == "branch")
			{
				o1_var_set.insert(tokens[2]);
			}
			else if(tokens[0] == "label")
			{
				continue;
			}
			else if(tokens[0] == "return")
			{
				if(f.ftype != F_VOID)
				{
					o1_var_set.insert(tokens[1]);
				}
			}
			else if(tokens[0] == "retire")
			{
				continue;
			}
			else
			{
				assert(false);
			}
		}
	}
}

void o1_handle_var_rely()
{
	queue<string> qv;
	for(auto uv : o1_var_set)
	{
		qv.push(uv);
	}
	while(!qv.empty())
	{
		string var_name = qv.front();
		qv.pop();
		for(string rely_name : o1_var_rely[var_name])
		{
			if(o1_var_set.count(rely_name) == 0)
			{
				o1_var_set.insert(rely_name);
				qv.push(rely_name);
			}
		}
	}
}

void o1_print_var_rely()
{
	for(auto uv : o1_var_set)
	{
		cerr << "used: " << uv << endl;
	}
	for(auto vr : o1_var_rely)
	{
		cerr << "rely: " << vr.first << ":" << endl;
		for(auto rely_name : vr.second)
		{
			cerr << "  " << rely_name << endl;
		}
	}
}

void o1_use_var_rely()
{
	for(Func& f : f_list)
	{
		string new_name = normalize_name(f.name);
		if(!f.is_used || f.is_extern)
		{
			continue;
		}
		for(size_t idx = 0; idx < f.ir_lines.size(); idx++)
		{
			string ir_line = f.ir_lines[idx];
			string edited_line;
			for(auto ch : ir_line)
			{
				if(ch == '[' || ch == ']')
				{
					ch = ' ';
				}
				edited_line += ch;
			}
			vector<string> tokens = split_string(edited_line);

			if(tokens.size() == 0)
			{
				continue;
			}
			else if(tokens[0] == "@func")
			{
				continue;
			}
			else if(tokens[0] == "@endfunc")
			{
				continue;
			}
			else if(tokens[0] == "call")
			{
				continue;
			}
			else if(tokens[0] == "!global")
			{
				assert(false);
			}
			else if(tokens[0] == "assign")
			{
				if(o1_var_set.count(tokens[1]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "@array")
			{
				continue;
			}
			else if(tokens[0] == "@var")
			{
				continue;
			}
			else if(tokens[0] == "DAnd")
			{
				if(o1_var_set.count(tokens[1]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "DOr")
			{
				if(o1_var_set.count(tokens[1]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "Add")
			{
				if(o1_var_set.count(tokens[2]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "Addr")
			{
				if(o1_var_set.count(tokens[1]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "Sub")
			{
				if(o1_var_set.count(tokens[2]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "Mul")
			{
				if(o1_var_set.count(tokens[2]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "Div")
			{
				if(o1_var_set.count(tokens[2]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "Rem")
			{
				if(o1_var_set.count(tokens[2]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "LT")
			{
				if(o1_var_set.count(tokens[2]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "LE")
			{
				if(o1_var_set.count(tokens[2]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "GT")
			{
				if(o1_var_set.count(tokens[2]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "GE")
			{
				if(o1_var_set.count(tokens[2]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "EQ")
			{
				if(o1_var_set.count(tokens[2]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "NE")
			{
				if(o1_var_set.count(tokens[2]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "Neg")
			{
				if(o1_var_set.count(tokens[2]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "Not")
			{
				if(o1_var_set.count(tokens[1]) == 0)
				{
					f.ir_lines[idx] = "";
				}
				continue;
			}
			else if(tokens[0] == "Pos")
			{
				assert(false);
			}
			else if(tokens[0] == "branch")
			{
				continue;
			}
			else if(tokens[0] == "label")
			{
				continue;
			}
			else if(tokens[0] == "return")
			{
				continue;
			}
			else if(tokens[0] == "retire")
			{
				continue;
			}
			else
			{
				assert(false);
			}
		}
	}
}

void asmgen()
{
	init_extern_func();
	parse_ir();
	show_ir();
	if(optimize_level == 1)
	{
		o1_handle_func_rely();

		o1_build_var_rely();
		o1_print_var_rely();
		o1_handle_var_rely();
		o1_print_var_rely();
		o1_use_var_rely();
		show_ir();
	}
	o0_gen_asm();
	put_global();
	put_func();
}
