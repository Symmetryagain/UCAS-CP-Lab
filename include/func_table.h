#ifndef CACT_FUNCTABLE_H
#define CACT_FUNCTABLE_H

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "btype.h"

struct FuncParamsType {
  Btype bType;
  bool isArray;
  std::vector<size_t> arraySize;
  FuncParamsType() {}
  FuncParamsType(Btype _bType, bool _isArray, std::vector<size_t> _arraySize = {})
    :bType(_bType), isArray(_isArray), arraySize(_arraySize) {}
};

class Func {
private:
  std::string _name_in_IR;
  Btype _retvalType;
  std::vector<FuncParamsType> _params;
public:
  Func() {}
  explicit Func(
    std::string name,
    Btype retType,
    std::vector<FuncParamsType> params
  ) :_name_in_IR(name), _retvalType(retType), _params(params) {}
  std::string name_in_IR();
  Btype retvalType();
  std::vector<FuncParamsType> params();
};

class FuncTable {
private:
  std::unordered_map<std::string, Func> table;
public:
  bool check(std::string &_name);
  std::optional<Func> resolve(std::string &_name);
  bool define(Btype retType, std::string &name, const std::vector<FuncParamsType> &funcParams);
};

#endif // CACT_FUNCTABLE_H
