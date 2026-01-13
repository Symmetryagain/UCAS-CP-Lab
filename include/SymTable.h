#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include <bits/stdc++.h>
#include "btype.h"

class Symbol {
private:
  std::string _Name_in_IR;
  Btype _b_type;
  bool _is_array;
  bool _is_const;
  std::vector<size_t> _Array_Size;
  std::variant<int, float, double, bool> _value;
public:
  Symbol() {}
  explicit Symbol(
    std::string _nameInIR, 
    Btype _bType, 
    bool _isArray,
    bool _isConst,
    std::vector<size_t> _arraySize
  ) :_Name_in_IR(_nameInIR), _b_type(_bType), _is_array(_isArray), _is_const(_isConst), _Array_Size(_arraySize) {}

  std::string name_in_IR();
  Btype bType();
  bool isArray();
  bool isConst();
  std::vector<size_t> arraySize();
  std::variant<int, float, double, bool> value();
  void modify_value(std::variant<int, float, double, bool> value);
};

class SymTable {
private:
  std::unordered_map<std::string, Symbol> table;
public:
  bool checkLocal(const std::string &name);
  std::optional<Symbol> resolveLocal(const std::string &name);
  bool defineLocal(const std::string name, const Symbol &sym);
  void modify_value(
    std::string name, 
    std::variant<int, float, double, bool> value
  );
};

class SymTree {
private:
  std::vector<SymTable> _sym_stack;
public:
  void enterScope();
  void leaveScope();
  bool check(const std::string &name);
  std::optional<Symbol> resolve(const std::string &name);
  bool define(
    Btype bType, 
    std::string &name,
    bool isArray,
    bool isConst,
    std::vector<size_t> arraySize
  );
  void modify_value(
    std::string name,
    std::variant<int, float, double, bool> value
  );
};

#endif // _SYMTABLE_H