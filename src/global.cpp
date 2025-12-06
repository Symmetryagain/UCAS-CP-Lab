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

Btype str_to_type(const std::string &s) {
  if (s == "int") return Int;
  else if (s == "float") return Float;
  else if (s == "double") return Double;
  else if (s == "bool") return Bool;
  else if (s == "void") return Void;
  else {
    // 
    assert(0);
    return Void;
  }
}

int parse_int(const std::string &s) {
  return std::stoi(s, nullptr, 0);
}

std::string createLabel() {
  ++g_count;
  return "l_" + std::to_string(g_count);
}

std::string createVar(const Btype &b, bool is_array) {
  ++g_count;
  if (!is_array) return btype_string[b] + "_" + std::to_string(g_count);
  else return "a" + btype_string[b] + "_" + std::to_string(g_count);
}