#include "Analyzer.h"
#include "FuncTable.h"
#include "SymTable.h"
#include "utils.h"

extern SymTree g_symtree;
extern FuncTable g_functable;
extern std::string special_funcname[];
extern std::string btype_string[];

std::any
Analyzer::visitTest(CACTParser::TestContext *context) { // will not reach here
  std::cerr << "Always continue; Never break;" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_1_ident(CACTParser::Expr_1_identContext *context) {
  std::cerr << "Enter Expr_1_ident" << std::endl;
  std::string varName = context->Ident()->getText();
  auto res = g_symtree.resolve(varName);
  if (!res.has_value()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  Symbol var = res.value();
  context->btype = var.bType();
  context->res = "%" + var.name_in_IR();
  if (var.isArray())
    context->array_size = var.arraySize();
  else
    context->array_size = {};
  std::cerr << "Leave Expr_1_ident" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitExpr_1_constant(CACTParser::Expr_1_constantContext *context) {
  std::cerr << "Enter Expr_1_constant" << std::endl;
  context->const_()->accept(this);
  context->btype = context->const_()->btype;
  context->res = context->const_()->value;
  context->array_size = {};
  std::cerr << "Leave Expr_1_constant" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitExpr_1_braket(CACTParser::Expr_1_braketContext *context) {
  std::cerr << "Enter Expr_1_braket" << std::endl;
  context->expr_8()->accept(this);
  context->btype = context->expr_8()->btype;
  context->res = context->expr_8()->res;
  context->array_size = context->expr_8()->array_size;
  std::cerr << "Leave Expr_1_braket" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitExpr_1_func_call(CACTParser::Expr_1_func_callContext *context) {
  std::cerr << "Enter Expr_1_func_call" << std::endl;
  context->func_call()->accept(this);
  context->btype = context->func_call()->btype;
  context->res = context->func_call()->res;
  context->array_size = {};
  std::cerr << "Leave Expr_1_func_call" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_1_array(CACTParser::Expr_1_arrayContext *context) {
  std::cerr << "Enter Expr_1_array" << std::endl;
  std::string arrName = context->Ident()->getText();
  auto res = g_symtree.resolve(arrName);
  if (!res.has_value()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  Symbol arr = res.value();
  if (!arr.isArray()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = arr.bType();
  std::vector<size_t> array_size(arr.arraySize().size());
  if (arr.arraySize().size() < context->expr_8().size()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  array_size.back() = 1;
  for (int i = (int)array_size.size() - 2; i >= 0; --i) {
    array_size[i] = array_size[i + 1] * arr.arraySize()[i + 1];
  }
  std::string offset = "%" + createVar(Int, false);
  std::string tmp = "%" + createVar(Int, false);
  std::string arr_name = "%" + arr.name_in_IR();
  std::cout << "@var " << offset << std::endl;
  std::cout << "@var " << tmp << std::endl;
  std::cout << "assign " << offset << " 0" << std::endl;
  for (int i = 0; i < (int)context->expr_8().size(); ++i) {
    context->expr_8()[i]->accept(this);
    if (context->expr_8()[i]->btype != Int ||
        !context->expr_8()[i]->array_size.empty()) {
      //
      exit(3);
      assert(0);
      return nullptr;
    }
    std::cout << "Mul i " << tmp << " " << context->expr_8()[i]->res
              << " " << array_size[i] << std::endl; // must be Int
    std::cout << "Add i " << offset << " " << offset << " " << tmp
              << std::endl; // must be Int
  }
  if (arr.arraySize().size() == context->expr_8().size()) { // not array
    context->array_size = {};
    std::string res = "%" + createVar(context->btype, false);
    std::cout << "@var " << res << std::endl;
    std::cout << "assign " << res << " " << arr_name << "[" << offset
              << "]" << std::endl;
    context->res = res;
  } else {
    for (int i = context->expr_8().size(); i < (int)arr.arraySize().size();
         ++i) {
      context->array_size.push_back(arr.arraySize()[i]);
    }
    std::string res = "%" + createVar(context->btype, true);
    std::cout << "@var " << res << std::endl;
    std::cout << "Addr " << res << " " << arr_name << " " << offset
              << std::endl;
    context->res = res;
  }
  std::cerr << "Leave Expr_1_array" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitNum_const_int(CACTParser::Num_const_intContext *context) {
  std::cerr << "Enter Num_const_int" << std::endl;
  context->btype = Int;
  context->value = context->intconst()->getText();
  std::cerr << "Leave Num_const_int" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitNum_const_float(CACTParser::Num_const_floatContext *context) {
  std::cerr << "Enter Num_const_float" << std::endl;
  context->btype = Float;
  context->value = context->FloatConst()->getText();
  std::cerr << "Leave Num_const_float" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitNum_const_double(CACTParser::Num_const_doubleContext *context) {
  std::cerr << "Enter Num_const_double" << std::endl;
  context->btype = Double;
  context->value = context->DoubleConst()->getText();
  std::cerr << "Leave Num_const_double" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitConst_num_const(CACTParser::Const_num_constContext *context) {
  std::cerr << "Enter Const_num_const" << std::endl;
  context->num_const()->accept(this);
  context->btype = context->num_const()->btype;
  context->value = context->num_const()->value;
  std::cerr << "Leave Const_num_const" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitConst_bool_const(CACTParser::Const_bool_constContext *context) {
  std::cerr << "Enter Const_bool_const" << std::endl;
  context->btype = Bool;
  context->value = context->boolconst()->getText();
  std::cerr << "Leave Const_bool_const" << std::endl;
  return nullptr;
}

std::any Analyzer::visitSigned_const_const(
    CACTParser::Signed_const_constContext *context) {
  std::cerr << "Enter Signed_const_const" << std::endl;
  context->num_const()->accept(this);
  context->btype = context->num_const()->btype;
  context->value = context->num_const()->value;
  context->sign = false;
  std::cerr << "Leave Signed_const_const" << std::endl;
  return nullptr;
}

std::any Analyzer::visitSigned_const_plus(
    CACTParser::Signed_const_plusContext *context) {
  std::cerr << "Enter Signed_const_plus" << std::endl;
  context->signed_num_const()->accept(this);
  context->btype = context->signed_num_const()->btype;
  context->value = context->signed_num_const()->value;
  context->sign = context->signed_num_const()->sign;
  std::cerr << "Leave Signed_const_plus" << std::endl;
  return nullptr;
}

std::any Analyzer::visitSigned_const_minus(
    CACTParser::Signed_const_minusContext *context) {
  std::cerr << "Enter Signed_const_minus" << std::endl;
  context->signed_num_const()->accept(this);
  context->btype = context->signed_num_const()->btype;
  context->value = context->signed_num_const()->value;
  context->sign = !context->signed_num_const()->sign;
  std::cerr << "Leave Signed_const_minus" << std::endl;
  return nullptr;
}

std::any Analyzer::visitSigned_const_num_const(
    CACTParser::Signed_const_num_constContext *context) {
  std::cerr << "Enter Signed_const_num_const" << std::endl;
  context->signed_num_const()->accept(this);
  context->btype = context->signed_num_const()->btype;
  if (context->signed_num_const()->sign)
    context->value = "-" + context->signed_num_const()->value;
  else
    context->value = context->signed_num_const()->value;
  std::cerr << "Leave Signed_const_num_const" << std::endl;
  return nullptr;
}

std::any Analyzer::visitSigned_const_bool_const(
    CACTParser::Signed_const_bool_constContext *context) {
  std::cerr << "Enter Signed_const_bool_const" << std::endl;
  context->btype = Bool;
  context->value = context->boolconst()->getText();
  std::cerr << "Leave Signed_const_bool_const" << std::endl;
  return nullptr;
}

std::any Analyzer::visitInt_constant(CACTParser::Int_constantContext *context) {
  std::cerr << "Enter Int_constant" << std::endl;

  std::cerr << "Leave Int_constant" << std::endl;
  return nullptr;
}

std::any Analyzer::visitHex_constant(CACTParser::Hex_constantContext *context) {
  std::cerr << "Enter Hex_constant" << std::endl;

  std::cerr << "Leave Hex_constant" << std::endl;
  return nullptr;
}

std::any Analyzer::visitOct_constant(CACTParser::Oct_constantContext *context) {
  std::cerr << "Enter Oct_constant" << std::endl;

  std::cerr << "Leave Oct_constant" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitTrue_constant(CACTParser::True_constantContext *context) {
  std::cerr << "Enter True_constant" << std::endl;

  std::cerr << "Leave True_constant" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitFalse_constant(CACTParser::False_constantContext *context) {
  std::cerr << "Enter False_constant" << std::endl;

  std::cerr << "Leave False_constant" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitExpr_2_expr_1(CACTParser::Expr_2_expr_1Context *context) {
  std::cerr << "Enter Expr_2_expr_1" << std::endl;
  context->expr_1()->accept(this);
  context->btype = context->expr_1()->btype;
  context->array_size = context->expr_1()->array_size;
  context->res = context->expr_1()->res;
  std::cerr << "Leave Expr_2_expr_1" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_2_plus(CACTParser::Expr_2_plusContext *context) {
  std::cerr << "Enter Expr_2_plus" << std::endl;
  context->expr_2()->accept(this);
  if (context->expr_2()->btype == Bool ||
      !context->expr_2()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = context->expr_2()->btype;
  context->array_size = {};
  std::string res = "%" + createVar(context->btype, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "assign " << res << " " << context->expr_2()->res << std::endl;
  context->res = res;
  std::cerr << "Leave Expr_2_plus" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_2_minus(CACTParser::Expr_2_minusContext *context) {
  std::cerr << "Enter Expr_2_minus" << std::endl;
  context->expr_2()->accept(this);
  if (context->expr_2()->btype == Bool ||
      !context->expr_2()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = context->expr_2()->btype;
  context->array_size = {};
  std::string res = "%" + createVar(context->btype, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "Neg " << btype_string[context->btype] << " " << res << " "
            << context->expr_2()->res << std::endl;
  context->res = res;
  std::cerr << "Leave Expr_2_minus" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_2_not(CACTParser::Expr_2_notContext *context) {
  std::cerr << "Enter Expr_2_not" << std::endl;
  context->expr_2()->accept(this);
  if (context->expr_2()->btype != Bool ||
      !context->expr_2()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = Bool;
  context->array_size = {};
  std::string res = "%" + createVar(Bool, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "Not " << res << " " << context->expr_2()->res << std::endl;
  context->res = res;
  std::cerr << "Leave Expr_2_not" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitExpr_3_divide(CACTParser::Expr_3_divideContext *context) {
  std::cerr << "Enter Expr_3_divide" << std::endl;
  context->expr_2()->accept(this);
  context->expr_3()->accept(this);
  if (context->expr_3()->btype != context->expr_2()->btype) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (context->expr_3()->btype == Bool || context->expr_2()->btype == Bool) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (!context->expr_3()->array_size.empty() ||
      !context->expr_2()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = context->expr_3()->btype;
  context->array_size = {};
  std::string res = "%" + createVar(context->btype, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "Div " << btype_string[context->btype] << " " << res << " "
            << context->expr_3()->res << " " << context->expr_2()->res
            << std::endl;
  context->res = res;
  std::cerr << "Leave Expr_3_divide" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_3_times(CACTParser::Expr_3_timesContext *context) {
  std::cerr << "Enter Expr_3_times" << std::endl;
  context->expr_2()->accept(this);
  context->expr_3()->accept(this);
  if (context->expr_3()->btype != context->expr_2()->btype) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (context->expr_3()->btype == Bool || context->expr_2()->btype == Bool) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (!context->expr_3()->array_size.empty() ||
      !context->expr_2()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = context->expr_3()->btype;
  context->array_size = {};
  std::string res = "%" + createVar(context->btype, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "Mul " << btype_string[context->btype] << " " << res << " "
            << context->expr_3()->res << " " << context->expr_2()->res
            << std::endl;
  context->res = res;
  std::cerr << "Leave Expr_3_times" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitExpr_3_remain(CACTParser::Expr_3_remainContext *context) {
  std::cerr << "Enter Expr_3_remain" << std::endl;
  context->expr_2()->accept(this);
  context->expr_3()->accept(this);
  if (context->expr_3()->btype != Int || context->expr_2()->btype != Int) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (!context->expr_3()->array_size.empty() ||
      !context->expr_2()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = Int;
  context->array_size = {};
  std::string res = "%" + createVar(Int, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "Rem " << res << " " << context->expr_3()->res << " "
            << context->expr_2()->res << std::endl; // must be Int
  context->res = res;
  std::cerr << "Leave Expr_3_remain" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitExpr_3_expr_2(CACTParser::Expr_3_expr_2Context *context) {
  std::cerr << "Enter Expr_3_expr_2" << std::endl;
  context->expr_2()->accept(this);
  context->btype = context->expr_2()->btype;
  context->array_size = context->expr_2()->array_size;
  context->res = context->expr_2()->res;
  std::cerr << "Leave Expr_3_expr_2" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_4_plus(CACTParser::Expr_4_plusContext *context) {
  std::cerr << "Enter Expr_4_plus" << std::endl;
  context->expr_3()->accept(this);
  context->expr_4()->accept(this);
  if (context->expr_4()->btype != context->expr_3()->btype) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (context->expr_4()->btype == Bool || context->expr_3()->btype == Bool) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (!context->expr_4()->array_size.empty() ||
      !context->expr_3()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = context->expr_4()->btype;
  context->array_size = {};
  std::string res = "%" + createVar(context->btype, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "Add " << btype_string[context->btype] << " " << res << " "
            << context->expr_4()->res << " " << context->expr_3()->res
            << std::endl;
  context->res = res;
  std::cerr << "Leave Expr_4_plus" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitExpr_4_expr_3(CACTParser::Expr_4_expr_3Context *context) {
  std::cerr << "Enter Expr_4_expr_3" << std::endl;
  context->expr_3()->accept(this);
  context->btype = context->expr_3()->btype;
  context->array_size = context->expr_3()->array_size;
  context->res = context->expr_3()->res;
  std::cerr << "Leave Expr_4_expr_3" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_4_minus(CACTParser::Expr_4_minusContext *context) {
  std::cerr << "Enter Expr_4_minus" << std::endl;
  context->expr_3()->accept(this);
  context->expr_4()->accept(this);
  if (context->expr_4()->btype != context->expr_3()->btype) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (context->expr_4()->btype == Bool || context->expr_3()->btype == Bool) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (!context->expr_4()->array_size.empty() ||
      !context->expr_3()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = context->expr_4()->btype;
  context->array_size = {};
  std::string res = "%" + createVar(context->btype, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "Sub " << btype_string[context->btype] << " " << res << " "
            << context->expr_4()->res << " " << context->expr_3()->res
            << std::endl;
  context->res = res;
  std::cerr << "Leave Expr_4_minus" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_5_ge(CACTParser::Expr_5_geContext *context) {
  std::cerr << "Enter Expr_5_ge" << std::endl;
  context->expr_4()->accept(this);
  context->expr_5()->accept(this);
  if (context->expr_5()->btype != context->expr_4()->btype) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (context->expr_5()->btype == Bool || context->expr_4()->btype == Bool) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (!context->expr_5()->array_size.empty() ||
      !context->expr_4()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = Bool;
  context->array_size = {};
  std::string res = "%" + createVar(Bool, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "GE " << btype_string[context->expr_4()->btype] << " " << res << " "
            << context->expr_5()->res << " " << context->expr_4()->res
            << std::endl;
  context->res = res;
  std::cerr << "Leave Expr_5_ge" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_5_gt(CACTParser::Expr_5_gtContext *context) {
  std::cerr << "Enter Expr_5_gt" << std::endl;
  context->expr_4()->accept(this);
  context->expr_5()->accept(this);
  if (context->expr_5()->btype != context->expr_4()->btype) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (context->expr_5()->btype == Bool || context->expr_4()->btype == Bool) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (!context->expr_5()->array_size.empty() ||
      !context->expr_4()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = Bool;
  context->array_size = {};
  std::string res = "%" + createVar(Bool, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "GT " << btype_string[context->expr_4()->btype] << " " << res
            << " " << context->expr_5()->res << " " << context->expr_4()->res
            << std::endl;
  context->res = res;
  std::cerr << "Leave Expr_5_gt" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitExpr_5_expr_4(CACTParser::Expr_5_expr_4Context *context) {
  std::cerr << "Enter Expr_5_expr_4" << std::endl;
  context->expr_4()->accept(this);
  context->btype = context->expr_4()->btype;
  context->array_size = context->expr_4()->array_size;
  context->res = context->expr_4()->res;
  std::cerr << "Leave Expr_5_expr_4" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_5_le(CACTParser::Expr_5_leContext *context) {
  std::cerr << "Enter Expr_5_le" << std::endl;
  context->expr_4()->accept(this);
  context->expr_5()->accept(this);
  if (context->expr_5()->btype != context->expr_4()->btype) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (context->expr_5()->btype == Bool || context->expr_4()->btype == Bool) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (!context->expr_5()->array_size.empty() ||
      !context->expr_4()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = Bool;
  context->array_size = {};
  std::string res = "%" + createVar(Bool, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "LE " << btype_string[context->expr_4()->btype] << " " << res
            << " " << context->expr_5()->res << " " << context->expr_4()->res
            << std::endl;
  context->res = res;
  std::cerr << "Leave Expr_5_le" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_5_lt(CACTParser::Expr_5_ltContext *context) {
  std::cerr << "Enter Expr_5_lt" << std::endl;
  context->expr_4()->accept(this);
  context->expr_5()->accept(this);
  if (context->expr_5()->btype != context->expr_4()->btype) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (context->expr_5()->btype == Bool || context->expr_4()->btype == Bool) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (!context->expr_5()->array_size.empty() ||
      !context->expr_4()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = Bool;
  context->array_size = {};
  std::string res = "%" + createVar(Bool, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "LT " << btype_string[context->expr_4()->btype] << " " << res
            << " " << context->expr_5()->res << " " << context->expr_4()->res
            << std::endl;
  context->res = res;
  std::cerr << "Leave Expr_5_lt" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitExpr_6_expr_5(CACTParser::Expr_6_expr_5Context *context) {
  std::cerr << "Enter Expr_6_expr_5" << std::endl;
  context->expr_5()->accept(this);
  context->btype = context->expr_5()->btype;
  context->array_size = context->expr_5()->array_size;
  context->res = context->expr_5()->res;
  std::cerr << "Leave Expr_6_expr_5" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_6_ne(CACTParser::Expr_6_neContext *context) {
  std::cerr << "Enter Expr_6_ne" << std::endl;
  context->expr_5()->accept(this);
  context->expr_6()->accept(this);
  if (context->expr_6()->btype != context->expr_5()->btype) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (!context->expr_6()->array_size.empty() ||
      !context->expr_5()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = Bool;
  context->array_size = {};
  std::string res = "%" + createVar(Bool, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "NE " << btype_string[context->expr_5()->btype] << " " << res
            << " " << context->expr_6()->res << " " << context->expr_5()->res
            << std::endl;
  context->res = res;
  std::cerr << "Leave Expr_6_ne" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_6_eq(CACTParser::Expr_6_eqContext *context) {
  std::cerr << "Enter Expr_6_eq" << std::endl;
  context->expr_5()->accept(this);
  context->expr_6()->accept(this);
  if (context->expr_6()->btype != context->expr_5()->btype) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (!context->expr_6()->array_size.empty() ||
      !context->expr_5()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->btype = Bool;
  context->array_size = {};
  std::string res = "%" + createVar(Bool, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "EQ " << btype_string[context->expr_5()->btype] << " " << res
            << " " << context->expr_6()->res << " " << context->expr_5()->res
            << std::endl;
  context->res = res;
  std::cerr << "Leave Expr_6_eq" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitExpr_7_expr_6(CACTParser::Expr_7_expr_6Context *context) {
  std::cerr << "Enter Expr_7_expr_6" << std::endl;
  context->expr_6()->accept(this);
  context->btype = context->expr_6()->btype;
  context->array_size = context->expr_6()->array_size;
  context->res = context->expr_6()->res;
  std::cerr << "Leave Expr_7_expr_6" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_7_dand(CACTParser::Expr_7_dandContext *context) {
  std::cerr << "Enter Expr_7_dand" << std::endl;
  std::string res = "%" + createVar(Bool, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "assign " << res << " true" << std::endl;
  context->expr_7()->accept(this);
  if (context->expr_7()->btype != Bool || !context->expr_7()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  std::string f_label = createLabel();
  std::string nres = "%" + createVar(Bool, false);
  std::cout << "@var " << nres << std::endl;
  std::cout << "DAnd " << res << " " << res << " " << context->expr_7()->res << std::endl;
  std::cout << "Not " << nres << " " << res << std::endl;
  std::cout << "branch " << f_label << " " << nres << std::endl;
  context->expr_6()->accept(this);
  if (context->expr_6()->btype != Bool || !context->expr_6()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  std::cout << "DAnd " << res << " " << res << " " << context->expr_6()->res << std::endl;
  std::cout << "label " << f_label << std::endl;
  context->btype = Bool;
  context->array_size = {};
  context->res = res;
  std::cerr << "Leave Expr_7_dand" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_8_dor(CACTParser::Expr_8_dorContext *context) {
  std::cerr << "Enter Expr_8_dor" << std::endl;
  std::string res = "%" + createVar(Bool, false);
  std::cout << "@var " << res << std::endl;
  std::cout << "assign " << res << " false" << std::endl;
  context->expr_8()->accept(this);
  if (context->expr_8()->btype != Bool || !context->expr_8()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  std::string t_label = createLabel();
  std::cout << "DOr " << res << " " << res << " " << context->expr_8()->res << std::endl;
  std::cout << "branch " << t_label << " " << res << std::endl;
  context->expr_7()->accept(this);
  if (context->expr_7()->btype != Bool || !context->expr_7()->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  std::cout << "DOr " << res << " " << res << " " << context->expr_7()->res << std::endl;
  std::cout << "label " << t_label << std::endl;
  context->btype = Bool;
  context->array_size = {};
  context->res = res;
  std::cerr << "Leave Expr_8_dor" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitExpr_8_expr_7(CACTParser::Expr_8_expr_7Context *context) {
  std::cerr << "Enter Expr_8_expr_7" << std::endl;
  context->expr_7()->accept(this);
  context->btype = context->expr_7()->btype;
  context->array_size = context->expr_7()->array_size;
  context->res = context->expr_7()->res;
  std::cerr << "Leave Expr_8_expr_7" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitFunc_call_ident(CACTParser::Func_call_identContext *context) {
  std::cerr << "Enter Func_call_ident" << std::endl;
  std::string name = context->Ident()->getText();
  auto res = g_functable.resolve(name);
  if (!res.has_value()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  Func func = res.value();
  context->btype = func.retvalType();
  if (func.params().size() != context->expr_8().size()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  std::string func_call_Res;
  if (func.retvalType() == Void) {
    func_call_Res = "void";
  } else {
    func_call_Res = "%" + createVar(func.retvalType(), false);
    std::cout << "@var " << func_call_Res << std::endl;
    context->res = func_call_Res;
  }
  for (int i = 0; i < (int)context->expr_8().size(); ++i) {
    context->expr_8()[i]->accept(this);
    if (context->expr_8()[i]->btype != func.params()[i].bType) {
      //
      exit(3);
      assert(0);
      return nullptr;
    }
    if (context->expr_8()[i]->array_size.size() !=
        func.params()[i].arraySize.size()) {
      //
      exit(3);
      assert(0);
      return nullptr;
    }
    for (int j = 0; j < (int)context->expr_8()[i]->array_size.size(); ++j) {
      if (func.params()[i].arraySize[j] != 0 &&
          func.params()[i].arraySize[j] !=
              context->expr_8()[i]->array_size[j]) {
        //
        exit(3);
        assert(0);
        return nullptr;
      }
    }
  }
  std::cout << "call " << func_call_Res << " %" << func.name_in_IR() << " ( ";
  for (int i = 0; i < (int)context->expr_8().size(); ++i) {
    std::cout << context->expr_8()[i]->res << " ";
    if (i + 1 < (int)context->expr_8().size())
      std::cout << ", ";
  }
  std::cout << ")" << std::endl;
  std::cerr << "Leave Func_call_ident" << std::endl;
  return nullptr;
}

std::any Analyzer::visitType_int(CACTParser::Type_intContext *context) {
  std::cerr << "Enter Type_int" << std::endl;

  std::cerr << "Leave Type_int" << std::endl;
  return nullptr;
}

std::any Analyzer::visitType_bool(CACTParser::Type_boolContext *context) {
  std::cerr << "Enter Type_bool" << std::endl;

  std::cerr << "Leave Type_bool" << std::endl;
  return nullptr;
}

std::any Analyzer::visitType_float(CACTParser::Type_floatContext *context) {
  std::cerr << "Enter Type_float" << std::endl;

  std::cerr << "Leave Type_float" << std::endl;
  return nullptr;
}

std::any Analyzer::visitType_double(CACTParser::Type_doubleContext *context) {
  std::cerr << "Enter Type_double" << std::endl;

  std::cerr << "Leave Type_double" << std::endl;
  return nullptr;
}

std::any Analyzer::visitType_void(CACTParser::Type_voidContext *context) {
  std::cerr << "Enter Type_void" << std::endl;

  std::cerr << "Leave Type_void" << std::endl;
  return nullptr;
}

std::any Analyzer::visitStmt_assign(CACTParser::Stmt_assignContext *context) {
  std::cerr << "Enter Stmt_assign" << std::endl;
  context->l_value()->accept(this);
  context->expr_8()->accept(this);
  if (context->expr_8()->btype != context->l_value()->btype) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  std::cout << "assign %" << context->l_value()->code << " "
            << context->expr_8()->res << std::endl;
  std::cerr << "Leave Stmt_assign" << std::endl;
  return nullptr;
}

std::any Analyzer::visitL_value(CACTParser::L_valueContext *context) {
  std::cerr << "Enter L_value" << std::endl;
  std::string varName = context->Ident()->getText();
  auto res = g_symtree.resolve(varName);
  if (!res.has_value()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  Symbol var = res.value();
  if (var.isConst()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (context->expr_8().size() != var.arraySize().size()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  for (int i = 0; i < (int)context->expr_8().size(); ++i) {
    context->expr_8()[i]->accept(this);
    if (context->expr_8()[i]->btype != Int) {
      //
      exit(3);
      assert(0);
      return nullptr;
    }
  }
  context->btype = var.bType();
  if (var.isArray()) {
    std::string offset = "%" + createVar(Int, false);
    std::string tmp = "%" + createVar(Int, false);
    std::cout << "@var " << offset << std::endl;
    std::cout << "@var " << tmp << std::endl;
    std::cout << "assign " << offset << " " << context->expr_8().back()->res
              << std::endl;
    int scale = var.arraySize().back();
    for (int i = (int)var.arraySize().size() - 2; i >= 0; --i) {
      std::cout << "Mul i " << tmp << " " << context->expr_8()[i]->res << " "
                << scale << std::endl;
      std::cout << "Add i " << offset << " " << offset << " " << tmp << std::endl;
      scale *= var.arraySize()[i];
    }
    context->code = var.name_in_IR() + "[" + offset + "]";
  } else {
    context->code = var.name_in_IR();
  }
  std::cerr << "Leave L_value" << std::endl;
  return nullptr;
}

std::any Analyzer::visitStmt_expr(CACTParser::Stmt_exprContext *context) {
  std::cerr << "Enter Stmt_expr" << std::endl;
  if (context->expr_8() != nullptr)
    context->expr_8()->accept(this);
  std::cerr << "Leave Stmt_expr" << std::endl;
  return nullptr;
}

std::any Analyzer::visitStmt_block(CACTParser::Stmt_blockContext *context) {
  std::cerr << "Enter Stmt_block" << std::endl;
  context->block()->need_type = context->need_type;
  context->block()->brk_target = context->brk_target;
  context->block()->ctn_target = context->ctn_target;
  context->block()->has_return = false;
  g_symtree.enterScope();
  context->block()->accept(this);
  g_symtree.leaveScope();
  if (context->block()->has_return)
    context->has_return = true;
  std::cerr << "Leave Stmt_block" << std::endl;
  return nullptr;
}

std::any Analyzer::visitStmt_return(CACTParser::Stmt_returnContext *context) {
  std::cerr << "Enter Stmt_return" << std::endl;
  if (context->need_type == Void) {
    if (context->expr_8() != nullptr) {
      //
      exit(3);
      assert(0);
      return nullptr;
    }
    std::cout << "return" << std::endl;
  } else {
    context->expr_8()->accept(this);
    if (context->expr_8()->btype != context->need_type) {
      //
      exit(3);
      assert(0);
      return nullptr;
    }
    std::cout << "return " << context->expr_8()->res << std::endl;
  }
  std::cerr << "Leave Stmt_return" << std::endl;
  return nullptr;
}

std::any Analyzer::visitStmt_if(CACTParser::Stmt_ifContext *context) {
  std::cerr << "Enter Stmt_if" << std::endl;
  context->expr_8()->accept(this);
  if (context->expr_8()->btype != Bool) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  std::string ncond = "%" + createVar(Bool, false);
  std::cout << "@var " << ncond << std::endl;
  std::cout << "Not " << ncond << " " << context->expr_8()->res << std::endl;
  std::string leaveLabel = createLabel();
  if (context->stmt().size() > 1) { // if-else
    std::string elseLabel = createLabel();
    std::cout << "branch " << elseLabel << " " << ncond << std::endl;
    bool has_return = true;
    for (size_t i = 0; i < context->stmt().size(); ++i) {
      if (i == 1)
        std::cout << "label " << elseLabel << std::endl;
      context->stmt()[i]->brk_target = context->brk_target;
      context->stmt()[i]->ctn_target = context->ctn_target;
      context->stmt()[i]->need_type = context->need_type;
      context->stmt()[i]->has_return = false;
      context->stmt()[i]->accept(this);
      if (!context->stmt()[i]->has_return)
        has_return = false;
      if (i == 0)
        std::cout << "branch " << leaveLabel << " true" << std::endl;
    }
    context->has_return = has_return;
  } else { // if
    std::cout << "branch " << leaveLabel << " " << ncond << std::endl;
    context->stmt()[0]->brk_target = context->brk_target;
    context->stmt()[0]->ctn_target = context->ctn_target;
    context->stmt()[0]->need_type = context->need_type;
    context->stmt()[0]->has_return = false;
    context->stmt()[0]->accept(this);
  }
  std::cout << "label " << leaveLabel << std::endl;
  std::cerr << "Leave Stmt_if" << std::endl;
  return nullptr;
}

std::any Analyzer::visitStmt_while(CACTParser::Stmt_whileContext *context) {
  std::cerr << "Enter Stmt_while" << std::endl;
  std::string enterLabel = createLabel();
  std::string leaveLabel = createLabel();
  std::cout << "label " << enterLabel << std::endl;
  context->expr_8()->accept(this);
  if (context->expr_8()->btype != Bool) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  std::string ncond = "%" + createVar(Bool, false);
  std::cout << "@var " << ncond << std::endl;
  std::cout << "Not " << ncond << " " << context->expr_8()->res << std::endl;
  std::cout << "branch " << leaveLabel << " " << ncond << std::endl;
  context->stmt()->ctn_target = enterLabel;
  context->stmt()->brk_target = leaveLabel;
  context->stmt()->need_type = context->need_type;
  context->stmt()->has_return = false;
  context->stmt()->accept(this);
  std::cout << "branch " << enterLabel << " true" << std::endl;
  std::cout << "label " << leaveLabel << std::endl;
  //
  std::cerr << "Leave Stmt_while" << std::endl;
  return nullptr;
}

std::any Analyzer::visitStmt_break(CACTParser::Stmt_breakContext *context) {
  std::cerr << "Enter Stmt_break" << std::endl;
  if (context->brk_target.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  std::cout << "branch " << context->brk_target << " true" << std::endl;
  std::cerr << "Leave Stmt_break" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitStmt_continue(CACTParser::Stmt_continueContext *context) {
  std::cerr << "Enter Stmt_continue" << std::endl;
  if (context->ctn_target.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  std::cout << "branch " << context->ctn_target << " true" << std::endl;
  std::cerr << "Leave Stmt_continue" << std::endl;
  return nullptr;
}

std::any Analyzer::visitAssign_stmt(CACTParser::Assign_stmtContext *context) {
  std::cerr << "Enter Assign_stmt" << std::endl;
  context->stmt_assign()->accept(this);
  std::cerr << "Leave Assign_stmt" << std::endl;
  return nullptr;
}

std::any Analyzer::visitExpr_stmt(CACTParser::Expr_stmtContext *context) {
  std::cerr << "Enter Expr_stmt" << std::endl;
  context->stmt_expr()->accept(this);
  std::cerr << "Leave Expr_stmt" << std::endl;
  return nullptr;
}

std::any Analyzer::visitBlock_stmt(CACTParser::Block_stmtContext *context) {
  std::cerr << "Enter Block_stmt" << std::endl;
  context->stmt_block()->need_type = context->need_type;
  context->stmt_block()->brk_target = context->brk_target;
  context->stmt_block()->ctn_target = context->ctn_target;
  context->stmt_block()->has_return = false;
  context->stmt_block()->accept(this);
  if (context->stmt_block()->has_return)
    context->has_return = true;
  std::cerr << "Leave Block_stmt" << std::endl;
  return nullptr;
}

std::any Analyzer::visitReturn_stmt(CACTParser::Return_stmtContext *context) {
  std::cerr << "Enter Return_stmt" << std::endl;
  context->stmt_return()->need_type = context->need_type;
  context->stmt_return()->accept(this);
  context->has_return = true;
  std::cerr << "Leave Return_stmt" << std::endl;
  return nullptr;
}

std::any Analyzer::visitIf_stmt(CACTParser::If_stmtContext *context) {
  std::cerr << "Enter If_stmt" << std::endl;
  context->stmt_if()->need_type = context->need_type;
  context->stmt_if()->brk_target = context->brk_target;
  context->stmt_if()->ctn_target = context->ctn_target;
  context->stmt_if()->has_return = false;
  context->stmt_if()->accept(this);
  if (context->stmt_if()->has_return)
    context->has_return = true;
  std::cerr << "Leave If_stmt" << std::endl;
  return nullptr;
}

std::any Analyzer::visitWhile_stmt(CACTParser::While_stmtContext *context) {
  std::cerr << "Enter While_stmt" << std::endl;
  context->stmt_while()->need_type = context->need_type;
  context->stmt_while()->brk_target = context->brk_target;
  context->stmt_while()->ctn_target = context->ctn_target;
  context->stmt_while()->has_return = false;
  context->stmt_while()->accept(this);
  // if (context->stmt_while()->has_return) context->has_return = true;
  std::cerr << "Leave While_stmt" << std::endl;
  return nullptr;
}

std::any Analyzer::visitBreak_stmt(CACTParser::Break_stmtContext *context) {
  std::cerr << "Enter Break_stmt" << std::endl;
  context->stmt_break()->brk_target = context->brk_target;
  context->stmt_break()->accept(this);
  std::cerr << "Leave Break_stmt" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitContinue_stmt(CACTParser::Continue_stmtContext *context) {
  std::cerr << "Enter Continue_stmt" << std::endl;
  context->stmt_continue()->ctn_target = context->ctn_target;
  context->stmt_continue()->accept(this);
  std::cerr << "Leave Continue_stmt" << std::endl;
  return nullptr;
}

std::any Analyzer::visitDecl_var(CACTParser::Decl_varContext *context) {
  std::cerr << "Enter Decl_var" << std::endl;
  Btype btype = str_to_type(context->type()->getText());
  for (int i = 0; i < (int)context->var_def().size(); ++i) {
    context->var_def()[i]->is_global = context->is_global;
    context->var_def()[i]->need_type = btype;
    context->var_def()[i]->accept(this);
  }
  std::cerr << "Leave Decl_var" << std::endl;
  return nullptr;
}

std::any Analyzer::visitDecl_const(CACTParser::Decl_constContext *context) {
  std::cerr << "Enter Decl_const" << std::endl;
  Btype btype = str_to_type(context->type()->getText());
  for (int i = 0; i < (int)context->const_def().size(); ++i) {
    context->const_def()[i]->is_global = context->is_global;
    context->const_def()[i]->need_type = btype;
    context->const_def()[i]->accept(this);
  }
  std::cerr << "Leave Decl_const" << std::endl;
  return nullptr;
}

std::any Analyzer::visitVar_decl(CACTParser::Var_declContext *context) {
  std::cerr << "Enter Var_decl" << std::endl;
  context->decl_var()->is_global = context->is_global;
  context->decl_var()->accept(this);
  std::cerr << "Leave Var_decl" << std::endl;
  return nullptr;
}

std::any Analyzer::visitConst_decl(CACTParser::Const_declContext *context) {
  std::cerr << "Enter Const_decl" << std::endl;
  context->decl_const()->is_global = context->is_global;
  context->decl_const()->accept(this);
  std::cerr << "Leave Const_decl" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitBlock_item_decl(CACTParser::Block_item_declContext *context) {
  std::cerr << "Enter Block_item_decl" << std::endl;
  context->decl()->is_global = false;
  context->decl()->accept(this);
  std::cerr << "Leave Block_item_decl" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitBlock_item_stmt(CACTParser::Block_item_stmtContext *context) {
  std::cerr << "Enter Block_item_stmt" << std::endl;
  context->stmt()->need_type = context->need_type;
  context->stmt()->brk_target = context->brk_target;
  context->stmt()->ctn_target = context->ctn_target;
  context->stmt()->has_return = false;
  context->stmt()->accept(this);
  if (context->stmt()->has_return)
    context->has_return = true;
  std::cerr << "Leave Block_item_stmt" << std::endl;
  return nullptr;
}

std::any Analyzer::visitBlock(CACTParser::BlockContext *context) {
  std::cerr << "Enter Block" << std::endl;
  bool has_return = false;
  for (size_t i = 0; i < context->block_item().size(); ++i) {
    context->block_item()[i]->need_type = context->need_type;
    context->block_item()[i]->brk_target = context->brk_target;
    context->block_item()[i]->ctn_target = context->ctn_target;
    context->block_item()[i]->has_return = false;
    context->block_item()[i]->accept(this);
    if (context->block_item()[i]->has_return)
      has_return = true;
  }
  context->has_return = has_return;
  std::cerr << "Leave Block" << std::endl;
  return nullptr;
}

std::any Analyzer::visitVar_def(CACTParser::Var_defContext *context) {
  std::cerr << "Enter Var_def" << std::endl;
  std::string name = context->Ident()->getText();
  if (g_symtree.check(name)) {
    // 
    exit(3);
    assert(0);
    return nullptr;
  }
  int total_size = 1;
  std::vector<size_t> array_size;
  array_size.clear();
  if (context->intconst().empty()) { // variant
    if (!g_symtree.define(context->need_type, name, false, false, {})) {
      //
      exit(3);
      assert(0);
      return nullptr;
    }
    if (context->is_global)
      std::cout << "!global ";
    std::cout << "@var %" << name << std::endl;
  } else { // array
    for (int i = 0; i < (int)context->intconst().size(); ++i) {
      int sz = parse_int(context->intconst()[i]->getText());
      array_size.push_back(sz);
      total_size *= sz;
    }
    if (!g_symtree.define(context->need_type, name, true, false, array_size)) {
      //
      exit(3);
      assert(0);
      return nullptr;
    }
    if (context->is_global)
      std::cout << "!global ";
    std::cout << "@array %" << name << " " << total_size << std::endl;
  }

  if (context->array_signed_const() != nullptr) { // has value
    context->array_signed_const()->need_type = context->need_type;
    context->array_signed_const()->array_size = array_size;
    context->array_signed_const()->varName = name;
    context->array_signed_const()->offset = 0;
    context->array_signed_const()->at_top = true;
    context->array_signed_const()->is_global = context->is_global;
    context->array_signed_const()->accept(this);
  } else {                             // no value, assign to 0
    if (context->intconst().empty()) { // variant
      if (context->is_global)
        std::cout << "!global ";
      std::cout << "assign %" << name << " 0" << std::endl;
    } else { // array
      for (int i = 0; i < total_size; ++i) {
        if (context->is_global)
          std::cout << "!global ";
        std::cout << "assign %" << name << "[" << i << "] 0" << std::endl;
      }
    }
  }
  std::cerr << "Leave Var_def" << std::endl;
  return nullptr;
}

std::any Analyzer::visitConst_def(CACTParser::Const_defContext *context) {
  std::cerr << "Enter Const_def" << std::endl;
  std::string name = context->Ident()->getText();
  if (g_symtree.check(name)) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  std::vector<size_t> array_size;
  array_size.clear();
  if (context->intconst().empty()) { // variant
    if (!g_symtree.define(context->need_type, name, false, true, {})) {
      //
      exit(3);
      assert(0);
      return nullptr;
    }
    if (context->is_global)
      std::cout << "!global ";
    std::cout << "@var %" << name << std::endl;
  } else { // array
    int total_size = 1;
    for (int i = 0; i < (int)context->intconst().size(); ++i) {
      int sz = parse_int(context->intconst()[i]->getText());
      array_size.push_back(sz);
      total_size *= sz;
    }
    if (!g_symtree.define(context->need_type, name, true, true, array_size)) {
      //
      exit(3);
      assert(0);
      return nullptr;
    }
    if (context->is_global)
      std::cout << "!global ";
    std::cout << "@array %" << name << " " << total_size << std::endl;
  }
  context->array_signed_const()->need_type = context->need_type;
  context->array_signed_const()->array_size = array_size;
  context->array_signed_const()->varName = name;
  context->array_signed_const()->offset = 0;
  context->array_signed_const()->at_top = true;
  context->array_signed_const()->is_global = context->is_global;
  context->array_signed_const()->accept(this);
  std::cerr << "Leave Const_def" << std::endl;
  return nullptr;
}

std::any Analyzer::visitArray_signed_const_const(
    CACTParser::Array_signed_const_constContext *context) {
  std::cerr << "Enter Array_signed_const_const" << std::endl;
  if (!context->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  context->signed_const()->accept(this);
  if (context->need_type != context->signed_const()->btype) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (context->varName[0] == 'a') {
    if (context->is_global)
      std::cout << "!global ";
    std::cout << "assign %" << context->varName << "[" << context->offset
              << "] " << context->signed_const()->value << std::endl;
  } else {
    if (context->is_global)
      std::cout << "!global ";
    std::cout << "assign %" << context->varName << " "
              << context->signed_const()->value << std::endl;
  }
  std::cerr << "Leave Array_signed_const_const" << std::endl;
  return nullptr;
}

std::any Analyzer::visitArray_signed_const_array(
    CACTParser::Array_signed_const_arrayContext *context) {
  std::cerr << "Enter Array_signed_const_array" << std::endl;
  if (context->array_size.empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (context->at_top) { // int a[2][3] = {1,2,3,4,5,6}
    bool flag = true;
    for (int i = 0; i < (int)context->array_signed_const().size(); ++i) {
      if (context->array_signed_const()[i]->getText()[0] == '{') {
        flag = false;
        break;
      }
    }
    if (flag) { // satisfied
      int sz = 1;
      for (int i = 0; i < (int)context->array_size.size(); ++i)
        sz *= context->array_size[i];
      if ((int)context->array_signed_const().size() > sz) {
        //
        exit(3);
        assert(0);
        return nullptr;
      }
      for (int i = 0; i < (int)context->array_signed_const().size(); ++i) {
        context->array_signed_const()[i]->need_type = context->need_type;
        context->array_signed_const()[i]->array_size = {};
        context->array_signed_const()[i]->varName = context->varName;
        context->array_signed_const()[i]->offset = i;
        context->array_signed_const()[i]->at_top = false;
        context->array_signed_const()[i]->is_global = context->is_global;
        context->array_signed_const()[i]->accept(this);
      }
      for (int i = (int)context->array_signed_const().size(); i < sz; ++i) {
        if (context->is_global)
          std::cout << "!global ";
        std::cout << "assign %" << context->varName << "[" << i << "] 0"
                  << std::endl;
      }
      return nullptr;
    }
  }
  if (context->array_signed_const().size() > context->array_size[0]) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  std::vector<size_t> nxt_array_size;
  int sz = 1;
  for (int i = 1; i < (int)context->array_size.size(); ++i) {
    nxt_array_size.push_back(context->array_size[i]);
    sz *= context->array_size[i];
  }
  for (int i = 0; i < (int)context->array_signed_const().size(); ++i) {
    context->array_signed_const()[i]->need_type = context->need_type;
    context->array_signed_const()[i]->array_size = nxt_array_size;
    context->array_signed_const()[i]->offset = context->offset + i * sz;
    context->array_signed_const()[i]->varName = context->varName;
    context->array_signed_const()[i]->at_top = false;
    context->array_signed_const()[i]->is_global = context->is_global;
    context->array_signed_const()[i]->accept(this);
  }
  if (context->array_signed_const().size() < context->array_size[0]) {
    for (int i = context->array_signed_const().size() * sz;
         i < (int)context->array_size[0] * sz; ++i) {
      int offset = context->offset + i;
      if (context->is_global)
        std::cout << "!global ";
      std::cout << "assign %" << context->varName << "[" << offset << "] 0"
                << std::endl;
    }
  }
  std::cerr << "Leave Array_signed_const_array" << std::endl;
  return nullptr;
}

std::any Analyzer::visitFunc_def(CACTParser::Func_defContext *context) {
  std::cerr << "Enter Func_def" << std::endl;

  std::string typeName = context->type()->getText();
  Btype retType = str_to_type(typeName);

  std::string funcName = context->Ident()->getText();
  
  if (funcName == "main" && retType != Int) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }

  if (g_functable.check(funcName)) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }

  g_symtree.enterScope();
  std::vector<FuncParamsType> params;
  params.clear();
  if (funcName == "main" && !context->func_f_param().empty()) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  for (auto func_param : context->func_f_param()) {
    func_param->accept(this);
    params.push_back(func_param->params);
  }
  if (!g_functable.define(retType, funcName, params)) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  funcName = "%" + funcName;
  std::cout << "@func " << funcName << " ( ";
  for (int i = 0; i < (int)context->func_f_param().size(); ++i) {
    std::cout << context->func_f_param()[i]->code << " ";
    if (i + 1 < (int)context->func_f_param().size())
      std::cout << ", ";
  }
  std::cout << ")" << std::endl;
  context->block()->need_type = retType;
  context->block()->has_return = false;
  context->block()->accept(this);
  if (retType != Void && !context->block()->has_return) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  g_symtree.leaveScope();
  std::cout << "@endfunc" << std::endl;
  std::cerr << "Leave Func_def" << std::endl;
  return nullptr;
}

std::any Analyzer::visitFunc_f_param(CACTParser::Func_f_paramContext *context) {
  std::cerr << "Enter Func_f_param" << std::endl;

  Btype btype = str_to_type(context->type()->getText());
  std::string varName = context->Ident()->getText();
  std::vector<size_t> array_size;
  array_size.clear();
  if (g_symtree.check(varName)) {
    //
    exit(3);
    assert(0);
    return nullptr;
  }
  if (context->MBra().size()) {                                // array
    if (context->MBra().size() > context->intconst().size()) { // int a[][3]
      array_size.push_back(0);
    }
    for (size_t i = 0; i < context->intconst().size(); ++i) {
      size_t value = parse_int(context->intconst()[i]->getText());
      array_size.push_back(value);
    }
    context->params = FuncParamsType(btype, true, array_size);
    if (!g_symtree.define(btype, varName, true, false, array_size)) {
      //
      exit(3);
      assert(0);
      return nullptr;
    }

  } else {
    context->params = FuncParamsType(btype, false);
    if (!g_symtree.define(btype, varName, false, false, array_size)) {
      //
      exit(3);
      assert(0);
      return nullptr;
    }
  }
  context->code = "%" + varName;

  std::cerr << "Leave Func_f_param" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitComp_units_more(CACTParser::Comp_units_moreContext *context) {
  std::cerr << "Enter Comp_units_more" << std::endl;
  context->comp_unit()->accept(this);

  context->comp_units()->accept(this);
  std::cerr << "Leave Comp_units_more" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitComp_units_empty(CACTParser::Comp_units_emptyContext *context) {
  std::cerr << "Enter Comp_units_empty" << std::endl;

  std::cerr << "Leave Comp_units_empty" << std::endl;
  return nullptr;
}

std::any
Analyzer::visitComp_unit_decl(CACTParser::Comp_unit_declContext *context) {
  std::cerr << "Enter Comp_unit_decl" << std::endl;
  context->decl()->is_global = true;
  visitChildren(context);
  std::cerr << "Leave Comp_unit_decl" << std::endl;
  return nullptr;
}

std::any Analyzer::visitComp_unit_func_def(
    CACTParser::Comp_unit_func_defContext *context) {
  std::cerr << "Enter Comp_unit_func_def" << std::endl;
  visitChildren(context);
  std::cerr << "Leave Comp_unit_func_def" << std::endl;
  return nullptr;
}