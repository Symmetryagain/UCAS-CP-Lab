#include "interpreter.h"

#include <vector>

Table                       gtable;
vector<Local_State>         lstates;
vector<string>              lstrs;
map<string, vector<string>> func_params;

vector<string> splitString(string const &str)
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
		cout << "print int: " << lstate.ltable["%i_-1"].val.ival << endl;
		break;
	case -2:
		cout << "print float: " << lstate.ltable["%f_-2"].val.fval << endl;
		break;
	case -3:
		cout << "print double: " << lstate.ltable["%d_-3"].val.dval << endl;
		break;
	case -4:
		cout << "print bool: " << (lstate.ltable["%b_-4"].val.bval ? "true" : "false") << endl;
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

OpType get_op_type(string lstr)
{
	vector<string> tokens = splitString(lstr);
	if(tokens.size() == 0)
	{
		return OP_EMPTY;
	}
	else if(tokens[0] == "@func")
	{
		return OP_FUNC_DEF;
	}
	else if(tokens[0] == "@endfunc")
	{
		return OP_FUNC_END;
	}
	else if(tokens[0] == "call")
	{
		return OP_FUNC_CALL;
	}
	else if(tokens[0] == "!global")
	{
		assert(tokens.size() > 1);
		if(tokens[1] == "assign")
		{
			return OP_G_ASSIGN;
		}
		else if(tokens[1] == "@array")
		{
			return OP_G_ARRAY;
		}
		else if(tokens[1] == "@var")
		{
			return OP_G_VAR;
		}
		else
		{
			assert(false);
		}
	}
	else if(tokens[0] == "assign")
	{
		return OP_ASSIGN;
	}
	else if(tokens[0] == "@array")
	{
		return OP_ARRAY;
	}
	else if(tokens[0] == "@var")
	{
		return OP_VAR;
	}
	else if(tokens[0] == "DAnd")
	{
		return OP_AND;
	}
	else if(tokens[0] == "DOr")
	{
		return OP_OR;
	}
	else if(tokens[0] == "Add")
	{
		return OP_ADD;
	}
	else if(tokens[0] == "Addr")
	{
		return OP_ADDR;
	}
	else if(tokens[0] == "Sub")
	{
		return OP_SUB;
	}
	else if(tokens[0] == "Mul")
	{
		return OP_MUL;
	}
	else if(tokens[0] == "Div")
	{
		return OP_DIV;
	}
	else if(tokens[0] == "Rem")
	{
		return OP_REM;
	}
	else if(tokens[0] == "LT")
	{
		return OP_LT;
	}
	else if(tokens[0] == "LE")
	{
		return OP_LE;
	}
	else if(tokens[0] == "GT")
	{
		return OP_GT;
	}
	else if(tokens[0] == "GE")
	{
		return OP_GE;
	}
	else if(tokens[0] == "EQ")
	{
		return OP_EQ;
	}
	else if(tokens[0] == "NE")
	{
		return OP_NE;
	}
	else if(tokens[0] == "Neg")
	{
		return OP_NEG;
	}
	else if(tokens[0] == "Not")
	{
		return OP_NOT;
	}
	else if(tokens[0] == "Pos")
	{
		return OP_POS;
	}
	else if(tokens[0] == "branch")
	{
		return OP_BRANCH;
	}
	else if(tokens[0] == "label")
	{
		return OP_LABEL;
	}
	else if(tokens[0] == "return")
	{
		return OP_RETURN;
	}
	else if(tokens[0] == "retire")
	{
		return OP_RETIRE;
	}
	assert(false);
}

IdentType vtype_var_to_array(IdentType vtype)
{
	switch(vtype)
	{
	case IT_INT:
		return IT_ARRAY_INT;
	case IT_FLOAT:
		return IT_ARRAY_FLOAT;
	case IT_DOUBLE:
		return IT_ARRAY_DOUBLE;
	case IT_BOOL:
		return IT_ARRAY_BOOL;
	default:
		assert(false);
	}
}

IdentType vtype_array_to_var(IdentType vtype)
{
	switch(vtype)
	{
	case IT_ARRAY_INT:
		return IT_INT;
	case IT_ARRAY_FLOAT:
		return IT_FLOAT;
	case IT_ARRAY_DOUBLE:
		return IT_DOUBLE;
	case IT_ARRAY_BOOL:
		return IT_BOOL;
	default:
		assert(false);
	}
}

IdentType vtype_var_to_func(IdentType vtype)
{
	switch(vtype)
	{
	case IT_INT:
		return IT_FUNC_INT;
	case IT_FLOAT:
		return IT_FUNC_FLOAT;
	case IT_DOUBLE:
		return IT_FUNC_DOUBLE;
	case IT_BOOL:
		return IT_FUNC_BOOL;
	default:
		assert(false);
	}
}

IdentType vtype_func_to_var(IdentType vtype)
{
	switch(vtype)
	{
	case IT_FUNC_INT:
		return IT_INT;
	case IT_FUNC_FLOAT:
		return IT_FLOAT;
	case IT_FUNC_DOUBLE:
		return IT_DOUBLE;
	case IT_FUNC_BOOL:
		return IT_BOOL;
	case IT_FUNC_VOID:
		return IT_INT; // void function returns 0
	default:
		assert(false);
	}
}

IdentType get_func_type_by_prefix(string func_name)
{
	if(func_name == "%main")
	{
		return IT_FUNC_INT;
	}
	else if(func_name == "%print_int")
	{
		return IT_FUNC_VOID;
	}
	else if(func_name == "%print_float")
	{
		return IT_FUNC_VOID;
	}
	else if(func_name == "%print_double")
	{
		return IT_FUNC_VOID;
	}
	else if(func_name == "%print_bool")
	{
		return IT_FUNC_VOID;
	}
	else if(func_name == "%get_int")
	{
		return IT_FUNC_INT;
	}
	else if(func_name == "%get_float")
	{
		return IT_FUNC_FLOAT;
	}
	else if(func_name == "%get_double")
	{
		return IT_FUNC_DOUBLE;
	}
	assert(func_name[0] == '%');
	switch(func_name[1])
	{
	case 'i':
		return IT_FUNC_INT;
	case 'f':
		return IT_FUNC_FLOAT;
	case 'd':
		return IT_FUNC_DOUBLE;
	case 'b':
		return IT_FUNC_BOOL;
	case 'v':
		return IT_FUNC_VOID;
	default:
		assert(false);
	}
}

IdentType get_var_type_by_prefix(string var_name)
{
	assert(var_name[0] == '%');
	switch(var_name[1])
	{
	case 'i':
		return IT_INT;
	case 'f':
		return IT_FLOAT;
	case 'd':
		return IT_DOUBLE;
	case 'b':
		return IT_BOOL;
	case 'a':
		switch(var_name[2])
		{
		case 'i':
			return IT_ARRAY_INT;
		case 'f':
			return IT_ARRAY_FLOAT;
		case 'd':
			return IT_ARRAY_DOUBLE;
		case 'b':
			return IT_ARRAY_BOOL;
		default:
			assert(false);
		}
	default:
		assert(false);
	}
}

IdentType get_final_var_type_by_prefix(string var_name)
{
	switch(var_name[1])
	{
	case 'i':
		return IT_INT;
	case 'f':
		return IT_FLOAT;
	case 'd':
		return IT_DOUBLE;
	case 'b':
		return IT_BOOL;
	case 'a':
		switch(var_name[2])
		{
		case 'i':
			return IT_INT;
		case 'f':
			return IT_FLOAT;
		case 'd':
			return IT_DOUBLE;
		case 'b':
			return IT_BOOL;
		default:
			assert(false);
		}
	default:
		assert(false);
	}
}

string get_array_name(string rstr)
{
	cerr << "get_array_name: " << rstr << endl;
	assert(rstr[0] == '%');
	assert(rstr[1] == 'a');
	size_t pos = rstr.find('[');
	assert(pos != string::npos);
	return rstr.substr(0, pos);
}

int get_array_index(Local_State lstate, string rstr)
{
	cerr << "get_array_index: " << rstr << endl;
	assert(rstr[0] == '%');
	assert(rstr[1] == 'a');
	size_t pos1 = rstr.find('[');
	size_t pos2 = rstr.find(']');
	assert(pos1 != string::npos);
	assert(pos2 != string::npos);
	assert(pos1 < pos2);
	string index_str = rstr.substr(pos1 + 1, pos2 - pos1 - 1);
	return get_val_int(lstate, index_str).ival;
}

void init_func_def(string lstr, int line)
{
	cerr << "init_func_def: " << lstr << " " << line << endl;
	//@func %i_1 ( %ai_2 , %i_3 , %i_4 )
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

Value get_val_int(Local_State lstate, string rstr)
{
	cerr << "get_val_int: " << rstr << endl;
	Value val;
	if(rstr[0] == '%')
	{
		if(rstr[1] == 'a')
		{
			assert(rstr[2] == 'i');
			string array_name = get_array_name(rstr);
			int    index      = get_array_index(lstate, rstr);
			if(gtable[array_name].val_type == IT_NONE)
			{
				assert(lstate.ltable[array_name].val_type == IT_ARRAY_INT);
				val = lstate.ltable[array_name].val.aval[index];
			}
			else
			{
				assert(gtable[array_name].val_type == IT_ARRAY_INT);
				val = gtable[array_name].val.aval[index];
			}
		}
		else
		{
			assert(rstr[1] == 'i');
			if(gtable[rstr].val_type == IT_NONE)
			{
				assert(lstate.ltable[rstr].val_type == IT_INT);
				val = lstate.ltable[rstr].val;
			}
			else
			{
				assert(gtable[rstr].val_type == IT_INT);
				val = gtable[rstr].val;
			}
		}
	}
	else
	{
		val.ival = atoi(rstr.c_str());
	}
	cerr << "get_val_int: " << val.ival << endl;
	return val;
}

Value get_val_float(Local_State lstate, string rstr)
{
	cerr << "get_val_float: " << rstr << endl;
	Value val;
	if(rstr[0] == '%')
	{
		if(rstr[1] == 'a')
		{
			assert(rstr[2] == 'f');
			string array_name = get_array_name(rstr);
			int    index      = get_array_index(lstate, rstr);
			if(gtable[array_name].val_type == IT_NONE)
			{
				assert(lstate.ltable[array_name].val_type == IT_ARRAY_FLOAT);
				val = lstate.ltable[array_name].val.aval[index];
			}
			else
			{
				assert(gtable[array_name].val_type == IT_ARRAY_FLOAT);
				val = gtable[array_name].val.aval[index];
			}
		}
		else
		{
			assert(rstr[1] == 'f');
			if(gtable[rstr].val_type == IT_NONE)
			{
				assert(lstate.ltable[rstr].val_type == IT_FLOAT);
				val = lstate.ltable[rstr].val;
			}
			else
			{
				assert(gtable[rstr].val_type == IT_FLOAT);
				val = gtable[rstr].val;
			}
		}
	}
	else
	{
		val.fval = atof(rstr.c_str());
	}
	cerr << "get_val_float: " << val.fval << endl;
	return val;
}

Value get_val_double(Local_State lstate, string rstr)
{
	cerr << "get_val_double: " << rstr << endl;
	Value val;
	if(rstr[0] == '%')
	{
		if(rstr[1] == 'a')
		{
			assert(rstr[2] == 'd');
			string array_name = get_array_name(rstr);
			int    index      = get_array_index(lstate, rstr);
			if(gtable[array_name].val_type == IT_NONE)
			{
				assert(lstate.ltable[array_name].val_type == IT_ARRAY_DOUBLE);
				val = lstate.ltable[array_name].val.aval[index];
			}
			else
			{
				assert(gtable[array_name].val_type == IT_ARRAY_DOUBLE);
				val = gtable[array_name].val.aval[index];
			}
		}
		else
		{
			assert(rstr[1] == 'd');
			if(gtable[rstr].val_type == IT_NONE)
			{
				assert(lstate.ltable[rstr].val_type == IT_DOUBLE);
				val = lstate.ltable[rstr].val;
			}
			else
			{
				assert(gtable[rstr].val_type == IT_DOUBLE);
				val = gtable[rstr].val;
			}
		}
	}
	else
	{
		val.dval = atof(rstr.c_str());
	}
	cerr << "get_val_double: " << val.dval << endl;
	return val;
}

Value get_val_bool(Local_State lstate, string rstr)
{
	cerr << "get_val_bool: " << rstr << endl;
	Value val;
	if(rstr[0] == '%')
	{
		if(rstr[1] == 'a')
		{
			assert(rstr[2] == 'b');
			string array_name = get_array_name(rstr);
			int    index      = get_array_index(lstate, rstr);
			if(gtable[array_name].val_type == IT_NONE)
			{
				assert(lstate.ltable[array_name].val_type == IT_ARRAY_BOOL);
				val = lstate.ltable[array_name].val.aval[index];
			}
			else
			{
				assert(gtable[array_name].val_type == IT_ARRAY_BOOL);
				val = gtable[array_name].val.aval[index];
			}
		}
		else
		{
			assert(rstr[1] == 'b');
			if(gtable[rstr].val_type == IT_NONE)
			{
				assert(lstate.ltable[rstr].val_type == IT_BOOL);
				val = lstate.ltable[rstr].val;
			}
			else
			{
				assert(gtable[rstr].val_type == IT_BOOL);
				val = gtable[rstr].val;
			}
		}
	}
	else
	{
		val.bval = (rstr == "true");
	}
	cerr << "get_val_bool: " << val.bval << endl;
	return val;
}

Value get_val_array(Local_State lstate, string rstr)
{
	assert(rstr[0] == '%');
	assert(rstr[1] == 'a');
	cerr << "get_val_array: " << rstr << endl;
	if(gtable[rstr].val_type == IT_NONE)
	{
		assert(lstate.ltable[rstr].val_type == IT_ARRAY_INT ||
		       lstate.ltable[rstr].val_type == IT_ARRAY_FLOAT ||
		       lstate.ltable[rstr].val_type == IT_ARRAY_DOUBLE ||
		       lstate.ltable[rstr].val_type == IT_ARRAY_BOOL);
		cerr << "get_val_array: " << lstate.ltable[rstr].val.aval << endl;
		return lstate.ltable[rstr].val;
	}
	else
	{
		assert(gtable[rstr].val_type == IT_ARRAY_INT ||
		       gtable[rstr].val_type == IT_ARRAY_FLOAT ||
		       gtable[rstr].val_type == IT_ARRAY_DOUBLE ||
		       gtable[rstr].val_type == IT_ARRAY_BOOL);
		cerr << "get_val_array: " << gtable[rstr].val.aval << endl;
		return gtable[rstr].val;
	}
}

Value get_val(IdentType ident_type, Local_State lstate, string rstr)
{
	switch(ident_type)
	{
	case IT_INT:
		return get_val_int(lstate, rstr);
	case IT_FLOAT:
		return get_val_float(lstate, rstr);
	case IT_DOUBLE:
		return get_val_double(lstate, rstr);
	case IT_BOOL:
		return get_val_bool(lstate, rstr);
	case IT_ARRAY_INT:
	case IT_ARRAY_FLOAT:
	case IT_ARRAY_DOUBLE:
	case IT_ARRAY_BOOL:
		return get_val_array(lstate, rstr);
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
	assert(tokens[1] == "@array");
	assert(tokens[2][0] == '%');
	assert(tokens[2][1] == 'a');
	string name = tokens[2];
	assert(gtable[name].val_type == IT_NONE);
	int size              = get_val_int(g_lstate, tokens[3]).ival;
	gtable[name].name     = name;
	gtable[name].val_type = get_var_type_by_prefix(tokens[2]);
	gtable[name].val.aval = (Value *)malloc(sizeof(Value) * size);
}

void init_g_var(string lstr)
{
	cerr << "init_g_var: " << lstr << endl;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "!global");
	assert(tokens[1] == "@var");
	assert(tokens[2][0] == '%');
	string name = tokens[2];
	assert(gtable[name].val_type == IT_NONE);
	IdentType vtype       = get_var_type_by_prefix(tokens[2]);
	gtable[name].name     = name;
	gtable[name].val_type = vtype;
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
	lstate.ltable[name].val.aval = (Value *)malloc(sizeof(Value) * size);
}

void init_var(Local_State &lstate, string lstr)
{
	cerr << "init_var: " << lstr << endl;
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "@var");
	assert(tokens[1][0] == '%');
	string name = tokens[1];
	// assert(lstate.ltable[name].val_type == IT_NONE);
	assert(gtable[name].val_type == IT_NONE);
	IdentType vtype              = get_var_type_by_prefix(tokens[1]);
	lstate.ltable[name].name     = name;
	lstate.ltable[name].val_type = vtype;
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

void print_val(Value val, IdentType vtype)
{
	switch(vtype)
	{
	case IT_INT:
		cerr << val.ival;
		break;
	case IT_FLOAT:
		cerr << val.fval;
		break;
	case IT_DOUBLE:
		cerr << val.dval;
		break;
	case IT_BOOL:
		cerr << (val.bval ? "true" : "false");
		break;
	case IT_ARRAY_INT:
	case IT_ARRAY_FLOAT:
	case IT_ARRAY_DOUBLE:
	case IT_ARRAY_BOOL:
		cerr << val.aval;
		break;
	default:
		assert(false);
	}
}

Value func_call(string func_name, vector<Value> args)
{
	cerr << "func_call: " << func_name << endl;
	assert(gtable[func_name].val_type == IT_FUNC_INT ||
	       gtable[func_name].val_type == IT_FUNC_FLOAT ||
	       gtable[func_name].val_type == IT_FUNC_DOUBLE ||
	       gtable[func_name].val_type == IT_FUNC_BOOL ||
	       gtable[func_name].val_type == IT_FUNC_VOID);
	assert(args.size() == func_params[func_name].size());

	Local_State lstate = func_init_label(func_name);
	for(int i = 0; i < args.size(); i++)
	{
		init_arg(lstate, func_params[func_name][i], args[i]);
	}
	lstate.curline = gtable[func_name].val.line;
	lstate.type    = gtable[func_name].val_type;
	Value ret;
	while(!func_exec(lstate, ret))
		;
	cerr << "func_ret: ";
	print_val(ret, vtype_func_to_var(gtable[func_name].val_type));
	cerr << endl;
	return ret;
}

bool func_exec(Local_State &lstate, Value &ret)
{
	if(lstate.curline < 0)
	{
		io_exec(lstate, ret);
		return true;
	}
	assert(lstate.curline < lstrs.size());
	string lstr = lstrs[lstate.curline];
	cerr << "func_exec: " << lstr << endl;
	switch(get_op_type(lstr))
	{
	case OP_EMPTY:
	case OP_LABEL:
	case OP_FUNC_DEF:
		lstate.curline++;
		break;
	case OP_RETURN:
		ret = op_return(lstate, lstr);
		return true;
	case OP_ASSIGN:
		init_assign(lstate, lstr);
		break;
	case OP_ARRAY:
		init_array(lstate, lstr);
		break;
	case OP_VAR:
		init_var(lstate, lstr);
		break;
	case OP_AND:
		op_and(lstate, lstr);
		break;
	case OP_OR:
		op_or(lstate, lstr);
		break;
	case OP_NOT:
		op_not(lstate, lstr);
		break;
	case OP_ADD:
		op_add(lstate, lstr);
		break;
	case OP_ADDR:
		op_addr(lstate, lstr);
		break;
	case OP_SUB:
		op_sub(lstate, lstr);
		break;
	case OP_MUL:
		op_mul(lstate, lstr);
		break;
	case OP_DIV:
		op_div(lstate, lstr);
		break;
	case OP_REM:
		op_rem(lstate, lstr);
		break;
	case OP_EQ:
		op_eq(lstate, lstr);
		break;
	case OP_NE:
		op_ne(lstate, lstr);
		break;
	case OP_LT:
		op_lt(lstate, lstr);
		break;
	case OP_LE:
		op_le(lstate, lstr);
		break;
	case OP_GT:
		op_gt(lstate, lstr);
		break;
	case OP_GE:
		op_ge(lstate, lstr);
		break;
	case OP_POS:
		init_assign(lstate, lstr);
		break;
	case OP_NEG:
		op_neg(lstate, lstr);
		break;
	case OP_BRANCH:
		op_branch(lstate, lstr);
		break;
	case OP_FUNC_CALL:
		op_func_call(lstate, lstr);
		break;
	case OP_RETIRE:
		op_retire(lstate, lstr);
		break;
	default:
		assert(false);
	}
	return false;
}

void op_and(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "DAnd");
	bool  src1 = get_val_bool(lstate, tokens[2]).bval;
	bool  src2 = get_val_bool(lstate, tokens[3]).bval;
	Value val;
	val.bval = src1 && src2;
	local_assign(lstate, tokens[1], val, IT_BOOL);
}

void op_or(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "DOr");
	bool  src1 = get_val_bool(lstate, tokens[2]).bval;
	bool  src2 = get_val_bool(lstate, tokens[3]).bval;
	Value val;
	val.bval = src1 || src2;
	local_assign(lstate, tokens[1], val, IT_BOOL);
}

void op_not(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "Not");
	bool  src1 = get_val_bool(lstate, tokens[2]).bval;
	Value val;
	val.bval = !src1;
	local_assign(lstate, tokens[1], val, IT_BOOL);
}

void op_addr(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "Addr");
	Value *src1 = get_val_array(lstate, tokens[2]).aval;
	int    src2 = get_val_int(lstate, tokens[3]).ival;
	Value  val;
	val.aval = src1 + src2;
	local_assign(lstate, tokens[1], val, get_var_type_by_prefix(tokens[1]));
}

void op_branch(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "branch");
	bool cond = get_val_bool(lstate, tokens[2]).bval;
	if(cond)
	{
		assert(lstate.ltable[tokens[1]].val_type == IT_LABEL);
		lstate.curline = lstate.ltable[tokens[1]].val.line;
	}
}

void op_func_call(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "call");
	string func_name = tokens[2];
	assert(gtable[func_name].val_type == IT_FUNC_INT ||
	       gtable[func_name].val_type == IT_FUNC_FLOAT ||
	       gtable[func_name].val_type == IT_FUNC_DOUBLE ||
	       gtable[func_name].val_type == IT_FUNC_VOID ||
	       gtable[func_name].val_type == IT_FUNC_BOOL);
	vector<Value> args;
	for(int i = 4; i < tokens.size(); i++)
	{
		if(tokens[i] == "(" || tokens[i] == "," || tokens[i] == ")")
		{
			continue;
		}
		assert(args.size() < func_params[func_name].size());
		cerr << "parse arg:" << tokens[i]
		     << " to param:" << func_params[func_name][args.size()] << endl;
		switch(get_var_type_by_prefix(func_params[func_name][args.size()]))
		{
		case IT_INT:
			args.push_back(get_val_int(lstate, tokens[i]));
			break;
		case IT_FLOAT:
			args.push_back(get_val_float(lstate, tokens[i]));
			break;
		case IT_DOUBLE:
			args.push_back(get_val_double(lstate, tokens[i]));
			break;
		case IT_BOOL:
			args.push_back(get_val_bool(lstate, tokens[i]));
			break;
		case IT_ARRAY_BOOL:
		case IT_ARRAY_INT:
		case IT_ARRAY_FLOAT:
		case IT_ARRAY_DOUBLE:
			args.push_back(get_val_array(lstate, tokens[i]));
			break;
		default:
			assert(false);
		}
	}
	assert(args.size() == func_params[func_name].size());
	Value ret = func_call(func_name, args);
	switch(get_func_type_by_prefix(func_name))
	{
	case IT_FUNC_INT:
		local_assign(lstate, tokens[1], ret, IT_INT);
		break;
	case IT_FUNC_FLOAT:
		local_assign(lstate, tokens[1], ret, IT_FLOAT);
		break;
	case IT_FUNC_DOUBLE:
		local_assign(lstate, tokens[1], ret, IT_DOUBLE);
		break;
	case IT_FUNC_VOID:
		break;
	case IT_FUNC_BOOL:
		local_assign(lstate, tokens[1], ret, IT_BOOL);
		break;
	default:
		assert(false);
	}
}

Value op_return(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "return");
	Value ret;
	if(tokens.size() == 1)
	{
		return ret;
	}
	switch(vtype_func_to_var(lstate.type))
	{
	case IT_INT:
		ret = get_val_int(lstate, tokens[1]);
		break;
	case IT_FLOAT:
		ret = get_val_float(lstate, tokens[1]);
		break;
	case IT_DOUBLE:
		ret = get_val_double(lstate, tokens[1]);
		break;
	case IT_BOOL:
		ret = get_val_bool(lstate, tokens[1]);
		break;
	default:
		assert(false);
	}
	return ret;
}

void op_retire(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "retire");
	assert(gtable[tokens[1]].val_type == IT_NONE);
	assert(lstate.ltable[tokens[1]].val_type != IT_NONE);
	lstate.ltable[tokens[1]].val_type = IT_NONE;
}

void op_rem(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "Rem");
	int   src1 = get_val_int(lstate, tokens[2]).ival;
	int   src2 = get_val_int(lstate, tokens[3]).ival;
	Value val;
	val.ival = src1 % src2;
	local_assign(lstate, tokens[1], val, IT_INT);
}


void op_add(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "Add");
	switch(tokens[1][0])
	{
	case 'i':
		op_addi(lstate, lstr);
		break;
	case 'f':
		op_addf(lstate, lstr);
		break;
	case 'd':
		op_addd(lstate, lstr);
		break;
	default:
		assert(false);
	}
}

void op_addi(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	int            src1   = get_val_int(lstate, tokens[3]).ival;
	int            src2   = get_val_int(lstate, tokens[4]).ival;
	Value          val;
	val.ival = src1 + src2;
	local_assign(lstate, tokens[2], val, IT_INT);
}

void op_addf(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	float          src1   = get_val_float(lstate, tokens[3]).fval;
	float          src2   = get_val_float(lstate, tokens[4]).fval;
	Value          val;
	val.fval = src1 + src2;
	local_assign(lstate, tokens[2], val, IT_FLOAT);
}

void op_addd(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	double         src1   = get_val_double(lstate, tokens[3]).dval;
	double         src2   = get_val_double(lstate, tokens[4]).dval;
	Value          val;
	val.dval = src1 + src2;
	local_assign(lstate, tokens[2], val, IT_DOUBLE);
}

void op_sub(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "Sub");
	switch(tokens[1][0])
	{
	case 'i':
		op_subi(lstate, lstr);
		break;
	case 'f':
		op_subf(lstate, lstr);
		break;
	case 'd':
		op_subd(lstate, lstr);
		break;
	default:
		assert(false);
	}
}

void op_subi(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	int            src1   = get_val_int(lstate, tokens[3]).ival;
	int            src2   = get_val_int(lstate, tokens[4]).ival;
	Value          val;
	val.ival = src1 - src2;
	local_assign(lstate, tokens[2], val, IT_INT);
}

void op_subf(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	float          src1   = get_val_float(lstate, tokens[3]).fval;
	float          src2   = get_val_float(lstate, tokens[4]).fval;
	Value          val;
	val.fval = src1 - src2;
	local_assign(lstate, tokens[2], val, IT_FLOAT);
}

void op_subd(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	double         src1   = get_val_double(lstate, tokens[3]).dval;
	double         src2   = get_val_double(lstate, tokens[4]).dval;
	Value          val;
	val.dval = src1 - src2;
	local_assign(lstate, tokens[2], val, IT_DOUBLE);
}

void op_mul(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "Mul");
	switch(tokens[1][0])
	{
	case 'i':
		op_muli(lstate, lstr);
		break;
	case 'f':
		op_mulf(lstate, lstr);
		break;
	case 'd':
		op_muld(lstate, lstr);
		break;
	default:
		assert(false);
	}
}

void op_muli(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	int            src1   = get_val_int(lstate, tokens[3]).ival;
	int            src2   = get_val_int(lstate, tokens[4]).ival;
	Value          val;
	val.ival = src1 * src2;
	local_assign(lstate, tokens[2], val, IT_INT);
}

void op_mulf(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	float          src1   = get_val_float(lstate, tokens[3]).fval;
	float          src2   = get_val_float(lstate, tokens[4]).fval;
	Value          val;
	val.fval = src1 * src2;
	local_assign(lstate, tokens[2], val, IT_FLOAT);
}

void op_muld(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	double         src1   = get_val_double(lstate, tokens[3]).dval;
	double         src2   = get_val_double(lstate, tokens[4]).dval;
	Value          val;
	val.dval = src1 * src2;
	local_assign(lstate, tokens[2], val, IT_DOUBLE);
}

void op_div(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "Div");
	switch(tokens[1][0])
	{
	case 'i':
		op_divi(lstate, lstr);
		break;
	case 'f':
		op_divf(lstate, lstr);
		break;
	case 'd':
		op_divd(lstate, lstr);
		break;
	default:
		assert(false);
	}
}

void op_divi(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	int            src1   = get_val_int(lstate, tokens[3]).ival;
	int            src2   = get_val_int(lstate, tokens[4]).ival;
	Value          val;
	val.ival = src1 / src2;
	local_assign(lstate, tokens[2], val, IT_INT);
}

void op_divf(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	float          src1   = get_val_float(lstate, tokens[3]).fval;
	float          src2   = get_val_float(lstate, tokens[4]).fval;
	Value          val;
	val.fval = src1 / src2;
	local_assign(lstate, tokens[2], val, IT_FLOAT);
}

void op_divd(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	double         src1   = get_val_double(lstate, tokens[3]).dval;
	double         src2   = get_val_double(lstate, tokens[4]).dval;
	Value          val;
	val.dval = src1 / src2;
	local_assign(lstate, tokens[2], val, IT_DOUBLE);
}


void op_neg(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "Neg");
	switch(tokens[1][0])
	{
	case 'i':
		op_negi(lstate, lstr);
		break;
	case 'f':
		op_negf(lstate, lstr);
		break;
	case 'd':
		op_negd(lstate, lstr);
		break;
	default:
		assert(false);
	}
}

void op_negi(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	int            src1   = get_val_int(lstate, tokens[3]).ival;
	Value          val;
	val.ival = -src1;
	local_assign(lstate, tokens[2], val, IT_INT);
}

void op_negf(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	float          src1   = get_val_float(lstate, tokens[3]).fval;
	Value          val;
	val.fval = -src1;
	local_assign(lstate, tokens[2], val, IT_FLOAT);
}

void op_negd(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	double         src1   = get_val_double(lstate, tokens[3]).dval;
	Value          val;
	val.dval = -src1;
	local_assign(lstate, tokens[2], val, IT_DOUBLE);
}

void op_eq(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "EQ");
	switch(tokens[1][0])
	{
	case 'i':
		op_eqi(lstate, lstr);
		break;
	case 'f':
		op_eqf(lstate, lstr);
		break;
	case 'd':
		op_eqd(lstate, lstr);
		break;
	case 'b':
		op_eqb(lstate, lstr);
		break;
	default:
		assert(false);
	}
}

void op_eqi(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	int            src1   = get_val_int(lstate, tokens[3]).ival;
	int            src2   = get_val_int(lstate, tokens[4]).ival;
	Value          val;
	val.ival = src1 == src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_eqf(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	float          src1   = get_val_float(lstate, tokens[3]).fval;
	float          src2   = get_val_float(lstate, tokens[4]).fval;
	Value          val;
	val.ival = src1 == src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_eqd(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	double         src1   = get_val_double(lstate, tokens[3]).dval;
	double         src2   = get_val_double(lstate, tokens[4]).dval;
	Value          val;
	val.ival = src1 == src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_eqb(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	bool           src1   = get_val_bool(lstate, tokens[3]).bval;
	bool           src2   = get_val_bool(lstate, tokens[4]).bval;
	Value          val;
	val.ival = src1 == src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_ne(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "NE");
	switch(tokens[1][0])
	{
	case 'i':
		op_nei(lstate, lstr);
		break;
	case 'f':
		op_nef(lstate, lstr);
		break;
	case 'd':
		op_ned(lstate, lstr);
		break;
	case 'b':
		op_neb(lstate, lstr);
		break;
	default:
		assert(false);
	}
}

void op_nei(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	int            src1   = get_val_int(lstate, tokens[3]).ival;
	int            src2   = get_val_int(lstate, tokens[4]).ival;
	Value          val;
	val.ival = src1 != src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_nef(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	float          src1   = get_val_float(lstate, tokens[3]).fval;
	float          src2   = get_val_float(lstate, tokens[4]).fval;
	Value          val;
	val.ival = src1 != src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_ned(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	double         src1   = get_val_double(lstate, tokens[3]).dval;
	double         src2   = get_val_double(lstate, tokens[4]).dval;
	Value          val;
	val.ival = src1 != src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_neb(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	bool           src1   = get_val_bool(lstate, tokens[3]).bval;
	bool           src2   = get_val_bool(lstate, tokens[4]).bval;
	Value          val;
	val.ival = src1 != src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_lt(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "LT");
	switch(tokens[1][0])
	{
	case 'i':
		op_lti(lstate, lstr);
		break;
	case 'f':
		op_ltf(lstate, lstr);
		break;
	case 'd':
		op_ltd(lstate, lstr);
		break;
	default:
		assert(false);
	}
}

void op_lti(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	int            src1   = get_val_int(lstate, tokens[3]).ival;
	int            src2   = get_val_int(lstate, tokens[4]).ival;
	Value          val;
	val.ival = src1 < src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_ltf(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	float          src1   = get_val_float(lstate, tokens[3]).fval;
	float          src2   = get_val_float(lstate, tokens[4]).fval;
	Value          val;
	val.ival = src1 < src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_ltd(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	double         src1   = get_val_double(lstate, tokens[3]).dval;
	double         src2   = get_val_double(lstate, tokens[4]).dval;
	Value          val;
	val.ival = src1 < src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_le(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "LE");
	switch(tokens[1][0])
	{
	case 'i':
		op_lei(lstate, lstr);
		break;
	case 'f':
		op_lef(lstate, lstr);
		break;
	case 'd':
		op_led(lstate, lstr);
		break;
	default:
		assert(false);
	}
}

void op_lei(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	int            src1   = get_val_int(lstate, tokens[3]).ival;
	int            src2   = get_val_int(lstate, tokens[4]).ival;
	Value          val;
	val.ival = src1 <= src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_lef(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	float          src1   = get_val_float(lstate, tokens[3]).fval;
	float          src2   = get_val_float(lstate, tokens[4]).fval;
	Value          val;
	val.ival = src1 <= src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_led(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	double         src1   = get_val_double(lstate, tokens[3]).dval;
	double         src2   = get_val_double(lstate, tokens[4]).dval;
	Value          val;
	val.ival = src1 <= src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_gt(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "GT");
	switch(tokens[1][0])
	{
	case 'i':
		op_gti(lstate, lstr);
		break;
	case 'f':
		op_gtf(lstate, lstr);
		break;
	case 'd':
		op_gtd(lstate, lstr);
		break;
	default:
		assert(false);
	}
}

void op_gti(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	int            src1   = get_val_int(lstate, tokens[3]).ival;
	int            src2   = get_val_int(lstate, tokens[4]).ival;
	Value          val;
	val.ival = src1 > src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_gtf(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	float          src1   = get_val_float(lstate, tokens[3]).fval;
	float          src2   = get_val_float(lstate, tokens[4]).fval;
	Value          val;
	val.ival = src1 > src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_gtd(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	double         src1   = get_val_double(lstate, tokens[3]).dval;
	double         src2   = get_val_double(lstate, tokens[4]).dval;
	Value          val;
	val.ival = src1 > src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_ge(Local_State &lstate, string lstr)
{
	lstate.curline++;
	vector<string> tokens = splitString(lstr);
	assert(tokens[0] == "GE");
	switch(tokens[1][0])
	{
	case 'i':
		op_gei(lstate, lstr);
		break;
	case 'f':
		op_gef(lstate, lstr);
		break;
	case 'd':
		op_ged(lstate, lstr);
		break;
	default:
		assert(false);
	}
}

void op_gei(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	int            src1   = get_val_int(lstate, tokens[3]).ival;
	int            src2   = get_val_int(lstate, tokens[4]).ival;
	Value          val;
	val.ival = src1 >= src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_gef(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	float          src1   = get_val_float(lstate, tokens[3]).fval;
	float          src2   = get_val_float(lstate, tokens[4]).fval;
	Value          val;
	val.ival = src1 >= src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}

void op_ged(Local_State &lstate, string lstr)
{
	vector<string> tokens = splitString(lstr);
	double         src1   = get_val_double(lstate, tokens[3]).dval;
	double         src2   = get_val_double(lstate, tokens[4]).dval;
	Value          val;
	val.ival = src1 >= src2;
	local_assign(lstate, tokens[2], val, IT_BOOL);
}