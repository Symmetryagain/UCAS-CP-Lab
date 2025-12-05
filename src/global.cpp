#include "SymTable.h"
#include "FuncTable.h"
#include "tree/ParseTree.h"
#include "Analyzer.h"

int g_count = 0;
std::string btype_string[] = {"", "i", "f", "d", "b", "v", ""};
SymTree g_symtree;
FuncTable g_functable;
std::string special_funcname[] = {
  "print_int",
  "print_float",
  "print_double",
  "print_bool",
  "get_int",
  "get_float",
  "get_double",
  "main"
};
Btype special_funcType[] = {
  Void,
  Void,
  Void,
  Void,
  Int,
  Float,
  Double
};
std::vector<FuncParamsType> special_funcParams[] = {
  {FuncParamsType(Int, false)},
  {FuncParamsType(Float, false)},
  {FuncParamsType(Double, false)},
  {FuncParamsType(Bool, false)},
  {}, {}, {}
};