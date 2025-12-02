#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <bits/stdc++.h>
using namespace std;

vector<string> splitString(string const &str);

enum IdentType
{
	IT_NONE,
	IT_LABEL,
	IT_INT,
	IT_FLOAT,
	IT_DOUBLE,
	IT_BOOL,
	IT_ARRAY_INT,
	IT_ARRAY_FLOAT,
	IT_ARRAY_DOUBLE,
	IT_ARRAY_BOOL,
	IT_FUNC_INT,
	IT_FUNC_FLOAT,
	IT_FUNC_DOUBLE,
	IT_FUNC_BOOL,
	IT_FUNC_VOID
};

enum OpType
{
	OP_EMPTY,
	OP_FUNC_DEF,
	OP_FUNC_END,
	OP_FUNC_CALL,
	OP_G_VAR,
	OP_G_ASSIGN,
	OP_G_ARRAY,
	OP_VAR,
	OP_ASSIGN,
	OP_ARRAY,
	OP_AND,
	OP_OR,
	OP_MUL,
	OP_ADD,
	OP_ADDR,
	OP_SUB,
	OP_DIV,
	OP_REM,
	OP_LT,
	OP_GT,
	OP_LE,
	OP_GE,
	OP_EQ,
	OP_NE,
	OP_POS,
	OP_NEG,
	OP_NOT,
	OP_BRANCH,
	OP_LABEL,
	OP_RETURN,
	OP_RETIRE
};

union Value

{
	int    line;
	int    ival;
	float  fval;
	double dval;
	bool   bval;
	Value *aval;
};

struct Ident
{
	string    name;
	IdentType val_type;
	Value     val;

	Ident() : val_type(IT_NONE)
	{
	}
};

typedef map<string, Ident> Table;

extern Table gtable;

struct Local_State
{
	int curline;
	IdentType type;
	Table ltable;
};

Local_State const g_lstate = {0, IT_INT, Table()};

extern vector<string>              lstrs;
extern map<string, vector<string>> func_params;

void
    func_init(string name, int line, IdentType ret_type, vector<string> params);

OpType    get_op_type(string lstr);
IdentType vtype_var_to_array(IdentType vtype);
IdentType vtype_array_to_var(IdentType vtype);
IdentType vtype_var_to_func(IdentType vtype);
IdentType vtype_func_to_var(IdentType vtype);
IdentType get_func_type_by_prefix(string func_name);
IdentType get_var_type_by_prefix(string var_name);
IdentType get_final_var_type_by_prefix(string var_name);
string    get_array_name(string rstr);
int       get_array_index(Local_State lstate, string rstr);

void init_func_def(string lstr, int line);
void init_g_assign(string lstr);
void init_g_array(string lstr);
void init_g_var(string lstr);
void init_arg(Local_State &lstate, string name, Value val);

Value get_val(IdentType ident_type, Local_State lstate, string rstr);
Value get_val_int(Local_State lstate, string rstr);
Value get_val_float(Local_State lstate, string rstr);
Value get_val_double(Local_State lstate, string rstr);
Value get_val_bool(Local_State lstate, string rstr);
Value get_val_array(Local_State lstate, string rstr);

Local_State func_init_label(string func_name);
void        add_label(Local_State &lstate, string lstr, int line);
Value       func_call(string func_name, vector<Value> args);
bool        func_exec(Local_State &lstate, Value &val);

void print_val(Value val, IdentType vtype);

void init_assign(Local_State &lstate, string lstr);
void init_array(Local_State &lstate, string lstr);
void init_var(Local_State &lstate, string lstr);
void op_and(Local_State &lstate, string lstr);
void op_or(Local_State &lstate, string lstr);
void op_not(Local_State &lstate, string lstr);

void op_add(Local_State &lstate, string lstr);
void op_addi(Local_State &lstate, string lstr);
void op_addf(Local_State &lstate, string lstr);
void op_addd(Local_State &lstate, string lstr);
void op_addr(Local_State &lstate, string lstr);
void op_sub(Local_State &lstate, string lstr);
void op_subi(Local_State &lstate, string lstr);
void op_subf(Local_State &lstate, string lstr);
void op_subd(Local_State &lstate, string lstr);
void op_mul(Local_State &lstate, string lstr);
void op_muli(Local_State &lstate, string lstr);
void op_mulf(Local_State &lstate, string lstr);
void op_muld(Local_State &lstate, string lstr);
void op_div(Local_State &lstate, string lstr);
void op_divi(Local_State &lstate, string lstr);
void op_divf(Local_State &lstate, string lstr);
void op_divd(Local_State &lstate, string lstr);
void op_rem(Local_State &lstate, string lstr);
void op_remi(Local_State &lstate, string lstr);
void op_remf(Local_State &lstate, string lstr);
void op_remd(Local_State &lstate, string lstr);
void op_lt(Local_State &lstate, string lstr);
void op_lti(Local_State &lstate, string lstr);
void op_ltf(Local_State &lstate, string lstr);
void op_ltd(Local_State &lstate, string lstr);
void op_gt(Local_State &lstate, string lstr);
void op_gti(Local_State &lstate, string lstr);
void op_gtf(Local_State &lstate, string lstr);
void op_gtd(Local_State &lstate, string lstr);
void op_le(Local_State &lstate, string lstr);
void op_lei(Local_State &lstate, string lstr);
void op_lef(Local_State &lstate, string lstr);
void op_led(Local_State &lstate, string lstr);
void op_ge(Local_State &lstate, string lstr);
void op_gei(Local_State &lstate, string lstr);
void op_gef(Local_State &lstate, string lstr);
void op_ged(Local_State &lstate, string lstr);
void op_eq(Local_State &lstate, string lstr);
void op_eqi(Local_State &lstate, string lstr);
void op_eqf(Local_State &lstate, string lstr);
void op_eqd(Local_State &lstate, string lstr);
void op_eqb(Local_State &lstate, string lstr);
void op_ne(Local_State &lstate, string lstr);
void op_nei(Local_State &lstate, string lstr);
void op_nef(Local_State &lstate, string lstr);
void op_ned(Local_State &lstate, string lstr);
void op_neb(Local_State &lstate, string lstr);
void op_neg(Local_State &lstate, string lstr);
void op_negi(Local_State &lstate, string lstr);
void op_negf(Local_State &lstate, string lstr);
void op_negd(Local_State &lstate, string lstr);

void io_exec(Local_State &lstate, Value &val);

void  op_branch(Local_State &lstate, string lstr);
void  op_func_call(Local_State &lstate, string lstr);
void  op_retire(Local_State &lstate, string lstr);
Value op_return(Local_State &lstate, string lstr);

void local_assign(Local_State &lstate, string name, Value val, IdentType vtype);

void gtable_init();


#endif // INTERPRETER_H
