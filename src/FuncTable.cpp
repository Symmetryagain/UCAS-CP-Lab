#include "FuncTable.h"

extern int g_count;
extern std::string btype_string[];
extern std::string special_funcname[];

std::string Func::name_in_IR() { return _name_in_IR; }
std::vector<FuncParamsType> Func::params() { return _params; }
Btype Func::retvalType() { return _retvalType; }

bool FuncTable::check(std::string &_name) {
  return table.count(_name) ? true : false;
}

std::optional<Func> FuncTable::resolve(std::string &_name) {
  auto it = table.find(_name);
  if (it == table.end()) 
    return std::nullopt;
  else 
    return it->second;
}

bool FuncTable::define(
  Btype retType, 
  std::string &name, 
  const std::vector<FuncParamsType> &funcParams
) {
  if (table.count(name)) {
    // 
    return false;
  }
  std::string name_in_IR;
  bool flag = false;
  for (int i = 0; i < 8; ++i) 
    if (name == special_funcname[i]) {
      name_in_IR = name;
      flag = true;
      break;
    }
  if (!flag) {
    ++g_count;
    name_in_IR = btype_string[retType] + "_" + std::to_string(g_count);
  }
  Func func(name_in_IR, retType, funcParams);
  table[name] = func;
  name = name_in_IR;
  return true;
}