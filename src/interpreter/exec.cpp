#include "interpreter.h"

extern Table                       gtable;
extern vector<Local_State>         lstates;
extern vector<string>              lstrs;
extern map<string, vector<string>> func_params;

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
