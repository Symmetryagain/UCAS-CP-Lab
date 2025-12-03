#include "SymTable.h"

extern int g_count;
extern std::string btype_string[];

std::string Symbol::name_in_IR() { return _Name_in_IR; }
Btype Symbol::bType() { return _b_type; }
bool Symbol::isArray() { return _is_array; }
bool Symbol::isConst() { return _is_const; }
std::vector<size_t> Symbol::arraySize() { return _Array_Size; }

bool SymTable::checkLocal(const std::string &name) {
  return table.count(name) ? true : false;
}

std::optional<Symbol> SymTable::resolveLocal(const std::string &name) {
  auto it = table.find(name);
  if (it == table.end()) 
    return std::nullopt;
  else 
    return it->second;
}

bool SymTable::defineLocal(const std::string name, const Symbol &sym) {
  if (table.count(name)) {
    // 
    return false;
  }
  table[name] = sym;
  return true;
}

void SymTree::enterScope() {
  _sym_stack.push_back(SymTable());
}

void SymTree::leaveScope() {
  _sym_stack.pop_back();
}

bool SymTree::check(const std::string &name) {
  return _sym_stack.back().checkLocal(name);
}

std::optional<Symbol> SymTree::resolve(const std::string &name) {
  for (int i = _sym_stack.size() - 1; i >= 0; --i) {
    auto result = _sym_stack[i].resolveLocal(name);
    if (result.has_value()) {
      return result;
    }
  }
  return std::nullopt;
}

bool SymTree::define(
  Btype bType, 
  std::string &name,
  bool isArray,
  bool isConst,
  std::vector<size_t> arraySize
) {
  auto &sym_top = _sym_stack.back();
  if (sym_top.checkLocal(name)) {
    // 
    return false;
  }
  ++g_count;
  std::string name_in_IR = btype_string[bType] + "_" + std::to_string(g_count); // (int abc_123) -> i_7
  if (isArray) name_in_IR = "a" + name_in_IR; // double a[3] -> ad_6
  Symbol sym(
    name_in_IR,
    bType,
    isArray,
    isConst,
    arraySize
  );
  if (!sym_top.defineLocal(name, sym)) return false;
  name = name_in_IR;
  return true;
}
