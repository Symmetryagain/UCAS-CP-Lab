#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "utils.h"

inline std::ostream* global_out = &std::cout;

extern int g_count;
extern std::string btype_string[];
extern SymTree g_symtree;
extern FuncTable g_functable;
extern std::string special_funcname[];
extern Btype special_funcType[];
extern std::vector<FuncParamsType> special_funcParams[];
extern int merge_constant;

#define NPRE "%"
#define GLOBAL "!global"
#define VAR "@var"
#define ARRAY "@array"
#define FUNC "@func"
#define ENDFUNC "@endfunc"
#define CALL "call"
#define VOID "void"
#define LABEL "label"
#define RETURN "return"
#define ASSIGN "assign"
#define NEG "Neg"
#define NOT "Not"
#define BRANCH "branch"
#define ADD "Add"
#define SUB "Sub"
#define MUL "Mul"
#define DIV "Div"
#define REM "Rem"
#define GE "GE"
#define GT "GT"
#define LE "LE"
#define LT "LT"
#define EQ "EQ"
#define NE "NE"
#define DAND "DAnd"
#define DOR "DOr"
#define ADDR "Addr"

#endif // _GLOBAL_H