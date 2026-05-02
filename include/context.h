#ifndef CACT_CONTEXT_H
#define CACT_CONTEXT_H

#include <iostream>
#include <string>
#include <vector>

#include "btype.h"
#include "func_table.h"
#include "sym_table.h"

// Global compiler output stream
inline std::ostream* global_out = &std::cout;

// IR name generation
extern int              g_count;
extern std::string      btype_string[];

// Symbol and function tables
extern SymTree          g_symtree;
extern FuncTable        g_functable;

// Built-in special functions
extern std::string                      special_funcname[];
extern Btype                            special_funcType[];
extern std::vector<FuncParamsType>      special_funcParams[];

// Compiler flags
extern int              merge_constant;
extern int              optimize_level;

// IR and ASM line buffers
extern std::vector<std::string> ir_lines;
extern std::vector<std::string> asm_lines;

#endif // CACT_CONTEXT_H
