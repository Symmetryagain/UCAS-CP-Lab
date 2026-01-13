#ifndef ASMGEN_H
#define ASMGEN_H

#include "main.h"

#include <stddef.h>
#include <string>

using namespace std;

enum GType
{
	G_INT    = 0,
	G_BOOL   = 1,
	G_FLOAT  = 2,
	G_DOUBLE = 3,
};

constexpr size_t TypeSize[] = {4, 1, 4, 8, 8, 8, 8, 8, 8};

struct GlobalVar
{
	string   name;
	GType    gtype;
	uint64_t value;
	bool     is_bss;
	bool     is_const;
	bool     is_unused;
};

extern vector<GlobalVar>   gv_list;
extern map<string, size_t> gv_map;

struct GlobalArr
{
	string           name;
	GType            gtype;
	size_t           length;
	vector<uint64_t> values;
	bool             is_bss;
	bool             is_const;
	bool             is_unused;
};

extern vector<GlobalArr>   ga_list;
extern map<string, size_t> ga_map;

enum FType
{
	F_INT    = 0,
	F_BOOL   = 1,
	F_FLOAT  = 2,
	F_DOUBLE = 3,
	F_VOID   = 12
};

enum PType
{
	P_INT        = 0,
	P_BOOL       = 1,
	P_FLOAT      = 2,
	P_DOUBLE     = 3,
	P_PTR_INT    = 4,
	P_PTR_BOOL   = 5,
	P_PTR_FLOAT  = 6,
	P_PTR_DOUBLE = 7,
	P_REG        = 8,
};

struct Param
{
	string name;
	PType  ptype;
	size_t reg_pos;
};

struct LocalVar
{
	string name;
	PType  ptype;
	size_t sp_pos;
};

struct LocalArr
{
	string name;
	GType  gtype;
	size_t length;
	size_t sp_pos;
};

struct Func
{
	string              name;
	FType               ftype;
	size_t              gpr_a = 0;
	size_t              fpr_a = 0;
	vector<Param>       p_list;
	map<string, size_t> p_map;
	vector<LocalVar>    lv_list;
	map<string, size_t> lv_map;
	vector<LocalArr>    la_list;
	map<string, size_t> la_map;
	size_t              sp_size;
	vector<string>      ir_lines;
	vector<string>      asm_lines;
	bool                is_extern;
	bool                is_used;
};

extern vector<Func>        f_list;
extern map<string, size_t> f_map;


#endif // ASMGEN_H
