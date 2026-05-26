#include "interpreter.h"

Table                       gtable;
vector<Local_State>         lstates;
vector<string>              lstrs;
map<string, vector<string>> func_params;

void func_init(string name, int line, IdentType type, vector<string> params)
{
	assert(gtable[name].val_type == IT_NONE);
	gtable[name].name     = name;
	gtable[name].val_type = type;
	gtable[name].val.line = line;
	func_params[name]     = params;
}

void io_func_init()
{
	func_init("%print_int", -1, IT_FUNC_VOID, {"%i_-1"});
	func_init("%print_float", -2, IT_FUNC_VOID, {"%f_-2"});
	func_init("%print_double", -3, IT_FUNC_VOID, {"%d_-3"});
	func_init("%print_bool", -4, IT_FUNC_VOID, {"%b_-4"});
	func_init("%get_int", -5, IT_FUNC_INT, {});
	func_init("%get_float", -6, IT_FUNC_FLOAT, {});
	func_init("%get_double", -7, IT_FUNC_DOUBLE, {});
}

void io_exec(Local_State &lstate, Value &val)
{
	switch(lstate.curline)
	{
	case -1:
		cerr << "print int: " << lstate.ltable["%i_-1"].val.ival << endl;
		cout << lstate.ltable["%i_-1"].val.ival << endl;
		break;
	case -2:
    fprintf(stderr, "print float: %.6f\n", lstate.ltable["%f_-2"].val.fval);
    fprintf(stdout, "%.6f\n", lstate.ltable["%f_-2"].val.fval);
		break;
	case -3:
    fprintf(stderr, "print double: %.6lf\n", lstate.ltable["%d_-3"].val.dval);
    fprintf(stdout, "%.6lf\n", lstate.ltable["%d_-3"].val.dval);
		break;
	case -4:
		cerr << "print bool: " << (lstate.ltable["%b_-4"].val.bval ? "true" : "false") << endl;
		cout << (lstate.ltable["%b_-4"].val.bval ? "true" : "false") << endl;
		break;
	case -5:
		cin >> val.ival;
		break;
	case -6:
		cin >> val.fval;
		break;
	case -7:
		cin >> val.dval;
		break;
	default:
		assert(false);
	}
}

void init_g_assign(string lstr)
{
	cerr << "init_g_assign: " << lstr << endl;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "!global");
	assert(tokens[1] == "assign");
	assert(tokens[2][0] == '%');
	if(tokens[2][1] != 'a')
	{
		string    name  = tokens[2];
		string    val   = tokens[3];
		IdentType vtype = gtable[name].val_type;
		assert(vtype != IT_NONE);
		gtable[name].val = get_val(vtype, g_lstate, val);
	}
	else
	{
		string    name  = get_array_name(tokens[2]);
		int       index = get_array_index(g_lstate, tokens[2]);
		IdentType vtype = vtype_array_to_var(gtable[name].val_type);
		string    val   = tokens[3];
		gtable[name].val.aval[index] = get_val(vtype, g_lstate, val);
	}
}

void init_g_array(string lstr)
{
	cerr << "init_g_array: " << lstr << endl;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "!global");
	int idx = 1;
	if(tokens[idx] == "const") idx = 2;
	assert(tokens[idx] == "@array");
	string name = tokens[idx + 1];
	assert(name[0] == '%');
	assert(name[1] == 'a');
	assert(gtable[name].val_type == IT_NONE);
	int size              = get_val_int(g_lstate, tokens[idx + 2]).ival;
	gtable[name].name     = name;
	gtable[name].val_type = get_var_type_by_prefix(name);
	gtable[name].val.aval = (Value *)calloc(size, sizeof(Value));
}

void init_g_var(string lstr)
{
	cerr << "init_g_var: " << lstr << endl;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "!global");
	int idx = 1;
	if(tokens[idx] == "const") idx = 2;
	assert(tokens[idx] == "@var");
	string name = tokens[idx + 1];
	assert(name[0] == '%');
	assert(gtable[name].val_type == IT_NONE);
	IdentType vtype       = get_var_type_by_prefix(name);
	gtable[name].name     = name;
	gtable[name].val_type = vtype;
	gtable[name].val.ival = 0;
}

void local_assign(Local_State &lstate, string name, Value val, IdentType vtype)
{
	cerr << "local_assign: " << name << " ";
	print_val(val, vtype);
	cerr << endl;
	assert(name[0] == '%');
	switch(vtype)
	{
	case IT_INT:
	case IT_FLOAT:
	case IT_DOUBLE:
	case IT_BOOL:
		if(name[1] != 'a')
		{
			if(gtable[name].val_type != IT_NONE)
			{
				assert(gtable[name].val_type == vtype);
				gtable[name].val = val;
			}
			else
			{
				assert(lstate.ltable[name].val_type == vtype);
				lstate.ltable[name].val = val;
			}
		}
		else
		{
			string arrname = get_array_name(name);
			if(gtable[arrname].val_type != IT_NONE)
			{
				assert(vtype_array_to_var(gtable[arrname].val_type) == vtype);
				int index                       = get_array_index(lstate, name);
				gtable[arrname].val.aval[index] = val;
			}
			else
			{
				assert(vtype_array_to_var(lstate.ltable[arrname].val_type) ==
				       vtype);
				int index = get_array_index(lstate, name);
				lstate.ltable[arrname].val.aval[index] = val;
			}
		}
		break;
	case IT_ARRAY_INT:
	case IT_ARRAY_FLOAT:
	case IT_ARRAY_DOUBLE:
	case IT_ARRAY_BOOL:
		assert(name[1] == 'a');
		assert(gtable[name].val_type == IT_NONE);
		assert(lstate.ltable[name].val_type == vtype);
		lstate.ltable[name].val = val;
		break;
	default:
		assert(false);
	}
}

void init_assign(Local_State &lstate, string lstr)
{
	cerr << "init_assign: " << lstr << endl;
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "assign");
	assert(tokens[1][0] == '%');
	local_assign(
	    lstate,
	    tokens[1],
	    get_val(get_final_var_type_by_prefix(tokens[1]), lstate, tokens[2]),
	    get_final_var_type_by_prefix(tokens[1]));
}

void init_array(Local_State &lstate, string lstr)
{
	cerr << "init_array: " << lstr << endl;
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "@array");
	assert(tokens[1][0] == '%');
	assert(tokens[1][1] == 'a');
	string name = tokens[1];
	assert(lstate.ltable[name].val_type == IT_NONE);
	assert(gtable[name].val_type == IT_NONE);
	int size                     = get_val_int(lstate, tokens[2]).ival;
	lstate.ltable[name].name     = name;
	lstate.ltable[name].val_type = get_var_type_by_prefix(tokens[1]);
	lstate.ltable[name].val.aval = (Value *)calloc(size, sizeof(Value));
}

void init_var(Local_State &lstate, string lstr)
{
	cerr << "init_var: " << lstr << endl;
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "@var");
	assert(tokens[1][0] == '%');
	string name = tokens[1];
	assert(gtable[name].val_type == IT_NONE);
	IdentType vtype              = get_var_type_by_prefix(tokens[1]);
	lstate.ltable[name].name     = name;
	lstate.ltable[name].val_type = vtype;
	lstate.ltable[name].val.ival = 0;
}

void init_arg(Local_State &lstate, string name, Value val)
{
	cerr << "init_arg: " << name << " ";
	print_val(val, get_var_type_by_prefix(name));
	cerr << endl;
	assert(gtable[name].val_type == IT_NONE);
	assert(lstate.ltable[name].val_type == IT_NONE);
	assert(name[0] == '%');
	lstate.ltable[name].name     = name;
	lstate.ltable[name].val_type = get_var_type_by_prefix(name);
	lstate.ltable[name].val      = val;
}

void init_func_def(string lstr, int line)
{
	cerr << "init_func_def: " << lstr << " " << line << endl;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "@func");
	string         name = tokens[1];
	vector<string> params;
	for(int i = 2; i < tokens.size(); i++)
	{
		if(tokens[i][0] == '%')
		{
			params.push_back(tokens[i]);
		}
	}
	func_init(name, line, get_func_type_by_prefix(name), params);
}

void add_label(Local_State &lstate, string lstr, int line)
{
	cerr << "add_label: " << lstr << endl;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "label");
	string name                  = tokens[1];
	lstate.ltable[name].name     = name;
	lstate.ltable[name].val_type = IT_LABEL;
	lstate.ltable[name].val.line = line;
}

Local_State func_init_label(string func_name)
{
	assert(gtable[func_name].val_type == IT_FUNC_INT ||
	       gtable[func_name].val_type == IT_FUNC_FLOAT ||
	       gtable[func_name].val_type == IT_FUNC_DOUBLE ||
	       gtable[func_name].val_type == IT_FUNC_BOOL ||
	       gtable[func_name].val_type == IT_FUNC_VOID);
	if(gtable[func_name].val.line < 0)
	{
		return g_lstate;
	}
	Local_State lstate;
	bool        is_end = false;
	for(int i = gtable[func_name].val.line; !is_end; i++)
	{
		switch(get_op_type(lstrs[i]))
		{
		case OP_G_VAR:
		case OP_G_ARRAY:
		case OP_G_ASSIGN:
			assert(false);
			break;
		case OP_LABEL:
			add_label(lstate, lstrs[i], i);
			break;
		case OP_FUNC_END:
			is_end = true;
			break;
		default:
			break;
		}
	}
	return lstate;
}

void gtable_init()
{
	io_func_init();
	for(int i = 0; i < lstrs.size(); i++)
	{
		auto lstr = lstrs[i];
		cerr << "init: " << lstr << endl;
		switch(get_op_type(lstr))
		{
		case OP_FUNC_DEF:
			init_func_def(lstr, i);
			break;
		case OP_G_ASSIGN:
			init_g_assign(lstr);
			break;
		case OP_G_ARRAY:
			init_g_array(lstr);
			break;
		case OP_G_VAR:
			init_g_var(lstr);
			break;
		default:
			break;
		}
	}
	for(auto it = gtable.begin(); it != gtable.end(); it++)
	{
		cerr << "global: " << it->first << " " << it->second.val_type << endl;
	}

	assert(gtable["%main"].val_type == IT_FUNC_INT);
}
