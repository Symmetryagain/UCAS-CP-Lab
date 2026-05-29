#include "analyzer.h"
#include "func_table.h"
#include "sym_table.h"
#include "context.h"
#include "ir.h"
#include "utils.h"

using namespace std;

std::any
Analyzer::visitTest(CACTParser::TestContext *context) { // will not reach here
    dbg("Always continue; Never break;");
    return nullptr;
}

std::any
Analyzer::visitNum_const_int(CACTParser::Num_const_intContext *context) {
    dbg("Enter Num_const_int");
    context->btype = Int;
    context->value = context->intconst()->getText();
    context->r_value = std::stoi(context->intconst()->getText());
    dbg("Leave Num_const_int");
    return nullptr;
}

std::any
Analyzer::visitNum_const_float(CACTParser::Num_const_floatContext *context) {
    dbg("Enter Num_const_float");
    context->btype = Float;
    context->value = context->FloatConst()->getText();
    context->r_value = std::stof(context->FloatConst()->getText());
    dbg("Leave Num_const_float");
    return nullptr;
}

std::any
Analyzer::visitNum_const_double(CACTParser::Num_const_doubleContext *context) {
    dbg("Enter Num_const_double");
    context->btype = Double;
    context->value = context->DoubleConst()->getText();
    context->r_value = std::stod(context->DoubleConst()->getText());
    dbg("Leave Num_const_double");
    return nullptr;
}

std::any
Analyzer::visitConst_num_const(CACTParser::Const_num_constContext *context) {
    dbg("Enter Const_num_const");
    context->num_const()->accept(this);
    context->btype = context->num_const()->btype;
    context->value = context->num_const()->value;
    switch(context->num_const()->btype) {
        case Int:
            context->r_value = std::get<int>(context->num_const()->r_value);
            break;
        case Float:
            context->r_value = std::get<float>(context->num_const()->r_value);
            break;
        case Double:
            context->r_value = std::get<double>(context->num_const()->r_value);
            break;
        default:
            exit(3);
    }
    dbg("Leave Const_num_const");
    return nullptr;
}

std::any
Analyzer::visitConst_bool_const(CACTParser::Const_bool_constContext *context) {
    dbg("Enter Const_bool_const");
    context->btype = Bool;
    context->value = context->boolconst()->getText();
    context->r_value = context->boolconst()->r_value;
    dbg("Leave Const_bool_const");
    return nullptr;
}

std::any 
Analyzer::visitSigned_const_const(CACTParser::Signed_const_constContext *context) {
    dbg("Enter Signed_const_const");
    context->num_const()->accept(this);
    context->btype = context->num_const()->btype;
    context->value = context->num_const()->value;
    context->sign = false;
    context->r_value = context->num_const()->r_value;
    dbg("Leave Signed_const_const");
    return nullptr;
}

std::any 
Analyzer::visitSigned_const_plus(CACTParser::Signed_const_plusContext *context) {
    dbg("Enter Signed_const_plus");
    context->signed_num_const()->accept(this);
    context->btype = context->signed_num_const()->btype;
    context->value = context->signed_num_const()->value;
    context->sign = context->signed_num_const()->sign;
    context->r_value = context->signed_num_const()->r_value;
    dbg("Leave Signed_const_plus");
    return nullptr;
}

std::any 
Analyzer::visitSigned_const_minus(CACTParser::Signed_const_minusContext *context) {
    dbg("Enter Signed_const_minus");
    context->signed_num_const()->accept(this);
    context->btype = context->signed_num_const()->btype;
    context->value = context->signed_num_const()->value;
    context->sign = !context->signed_num_const()->sign;
    switch(context->signed_num_const()->btype) {
        case Int:
            context->r_value = -std::get<int>(context->signed_num_const()->r_value);
            break;
        case Float:
            context->r_value = -std::get<float>(context->signed_num_const()->r_value);
            break;
        case Double:
            context->r_value = -std::get<double>(context->signed_num_const()->r_value);
            break;
        default:
            exit(3);
    }
    dbg("Leave Signed_const_minus");
    return nullptr;
}

std::any 
Analyzer::visitSigned_const_num_const(CACTParser::Signed_const_num_constContext *context) {
    dbg("Enter Signed_const_num_const");
    context->signed_num_const()->accept(this);
    context->btype = context->signed_num_const()->btype;
    if (context->signed_num_const()->sign)
        context->value = "-" + context->signed_num_const()->value;
    else
        context->value = context->signed_num_const()->value;
    switch(context->signed_num_const()->btype) {
        case Int:
            context->r_value = std::get<int>(context->signed_num_const()->r_value);
            break;
        case Float:
            context->r_value = std::get<float>(context->signed_num_const()->r_value);
            break;
        case Double:
            context->r_value = std::get<double>(context->signed_num_const()->r_value);
            break;
        default:
            exit(3);
    }
    dbg("Leave Signed_const_num_const");
    return nullptr;
}

std::any 
Analyzer::visitSigned_const_bool_const(CACTParser::Signed_const_bool_constContext *context) {
    dbg("Enter Signed_const_bool_const");
    context->btype = Bool;
    context->value = context->boolconst()->getText();
    context->r_value = context->boolconst()->r_value;
    dbg("Leave Signed_const_bool_const");
    return nullptr;
}

std::any 
Analyzer::visitInt_constant(CACTParser::Int_constantContext *context) {
    dbg("Enter Int_constant");

    dbg("Leave Int_constant");
    return nullptr;
}

std::any 
Analyzer::visitHex_constant(CACTParser::Hex_constantContext *context) {
    dbg("Enter Hex_constant");

    dbg("Leave Hex_constant");
    return nullptr;
}

std::any 
Analyzer::visitOct_constant(CACTParser::Oct_constantContext *context) {
    dbg("Enter Oct_constant");

    dbg("Leave Oct_constant");
    return nullptr;
}

std::any
Analyzer::visitTrue_constant(CACTParser::True_constantContext *context) {
    dbg("Enter True_constant");
    context->r_value = true;
    dbg("Leave True_constant");
    return nullptr;
}

std::any
Analyzer::visitFalse_constant(CACTParser::False_constantContext *context) {
    dbg("Enter False_constant");
    context->r_value = false;
    dbg("Leave False_constant");
    return nullptr;
}

std::any 
Analyzer::visitExpr_1_ident(CACTParser::Expr_1_identContext *context) {
    dbg("Enter Expr_1_ident");
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
    context->res = NPRE + var.name_in_IR();
    if (var.isArray())
        context->array_size = var.arraySize();
    else
        context->array_size = {};
    context->is_const = var.isConst();
    if (context->is_const) 
        context->value = var.value();
    context->code = "";
    dbg("Leave Expr_1_ident");
    return nullptr;
}

std::any
Analyzer::visitExpr_1_constant(CACTParser::Expr_1_constantContext *context) {
    dbg("Enter Expr_1_constant");
    context->const_()->accept(this);
    context->btype = context->const_()->btype;
    context->res = context->const_()->value;
    context->array_size = {};
    context->is_const = true;
    context->value = context->const_()->r_value;
    context->code = "";
    dbg("Leave Expr_1_constant");
    return nullptr;
}

std::any
Analyzer::visitExpr_1_braket(CACTParser::Expr_1_braketContext *context) {
    dbg("Enter Expr_1_braket");
    context->expr_8()->accept(this);
    context->btype = context->expr_8()->btype;
    context->res = context->expr_8()->res;
    context->array_size = context->expr_8()->array_size;
    context->is_const = context->expr_8()->is_const;
    if (context->expr_8()->is_const) {
        context->value = context->expr_8()->value;
    }
    context->code = context->expr_8()->code;
    dbg("Leave Expr_1_braket");
    return nullptr;
}

std::any
Analyzer::visitExpr_1_func_call(CACTParser::Expr_1_func_callContext *context) {
    dbg("Enter Expr_1_func_call");
    context->func_call()->accept(this);
    context->btype = context->func_call()->btype;
    context->res = context->func_call()->res;
    context->array_size = {};
    context->is_const = false;
    context->code = context->func_call()->code;
    dbg("Leave Expr_1_func_call");
    return nullptr;
}

std::any 
Analyzer::visitExpr_1_array(CACTParser::Expr_1_arrayContext *context) {
    dbg("Enter Expr_1_array");
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
    std::string offset = NPRE + createVar(Int, false);
    std::string tmp = NPRE + createVar(Int, false);
    context->code = "";
    context->code += gen(VAR, offset);
    context->code += gen(VAR, tmp);
    context->code += gen(ASSIGN, offset, "0");
    for (int i = 0; i < (int)context->expr_8().size(); ++i) {
        context->expr_8()[i]->accept(this);
        if (context->expr_8()[i]->btype != Int ||
                !context->expr_8()[i]->array_size.empty()) {
            //
            exit(3);
            assert(0);
            return nullptr;
        }
        context->code += context->expr_8()[i]->code;
        context->code += gen(MUL, btype_string[Int], tmp, context->expr_8()[i]->res, array_size[i]);
        context->code += gen(ADD, btype_string[Int], offset, offset, tmp);
    }
    std::string arr_name = NPRE + arr.name_in_IR();
    if (arr.arraySize().size() == context->expr_8().size()) { // not array
        context->array_size = {};
        std::string res = NPRE + createVar(context->btype, false);
        context->code += gen(VAR, res);
        context->code += gen(ASSIGN, res, arr_name + "[" + offset + "]");
        context->res = res;
    } else {
        for (int i = context->expr_8().size(); i < (int)arr.arraySize().size(); ++i) {
            context->array_size.push_back(arr.arraySize()[i]);
        }
        std::string res = NPRE + createVar(context->btype, true);
        context->code += gen(VAR, res);
        context->code += gen(ADDR, res, arr_name, offset);
        context->res = res;
    }
    context->is_const = false;
    dbg("Leave Expr_1_array");
    return nullptr;
}

std::any
Analyzer::visitExpr_2_expr_1(CACTParser::Expr_2_expr_1Context *context) {
    dbg("Enter Expr_2_expr_1");
    context->expr_1()->accept(this);
    context->btype = context->expr_1()->btype;
    context->array_size = context->expr_1()->array_size;
    context->res = context->expr_1()->res;
    context->code = context->expr_1()->code;
    context->is_const = context->expr_1()->is_const;
    if (context->expr_1()->is_const) {
        context->value = context->expr_1()->value;
    }
    dbg("Leave Expr_2_expr_1");
    return nullptr;
}

std::any 
Analyzer::visitExpr_2_plus(CACTParser::Expr_2_plusContext *context) {
    dbg("Enter Expr_2_plus");
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
    context->is_const = context->expr_2()->is_const;
    if (context->is_const) {
        context->value = context->expr_2()->value;
    }
    context->code = "";
    if (merge_constant && context->is_const) {
        switch(context->btype) {
            case Int: context->res = std::to_string(std::get<int>(context->value)); break;
            case Float: context->res = std::to_string(std::get<float>(context->value)); break;
            case Double: context->res = std::to_string(std::get<double>(context->value)); break;
            default: exit(3);
        }
    } else {
        std::string res = NPRE + createVar(context->btype, false);  
        context->res = res;  
        context->code += context->expr_2()->code;
        context->code += gen(VAR, res);
        context->code += gen(ASSIGN, res, context->expr_2()->res);
    }
    dbg("Leave Expr_2_plus");
    return nullptr;
}

std::any 
Analyzer::visitExpr_2_minus(CACTParser::Expr_2_minusContext *context) {
    dbg("Enter Expr_2_minus");
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
    context->is_const = context->expr_2()->is_const;
    context->code = "";
    if (context->is_const) {
        switch(context->expr_2()->btype) {
            case Int: context->value = -std::get<int>(context->expr_2()->value); break;
            case Float: context->value = -std::get<float>(context->expr_2()->value); break;
            case Double: context->value = -std::get<double>(context->expr_2()->value); break;
            default: exit(3);
        }
    }
    if (merge_constant && context->is_const) {
        switch(context->btype) {
            case Int: context->res = std::to_string(std::get<int>(context->value)); break;
            case Float: context->res = std::to_string(std::get<float>(context->value)); break;
            case Double: context->res = std::to_string(std::get<double>(context->value)); break;
            default: exit(3);
        }
    } else {
        std::string res = NPRE + createVar(context->btype, false);
        context->res = res;
        context->code += context->expr_2()->code;
        context->code += gen(VAR, res);
        context->code += gen(NEG, btype_string[context->btype], res, context->expr_2()->res);
    }
    dbg("Leave Expr_2_minus");
    return nullptr;
}

std::any 
Analyzer::visitExpr_2_not(CACTParser::Expr_2_notContext *context) {
    dbg("Enter Expr_2_not");
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
    context->is_const = context->expr_2()->is_const;
    if (context->is_const) {
        context->value = !std::get<bool>(context->expr_2()->value);
    }
    context->code = "";
    if (merge_constant && context->is_const) {
        context->res = std::get<bool>(context->value) ? "true" : "false";
        context->code = context->expr_2()->code;
    } else {
        std::string res = NPRE + createVar(Bool, false);
        context->res = res;
        context->code += context->expr_2()->code;
        context->code += gen(VAR, res);
        context->code += gen(NOT, res, context->expr_2()->res);
    }
    dbg("Leave Expr_2_not");
    return nullptr;
}

std::any
Analyzer::visitExpr_3_divide(CACTParser::Expr_3_divideContext *context) {
    dbg("Enter Expr_3_divide");
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
    context->is_const = context->expr_3()->is_const && context->expr_2()->is_const;
    if (context->is_const) {
        switch(context->btype) {
            case Int:
                if (std::get<int>(context->expr_2()->value) == 0) {
                    dbg("warning: division by zero [-Wdiv-by-zero]");
                    context->is_const = false;
                } else {
                    context->value = std::get<int>(context->expr_3()->value) / std::get<int>(context->expr_2()->value);
                }
                break;
            case Float: context->value = std::get<float>(context->expr_3()->value) / std::get<float>(context->expr_2()->value); break;
            case Double: context->value = std::get<double>(context->expr_3()->value) / std::get<double>(context->expr_2()->value); break;
            default: exit(3);
        }
    }
    context->code = "";
    if (merge_constant && context->is_const) {
        switch(context->btype) {
            case Int: context->res = std::to_string(std::get<int>(context->value)); break;
            case Float: context->res = std::to_string(std::get<float>(context->value)); break;
            case Double: context->res = std::to_string(std::get<double>(context->value)); break;
            default: exit(3);
        }
    } else {
        std::string res = NPRE + createVar(context->btype, false);
        context->res = res;
        context->code += context->expr_3()->code;
        context->code += context->expr_2()->code;
        context->code += gen(VAR, res);
        context->code += gen(DIV, btype_string[context->btype], res, context->expr_3()->res, context->expr_2()->res);
    }
    dbg("Leave Expr_3_divide");
    return nullptr;
}

std::any 
Analyzer::visitExpr_3_times(CACTParser::Expr_3_timesContext *context) {
    dbg("Enter Expr_3_times");
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
    context->is_const = context->expr_3()->is_const && context->expr_2()->is_const;
    context->code = "";
    if (context->is_const) {
        switch(context->btype) {
            case Int: context->value = std::get<int>(context->expr_3()->value) * std::get<int>(context->expr_2()->value); break;
            case Float: context->value = std::get<float>(context->expr_3()->value) * std::get<float>(context->expr_2()->value); break;
            case Double: context->value = std::get<double>(context->expr_3()->value) * std::get<double>(context->expr_2()->value); break;
            default: exit(3);
        }
    }
    if (merge_constant && context->is_const) {
        switch(context->btype) {
            case Int: context->res = std::to_string(std::get<int>(context->value)); break;
            case Float: context->res = std::to_string(std::get<float>(context->value)); break;
            case Double: context->res = std::to_string(std::get<double>(context->value)); break;
            default: exit(3);
        }
    } else {
        std::string res = NPRE + createVar(context->btype, false);
        context->res = res;
        context->code += context->expr_3()->code;
        context->code += context->expr_2()->code;
        context->code += gen(VAR, res);
        context->code += gen(MUL, btype_string[context->btype], res, context->expr_3()->res, context->expr_2()->res);
    }
    dbg("Leave Expr_3_times");
    return nullptr;
}

std::any
Analyzer::visitExpr_3_remain(CACTParser::Expr_3_remainContext *context) {
    dbg("Enter Expr_3_remain");
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
    context->is_const = context->expr_3()->is_const && context->expr_2()->is_const;
    if (context->is_const) {
        if (std::get<int>(context->expr_2()->value) == 0) {
            std::cerr << "warning: division by zero [-Wdiv-by-zero]" << std::endl;
            context->is_const = false;
        } else {
            context->value = std::get<int>(context->expr_3()->value) % std::get<int>(context->expr_2()->value);
        }
    }
    context->code = "";
    if (merge_constant && context->is_const) {
        context->res = std::to_string(std::get<int>(context->value));
    } else {
        std::string res = NPRE + createVar(Int, false);
        context->res = res;
        context->code += context->expr_3()->code;
        context->code += context->expr_2()->code;
        context->code += gen(VAR, res);
        context->code += gen(REM, res, context->expr_3()->res, context->expr_2()->res);
    }
    dbg("Leave Expr_3_remain");
    return nullptr;
}

std::any
Analyzer::visitExpr_3_expr_2(CACTParser::Expr_3_expr_2Context *context) {
    dbg("Enter Expr_3_expr_2");
    context->expr_2()->accept(this);
    context->btype = context->expr_2()->btype;
    context->array_size = context->expr_2()->array_size;
    context->res = context->expr_2()->res;
    context->is_const = context->expr_2()->is_const;
    if (context->expr_2()->is_const) {
        context->value = context->expr_2()->value;
    }
    context->code = context->expr_2()->code;
    dbg("Leave Expr_3_expr_2");
    return nullptr;
}

std::any 
Analyzer::visitExpr_4_plus(CACTParser::Expr_4_plusContext *context) {
    dbg("Enter Expr_4_plus");
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
    context->is_const = context->expr_4()->is_const && context->expr_3()->is_const;
    if (context->is_const) {
        switch(context->btype) {
            case Int: context->value = std::get<int>(context->expr_4()->value) + std::get<int>(context->expr_3()->value); break;
            case Float: context->value = std::get<float>(context->expr_4()->value) + std::get<float>(context->expr_3()->value); break;
            case Double: context->value = std::get<double>(context->expr_4()->value) + std::get<double>(context->expr_3()->value); break;
            default: exit(3);
        }
    }
    context->code = "";
    if (merge_constant && context->is_const) {
        switch(context->btype) {
            case Int: context->res = std::to_string(std::get<int>(context->value)); break;
            case Float: context->res = std::to_string(std::get<float>(context->value)); break;
            case Double: context->res = std::to_string(std::get<double>(context->value)); break;
            default: exit(3);
        }
    } else {
        std::string res = NPRE + createVar(context->btype, false);
        context->res = res;
        context->code += context->expr_4()->code;
        context->code += context->expr_3()->code;
        context->code += gen(VAR, res);
        context->code += gen(ADD, btype_string[context->btype], res, context->expr_4()->res, context->expr_3()->res);
    }
    dbg("Leave Expr_4_plus");
    return nullptr;
}

std::any
Analyzer::visitExpr_4_expr_3(CACTParser::Expr_4_expr_3Context *context) {
    dbg("Enter Expr_4_expr_3");
    context->expr_3()->accept(this);
    context->btype = context->expr_3()->btype;
    context->array_size = context->expr_3()->array_size;
    context->res = context->expr_3()->res;
    context->is_const = context->expr_3()->is_const;
    if (context->expr_3()->is_const) {
        context->value = context->expr_3()->value;
    }
    context->code = context->expr_3()->code;
    dbg("Leave Expr_4_expr_3");
    return nullptr;
}

std::any 
Analyzer::visitExpr_4_minus(CACTParser::Expr_4_minusContext *context) {
    dbg("Enter Expr_4_minus");
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
    context->is_const = context->expr_4()->is_const && context->expr_3()->is_const;
    if (context->is_const) {
        switch(context->btype) {
            case Int: context->value = std::get<int>(context->expr_4()->value) - std::get<int>(context->expr_3()->value); break;
            case Float: context->value = std::get<float>(context->expr_4()->value) - std::get<float>(context->expr_3()->value); break;
            case Double: context->value = std::get<double>(context->expr_4()->value) - std::get<double>(context->expr_3()->value); break;
            default: exit(3);
        }
    }
    context->code = "";
    if (merge_constant && context->is_const) {
        switch(context->btype) {
            case Int: context->res = std::to_string(std::get<int>(context->value)); break;
            case Float: context->res = std::to_string(std::get<float>(context->value)); break;
            case Double: context->res = std::to_string(std::get<double>(context->value)); break;
            default: exit(3);
        }
    } else {
        std::string res = NPRE + createVar(context->btype, false);
        context->res = res;
        context->code += context->expr_4()->code;
        context->code += context->expr_3()->code;
        context->code += gen(VAR, res);
        context->code += gen(SUB, btype_string[context->btype], res, context->expr_4()->res, context->expr_3()->res);
    }
    dbg("Leave Expr_4_minus");
    return nullptr;
}

std::any 
Analyzer::visitExpr_5_ge(CACTParser::Expr_5_geContext *context) {
    dbg("Enter Expr_5_ge");
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
    context->is_const = context->expr_5()->is_const && context->expr_4()->is_const;
    if (context->is_const) {
        switch(context->expr_5()->btype) {
            case Int: context->value = std::get<int>(context->expr_5()->value) >= std::get<int>(context->expr_4()->value); break;
            case Float: context->value = std::get<float>(context->expr_5()->value) >= std::get<float>(context->expr_4()->value); break;
            case Double: context->value = std::get<double>(context->expr_5()->value) >= std::get<double>(context->expr_4()->value); break;
            default: exit(3);
        }
    }
    context->code = "";
    if (merge_constant && context->is_const) {
        context->res = std::get<bool>(context->value) ? "true" : "false";
    } else {
        std::string res = NPRE + createVar(Bool, false);
        context->res = res;
        context->code += context->expr_5()->code;
        context->code += context->expr_4()->code;
        context->code += gen(VAR, res);
        context->code += gen(GE, btype_string[context->expr_4()->btype], res, context->expr_5()->res, context->expr_4()->res);
    }
    dbg("Leave Expr_5_ge");
    return nullptr;
}

std::any 
Analyzer::visitExpr_5_gt(CACTParser::Expr_5_gtContext *context) {
    dbg("Enter Expr_5_gt");
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
    context->is_const = context->expr_5()->is_const && context->expr_4()->is_const;
    if (context->is_const) {
        switch(context->expr_5()->btype) {
            case Int: context->value = std::get<int>(context->expr_5()->value) > std::get<int>(context->expr_4()->value); break;
            case Float: context->value = std::get<float>(context->expr_5()->value) > std::get<float>(context->expr_4()->value); break;
            case Double: context->value = std::get<double>(context->expr_5()->value) > std::get<double>(context->expr_4()->value); break;
            default: exit(3);
        }
    }
    context->code = "";
    if (merge_constant && context->is_const) {
        context->res = std::get<bool>(context->value) ? "true" : "false";
    } else {
        std::string res = NPRE + createVar(Bool, false);
        context->res = res;
        context->code += context->expr_5()->code;
        context->code += context->expr_4()->code;
        context->code += gen(VAR, res);
        context->code += gen(GT, btype_string[context->expr_4()->btype], res, context->expr_5()->res, context->expr_4()->res);
    }
    dbg("Leave Expr_5_gt");
    return nullptr;
}

std::any
Analyzer::visitExpr_5_expr_4(CACTParser::Expr_5_expr_4Context *context) {
    dbg("Enter Expr_5_expr_4");
    context->expr_4()->accept(this);
    context->btype = context->expr_4()->btype;
    context->array_size = context->expr_4()->array_size;
    context->res = context->expr_4()->res;
    context->is_const = context->expr_4()->is_const;
    if (context->expr_4()->is_const) {
        context->value = context->expr_4()->value;
    }
    context->code = context->expr_4()->code;
    dbg("Leave Expr_5_expr_4");
    return nullptr;
}

std::any 
Analyzer::visitExpr_5_le(CACTParser::Expr_5_leContext *context) {
    dbg("Enter Expr_5_le");
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
    context->is_const = context->expr_5()->is_const && context->expr_4()->is_const;
    if (context->is_const) {
        switch(context->expr_5()->btype) {
            case Int: context->value = std::get<int>(context->expr_5()->value) <= std::get<int>(context->expr_4()->value); break;
            case Float: context->value = std::get<float>(context->expr_5()->value) <= std::get<float>(context->expr_4()->value); break;
            case Double: context->value = std::get<double>(context->expr_5()->value) <= std::get<double>(context->expr_4()->value); break;
            default: exit(3);
        }
    }
    context->code = "";
    if (merge_constant && context->is_const) {
        context->res = std::get<bool>(context->value) ? "true" : "false";
    } else {
        std::string res = NPRE + createVar(Bool, false);
        context->res = res;
        context->code += context->expr_5()->code;
        context->code += context->expr_4()->code;
        context->code += gen(VAR, res);
        context->code += gen(LE, btype_string[context->expr_4()->btype], res, context->expr_5()->res, context->expr_4()->res);
    }
    dbg("Leave Expr_5_le");
    return nullptr;
}

std::any 
Analyzer::visitExpr_5_lt(CACTParser::Expr_5_ltContext *context) {
    dbg("Enter Expr_5_lt");
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
    context->is_const = context->expr_5()->is_const && context->expr_4()->is_const;
    if (context->is_const) {
        switch(context->expr_5()->btype) {
            case Int: context->value = std::get<int>(context->expr_5()->value) < std::get<int>(context->expr_4()->value); break;
            case Float: context->value = std::get<float>(context->expr_5()->value) < std::get<float>(context->expr_4()->value); break;
            case Double: context->value = std::get<double>(context->expr_5()->value) < std::get<double>(context->expr_4()->value); break;
            default: exit(3);
        }
    }
    context->code = "";
    if (merge_constant && context->is_const) {
        context->res = std::get<bool>(context->value) ? "true" : "false";
    } else {
        std::string res = NPRE + createVar(Bool, false);
        context->res = res;
        context->code += context->expr_5()->code;
        context->code += context->expr_4()->code;
        context->code += gen(VAR, res);
        context->code += gen(LT, btype_string[context->expr_4()->btype], res, context->expr_5()->res, context->expr_4()->res);
    }
    dbg("Leave Expr_5_lt");
    return nullptr;
}

std::any
Analyzer::visitExpr_6_expr_5(CACTParser::Expr_6_expr_5Context *context) {
    dbg("Enter Expr_6_expr_5");
    context->expr_5()->accept(this);
    context->btype = context->expr_5()->btype;
    context->array_size = context->expr_5()->array_size;
    context->res = context->expr_5()->res;
    context->is_const = context->expr_5()->is_const;
    if (context->expr_5()->is_const) {
        context->value = context->expr_5()->value;
    }
    context->code = context->expr_5()->code;
    dbg("Leave Expr_6_expr_5");
    return nullptr;
}

std::any 
Analyzer::visitExpr_6_ne(CACTParser::Expr_6_neContext *context) {
    dbg("Enter Expr_6_ne");
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
    context->is_const = context->expr_6()->is_const && context->expr_5()->is_const;
    if (context->is_const) {
        switch(context->expr_6()->btype) {
            case Int: context->value = std::get<int>(context->expr_6()->value) != std::get<int>(context->expr_5()->value); break;
            case Float: context->value = std::get<float>(context->expr_6()->value) != std::get<float>(context->expr_5()->value); break;
            case Double: context->value = std::get<double>(context->expr_6()->value) != std::get<double>(context->expr_5()->value); break;
            case Bool: context->value = std::get<bool>(context->expr_6()->value) != std::get<bool>(context->expr_5()->value); break;
            default: exit(3);
        }
    }
    context->code = "";
    if (merge_constant && context->is_const) {
        context->res = std::get<bool>(context->value) ? "true" : "false";
    } else {
        std::string res = NPRE + createVar(Bool, false);
        context->res = res;
        context->code += context->expr_6()->code;
        context->code += context->expr_5()->code;
        context->code += gen(VAR, res);
        context->code += gen(NE, btype_string[context->expr_5()->btype], res, context->expr_6()->res, context->expr_5()->res);
    }
    dbg("Leave Expr_6_ne");
    return nullptr;
}

std::any 
Analyzer::visitExpr_6_eq(CACTParser::Expr_6_eqContext *context) {
    dbg("Enter Expr_6_eq");
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
    context->is_const = context->expr_6()->is_const && context->expr_5()->is_const;
    if (context->is_const) {
        switch(context->expr_6()->btype) {
            case Int: context->value = std::get<int>(context->expr_6()->value) == std::get<int>(context->expr_5()->value); break;
            case Float: context->value = std::get<float>(context->expr_6()->value) == std::get<float>(context->expr_5()->value); break;
            case Double: context->value = std::get<double>(context->expr_6()->value) == std::get<double>(context->expr_5()->value); break;
            case Bool: context->value = std::get<bool>(context->expr_6()->value) == std::get<bool>(context->expr_5()->value); break;
            default: exit(3);
        }
    }
    context->code = "";
    if (merge_constant && context->is_const) {
        context->res = std::get<bool>(context->value) ? "true" : "false";
    } else {
        std::string res = NPRE + createVar(Bool, false);
        context->res = res;
        context->code += context->expr_6()->code;
        context->code += context->expr_5()->code;
        context->code += gen(VAR, res);
        context->code += gen(EQ, btype_string[context->expr_5()->btype], res, context->expr_6()->res, context->expr_5()->res);
    }
    dbg("Leave Expr_6_eq");
    return nullptr;
}

std::any
Analyzer::visitExpr_7_expr_6(CACTParser::Expr_7_expr_6Context *context) {
    dbg("Enter Expr_7_expr_6");
    context->expr_6()->accept(this);
    context->btype = context->expr_6()->btype;
    context->array_size = context->expr_6()->array_size;
    context->res = context->expr_6()->res;
    context->is_const = context->expr_6()->is_const;
    if (context->expr_6()->is_const) {
        context->value = context->expr_6()->value;
    }
    context->code = context->expr_6()->code;
    dbg("Leave Expr_7_expr_6");
    return nullptr;
}

std::any 
Analyzer::visitExpr_7_dand(CACTParser::Expr_7_dandContext *context) {
    dbg("Enter Expr_7_dand");
    context->expr_7()->accept(this);
    if (context->expr_7()->btype != Bool || !context->expr_7()->array_size.empty()) {
        //
        exit(3);
        assert(0);
        return nullptr;
    }
    context->expr_6()->accept(this);
    if (context->expr_6()->btype != Bool || !context->expr_6()->array_size.empty()) {
        //
        exit(3);
        assert(0);
        return nullptr;
    }
    context->btype = Bool;
    context->array_size = {};
    context->is_const = context->expr_7()->is_const && context->expr_6()->is_const;
    if (context->is_const) {
        context->value = std::get<bool>(context->expr_7()->value) && std::get<bool>(context->expr_6()->value);
    }
    if (merge_constant && context->is_const) {
        context->res = std::get<bool>(context->value) ? "true" : "false";
    } else {
        std::string res = NPRE + createVar(Bool, false);
        context->res = res;
        std::string f_label = createLabel();
        std::string nres = NPRE + createVar(Bool, false);
        context->code = "";
        context->code += context->expr_7()->code;
        context->code += gen(VAR, res);
        context->code += gen(ASSIGN, res, context->expr_7()->res);
        context->code += gen(VAR, nres);
        context->code += gen(NOT, nres, context->expr_7()->res);
        context->code += gen(BRANCH, f_label, nres);
        context->code += context->expr_6()->code;
        context->code += gen(ASSIGN, res, context->expr_6()->res);
        context->code += gen(LABEL, f_label);
    }
    dbg("Leave Expr_7_dand");
    return nullptr;
}

std::any 
Analyzer::visitExpr_8_dor(CACTParser::Expr_8_dorContext *context) {
    dbg("Enter Expr_8_dor");
    context->expr_8()->accept(this);
    if (context->expr_8()->btype != Bool || !context->expr_8()->array_size.empty()) {
        //
        exit(3);
        assert(0);
        return nullptr;
    }
    context->expr_7()->accept(this);
    if (context->expr_7()->btype != Bool || !context->expr_7()->array_size.empty()) {
        //
        exit(3);
        assert(0);
        return nullptr;
    }
    context->btype = Bool;
    context->array_size = {};
    context->is_const = context->expr_8()->is_const && context->expr_7()->is_const;
    if (context->is_const) {
        context->value = std::get<bool>(context->expr_8()->value) || std::get<bool>(context->expr_7()->value);
    }
    if (merge_constant && context->is_const) {
        context->res = std::get<bool>(context->value) ? "true" : "false";
    } else {
        std::string res = NPRE + createVar(Bool, false);
        context->res = res;
        std::string t_label = createLabel();
        context->code = "";
        context->code += context->expr_8()->code;
        context->code += gen(VAR, res);
        context->code += gen(ASSIGN, res, context->expr_8()->res);
        context->code += gen(BRANCH, t_label, context->expr_8()->res);
        context->code += context->expr_7()->code;
        context->code += gen(ASSIGN, res, context->expr_7()->res);
        context->code += gen(LABEL, t_label);
    }
    dbg("Leave Expr_8_dor");
    return nullptr;
}

std::any
Analyzer::visitExpr_8_expr_7(CACTParser::Expr_8_expr_7Context *context) {
    dbg("Enter Expr_8_expr_7");
    context->expr_7()->accept(this);
    context->btype = context->expr_7()->btype;
    context->array_size = context->expr_7()->array_size;
    context->res = context->expr_7()->res;
    context->is_const = context->expr_7()->is_const;
    if (context->expr_7()->is_const) {
        context->value = context->expr_7()->value;
    }
    context->code = context->expr_7()->code;
    dbg("Leave Expr_8_expr_7");
    return nullptr;
}

std::any
Analyzer::visitFunc_call_ident(CACTParser::Func_call_identContext *context) {
    dbg("Enter Func_call_ident");
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
    context->code = "";
    if (func.retvalType() == Void) {
        func_call_Res = VOID;
    } else {
        func_call_Res = NPRE + createVar(func.retvalType(), false);
        context->res = func_call_Res;
        context->code += gen(VAR, func_call_Res);
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
        context->code += context->expr_8()[i]->code;
    }
    context->code += "call " + func_call_Res + " %" + func.name_in_IR() + " ( ";
    for (int i = 0; i < (int)context->expr_8().size(); ++i) {
        context->code += context->expr_8()[i]->res + " ";
        if (i + 1 < (int)context->expr_8().size()) context->code += ", ";
    }
    context->code += ")\n";
    dbg("Leave Func_call_ident");
    return nullptr;
}

std::any 
Analyzer::visitType_int(CACTParser::Type_intContext *context) {
    dbg("Enter Type_int");

    dbg("Leave Type_int");
    return nullptr;
}

std::any 
Analyzer::visitType_bool(CACTParser::Type_boolContext *context) {
    dbg("Enter Type_bool");

    dbg("Leave Type_bool");
    return nullptr;
}

std::any 
Analyzer::visitType_float(CACTParser::Type_floatContext *context) {
    dbg("Enter Type_float");

    dbg("Leave Type_float");
    return nullptr;
}

std::any 
Analyzer::visitType_double(CACTParser::Type_doubleContext *context) {
    dbg("Enter Type_double");

    dbg("Leave Type_double");
    return nullptr;
}

std::any 
Analyzer::visitType_void(CACTParser::Type_voidContext *context) {
    dbg("Enter Type_void");

    dbg("Leave Type_void");
    return nullptr;
}

std::any 
Analyzer::visitStmt_assign(CACTParser::Stmt_assignContext *context) {
    dbg("Enter Stmt_assign");
    context->l_value()->accept(this);
    context->expr_8()->accept(this);
    if (context->expr_8()->btype != context->l_value()->btype) {
        //
        exit(3);
        assert(0);
        return nullptr;
    }
    out(context->expr_8()->code);
    out(ASSIGN, context->l_value()->res, context->expr_8()->res);
    dbg("Leave Stmt_assign");
    return nullptr;
}

std::any 
Analyzer::visitL_value(CACTParser::L_valueContext *context) {
    dbg("Enter L_value");
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
        out(context->expr_8()[i]->code);
    }
    context->btype = var.bType();
    if (var.isArray()) {
        std::string offset = NPRE + createVar(Int, false);
        std::string tmp = NPRE + createVar(Int, false);
        out(VAR, offset);
        out(VAR, tmp);
        out(ASSIGN, offset, context->expr_8().back()->res);
        int scale = var.arraySize().back();
        for (int i = (int)var.arraySize().size() - 2; i >= 0; --i) {
            out(MUL, btype_string[Int], tmp, context->expr_8()[i]->res, scale);
            out(ADD, btype_string[Int], offset, offset, tmp);
            scale *= var.arraySize()[i];
        }
        context->res = NPRE + var.name_in_IR() + "[" + offset + "]";
    } else {
        context->res = NPRE + var.name_in_IR();
    }
    dbg("Leave L_value");
    return nullptr;
}

std::any 
Analyzer::visitStmt_expr(CACTParser::Stmt_exprContext *context) {
    dbg("Enter Stmt_expr");
    if (context->expr_8() != nullptr) {
        context->expr_8()->accept(this);
        out(context->expr_8()->code);
    }
    dbg("Leave Stmt_expr");
    return nullptr;
}

std::any 
Analyzer::visitStmt_block(CACTParser::Stmt_blockContext *context) {
    dbg("Enter Stmt_block");
    context->block()->need_type = context->need_type;
    context->block()->brk_target = context->brk_target;
    context->block()->ctn_target = context->ctn_target;
    context->block()->has_return = false;
    g_symtree.enterScope();
    context->block()->accept(this);
    g_symtree.leaveScope();
    if (context->block()->has_return)
        context->has_return = true;
    dbg("Leave Stmt_block");
    return nullptr;
}

std::any 
Analyzer::visitStmt_return(CACTParser::Stmt_returnContext *context) {
    dbg("Enter Stmt_return");
    if (context->need_type == Void) {
        if (context->expr_8() != nullptr) {
            //
            exit(3);
            assert(0);
            return nullptr;
        }
        out(RETURN);
    } else {
        context->expr_8()->accept(this);
        if (context->expr_8()->btype != context->need_type) {
            //
            exit(3);
            assert(0);
            return nullptr;
        }
        out(context->expr_8()->code);
        out(RETURN, context->expr_8()->res);
    }
    dbg("Leave Stmt_return");
    return nullptr;
}

std::any 
Analyzer::visitStmt_if(CACTParser::Stmt_ifContext *context) {
    dbg("Enter Stmt_if");
    context->expr_8()->accept(this);
    if (context->expr_8()->btype != Bool) {
        //
        exit(3);
        assert(0);
        return nullptr;
    }
    out(context->expr_8()->code);
    std::string ncond = NPRE + createVar(Bool, false);
    out(VAR, ncond);
    out(NOT, ncond, context->expr_8()->res);
    std::string leaveLabel = createLabel();
    if (context->stmt().size() > 1) { // if-else
        std::string elseLabel = createLabel();
        out(BRANCH, elseLabel, ncond);
        bool has_return = true;
        for (size_t i = 0; i < context->stmt().size(); ++i) {
            if (i == 1)
                out(LABEL, elseLabel);
            context->stmt()[i]->brk_target = context->brk_target;
            context->stmt()[i]->ctn_target = context->ctn_target;
            context->stmt()[i]->need_type = context->need_type;
            context->stmt()[i]->has_return = false;
            context->stmt()[i]->accept(this);
            if (!context->stmt()[i]->has_return)
                has_return = false;
            if (i == 0)
                out(BRANCH, leaveLabel, "true");
        }
        context->has_return = has_return;
    } else { // if only
        out(BRANCH, leaveLabel, ncond);
        context->stmt()[0]->brk_target = context->brk_target;
        context->stmt()[0]->ctn_target = context->ctn_target;
        context->stmt()[0]->need_type = context->need_type;
        context->stmt()[0]->has_return = false;
        context->stmt()[0]->accept(this);
    }
    out(LABEL, leaveLabel);
    dbg("Leave Stmt_if");
    return nullptr;
}

std::any 
Analyzer::visitStmt_while(CACTParser::Stmt_whileContext *context) {
    dbg("Enter Stmt_while");
    std::string enterLabel = createLabel();
    std::string leaveLabel = createLabel();
    out(LABEL, enterLabel);
    context->expr_8()->accept(this);
    if (context->expr_8()->btype != Bool) {
        //
        exit(3);
        assert(0);
        return nullptr;
    }
    out(context->expr_8()->code);
    std::string ncond = NPRE + createVar(Bool, false);
    out(VAR, ncond);
    out(NOT, ncond, context->expr_8()->res);
    out(BRANCH, leaveLabel, ncond);
    context->stmt()->ctn_target = enterLabel;
    context->stmt()->brk_target = leaveLabel;
    context->stmt()->need_type = context->need_type;
    context->stmt()->has_return = false;
    context->stmt()->accept(this);
    out(BRANCH, enterLabel, "true");
    out(LABEL, leaveLabel);
    dbg("Leave Stmt_while");
    return nullptr;
}

std::any 
Analyzer::visitStmt_break(CACTParser::Stmt_breakContext *context) {
    dbg("Enter Stmt_break");
    if (context->brk_target.empty()) {
        //
        exit(3);
        assert(0);
        return nullptr;
    }
    out(BRANCH, context->brk_target, "true");
    dbg("Leave Stmt_break");
    return nullptr;
}

std::any
Analyzer::visitStmt_continue(CACTParser::Stmt_continueContext *context) {
    dbg("Enter Stmt_continue");
    if (context->ctn_target.empty()) {
        //
        exit(3);
        assert(0);
        return nullptr;
    }
    out(BRANCH, context->ctn_target, "true");
    dbg("Leave Stmt_continue");
    return nullptr;
}

std::any 
Analyzer::visitAssign_stmt(CACTParser::Assign_stmtContext *context) {
    dbg("Enter Assign_stmt");
    context->stmt_assign()->accept(this);
    dbg("Leave Assign_stmt");
    return nullptr;
}

std::any 
Analyzer::visitExpr_stmt(CACTParser::Expr_stmtContext *context) {
    dbg("Enter Expr_stmt");
    context->stmt_expr()->accept(this);
    dbg("Leave Expr_stmt");
    return nullptr;
}

std::any 
Analyzer::visitBlock_stmt(CACTParser::Block_stmtContext *context) {
    dbg("Enter Block_stmt");
    context->stmt_block()->need_type = context->need_type;
    context->stmt_block()->brk_target = context->brk_target;
    context->stmt_block()->ctn_target = context->ctn_target;
    context->stmt_block()->has_return = false;
    context->stmt_block()->accept(this);
    if (context->stmt_block()->has_return) {
        context->has_return = true;
    }
    dbg("Leave Block_stmt");
    return nullptr;
}

std::any 
Analyzer::visitReturn_stmt(CACTParser::Return_stmtContext *context) {
    dbg("Enter Return_stmt");
    context->stmt_return()->need_type = context->need_type;
    context->stmt_return()->accept(this);
    context->has_return = true;
    dbg("Leave Return_stmt");
    return nullptr;
}

std::any 
Analyzer::visitIf_stmt(CACTParser::If_stmtContext *context) {
    dbg("Enter If_stmt");
    context->stmt_if()->need_type = context->need_type;
    context->stmt_if()->brk_target = context->brk_target;
    context->stmt_if()->ctn_target = context->ctn_target;
    context->stmt_if()->has_return = false;
    context->stmt_if()->accept(this);
    if (context->stmt_if()->has_return) {
        context->has_return = true;
    }
    dbg("Leave If_stmt");
    return nullptr;
}

std::any 
Analyzer::visitWhile_stmt(CACTParser::While_stmtContext *context) {
    dbg("Enter While_stmt");
    context->stmt_while()->need_type = context->need_type;
    context->stmt_while()->brk_target = context->brk_target;
    context->stmt_while()->ctn_target = context->ctn_target;
    context->stmt_while()->has_return = false;
    context->stmt_while()->accept(this);
    dbg("Leave While_stmt");
    return nullptr;
}

std::any 
Analyzer::visitBreak_stmt(CACTParser::Break_stmtContext *context) {
    dbg("Enter Break_stmt");
    context->stmt_break()->brk_target = context->brk_target;
    context->stmt_break()->accept(this);
    dbg("Leave Break_stmt");
    return nullptr;
}

std::any
Analyzer::visitContinue_stmt(CACTParser::Continue_stmtContext *context) {
    dbg("Enter Continue_stmt");
    context->stmt_continue()->ctn_target = context->ctn_target;
    context->stmt_continue()->accept(this);
    dbg("Leave Continue_stmt");
    return nullptr;
}

std::any 
Analyzer::visitDecl_var(CACTParser::Decl_varContext *context) {
    dbg("Enter Decl_var");
    Btype btype = str_to_type(context->type()->getText());
    for (int i = 0; i < (int)context->var_def().size(); ++i) {
        context->var_def()[i]->is_global = context->is_global;
        context->var_def()[i]->need_type = btype;
        context->var_def()[i]->accept(this);
    }
    dbg("Leave Decl_var");
    return nullptr;
}

std::any 
Analyzer::visitDecl_const(CACTParser::Decl_constContext *context) {
    dbg("Enter Decl_const");
    Btype btype = str_to_type(context->type()->getText());
    for (int i = 0; i < (int)context->const_def().size(); ++i) {
        context->const_def()[i]->is_global = context->is_global;
        context->const_def()[i]->need_type = btype;
        context->const_def()[i]->accept(this);
    }
    dbg("Leave Decl_const");
    return nullptr;
}

std::any 
Analyzer::visitVar_decl(CACTParser::Var_declContext *context) {
    dbg("Enter Var_decl");
    context->decl_var()->is_global = context->is_global;
    context->decl_var()->accept(this);
    dbg("Leave Var_decl");
    return nullptr;
}

std::any 
Analyzer::visitConst_decl(CACTParser::Const_declContext *context) {
    dbg("Enter Const_decl");
    context->decl_const()->is_global = context->is_global;
    context->decl_const()->accept(this);
    dbg("Leave Const_decl");
    return nullptr;
}

std::any
Analyzer::visitBlock_item_decl(CACTParser::Block_item_declContext *context) {
    dbg("Enter Block_item_decl");
    context->decl()->is_global = false;
    context->decl()->accept(this);
    dbg("Leave Block_item_decl");
    return nullptr;
}

std::any
Analyzer::visitBlock_item_stmt(CACTParser::Block_item_stmtContext *context) {
    dbg("Enter Block_item_stmt");
    context->stmt()->need_type = context->need_type;
    context->stmt()->brk_target = context->brk_target;
    context->stmt()->ctn_target = context->ctn_target;
    context->stmt()->has_return = false;
    context->stmt()->accept(this);
    if (context->stmt()->has_return)
        context->has_return = true;
    dbg("Leave Block_item_stmt");
    return nullptr;
}

std::any 
Analyzer::visitBlock(CACTParser::BlockContext *context) {
    dbg("Enter Block");
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
    dbg("Leave Block");
    return nullptr;
}

std::any 
Analyzer::visitVar_def(CACTParser::Var_defContext *context) {
    dbg("Enter Var_def");
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
    std::string res;
    if (context->intconst().empty()) { // variant
        if (!g_symtree.define(context->need_type, name, false, false, {})) {
            //
            exit(3);
            assert(0);
            return nullptr;
        }
        res = NPRE + name;
        if (context->is_global)
            out(GLOBAL, VAR, res);
        else
            out(VAR, res);
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
        res = NPRE + name;
        if (context->is_global)
            out(GLOBAL, ARRAY, res, std::to_string(total_size));
        else
            out(ARRAY, res, std::to_string(total_size));
    }

    if (context->array_signed_const() != nullptr) { // has value
        context->array_signed_const()->need_type = context->need_type;
        context->array_signed_const()->array_size = array_size;
        context->array_signed_const()->varName = name;
        context->array_signed_const()->offset = 0;
        context->array_signed_const()->at_top = true;
        context->array_signed_const()->is_global = context->is_global;
        context->array_signed_const()->is_global_const = false;
        context->array_signed_const()->accept(this);
    } else {                             // no initializer
        if (!context->is_global) {         // globals default to zero (bss)
            if (context->intconst().empty()) { // scalar
                out(ASSIGN, res, "0");
            } else { // array
                for (int i = 0; i < total_size; ++i) {
                    out(ASSIGN, res + "[" + std::to_string(i) + "]", "0");
                }
            }
        }
    }
    dbg("Leave Var_def");
    return nullptr;
}

std::any 
Analyzer::visitConst_def(CACTParser::Const_defContext *context) {
    dbg("Enter Const_def");
    std::string name = context->Ident()->getText();
    if (g_symtree.check(name)) {
        //
        exit(3);
        assert(0);
        return nullptr;
    }
    std::vector<size_t> array_size;
    array_size.clear();
    std::string res;
    std::string save_name = name;
    if (context->intconst().empty()) { // variant
        if (!g_symtree.define(context->need_type, name, false, true, {})) {
            //
            exit(3);
            assert(0);
            return nullptr;
        }
        res = NPRE + name;
        if (context->is_global)
            out(GLOBAL, "const", VAR, res);
        else
            out(VAR, res);
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
        res = NPRE + name;
        if (context->is_global)
            out(GLOBAL, "const", ARRAY, res, total_size);
        else
            out(ARRAY, res, total_size);
    }
    context->array_signed_const()->need_type = context->need_type;
    context->array_signed_const()->array_size = array_size;
    context->array_signed_const()->varName = name;
    context->array_signed_const()->offset = 0;
    context->array_signed_const()->at_top = true;
    context->array_signed_const()->is_global = context->is_global;
    context->array_signed_const()->is_global_const = true;
    context->array_signed_const()->accept(this);
    if (context->intconst().empty()) { // variant
        g_symtree.modify_value(save_name, context->array_signed_const()->r_value);
    }
    dbg("Leave Const_def");
    return nullptr;
}

std::any 
Analyzer::visitArray_signed_const_const(CACTParser::Array_signed_const_constContext *context) {
    dbg("Enter Array_signed_const_const");
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
    std::string res = NPRE + context->varName;
    // For non-const globals, skip zero-valued assigns: bss provides zero-init.
    // Const globals must emit all assigns to land in .rodata.
    bool is_init_zero = false;
    if (context->is_global && !context->is_global_const) {
        const auto &val = context->signed_const()->value;
        switch (context->signed_const()->btype) {
            case Bool:   is_init_zero = (val == "false"); break;
            case Int:    is_init_zero = (std::stoi(val) == 0); break;
            case Float:  is_init_zero = (std::stof(val) == 0.0f); break;
            case Double: is_init_zero = (std::stod(val) == 0.0); break;
            default: break;
        }
    }
    if (is_init_zero) {
        // skip: zero is the default
    } else if (context->varName[0] == 'a') {
        if (context->is_global)
            out(GLOBAL, ASSIGN, res + "[" + std::to_string(context->offset) + "]", context->signed_const()->value);
        else
            out(ASSIGN, res + "[" + std::to_string(context->offset) + "]", context->signed_const()->value);
    } else {
        if (context->is_global)
            out(GLOBAL, ASSIGN, res, context->signed_const()->value);
        else
            out(ASSIGN, res, context->signed_const()->value);
        context->r_value = context->signed_const()->r_value;
    }
    dbg("Leave Array_signed_const_const");
    return nullptr;
}

std::any 
Analyzer::visitArray_signed_const_array(CACTParser::Array_signed_const_arrayContext *context) {
    dbg("Enter Array_signed_const_array");
    if (context->array_size.empty()) {
        //
        exit(3);
        assert(0);
        return nullptr;
    }
    std::string res = NPRE + context->varName;
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
                context->array_signed_const()[i]->is_global_const = context->is_global_const;
                context->array_signed_const()[i]->accept(this);
            }
            if (!context->is_global) {
                for (int i = (int)context->array_signed_const().size(); i < sz; ++i) {
                    out(ASSIGN, res + "[" + std::to_string(i) + "]", "0");
                }
            }
            dbg("Leave Array_signed_const_array");
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
        context->array_signed_const()[i]->is_global_const = context->is_global_const;
        context->array_signed_const()[i]->accept(this);
    }
    if (context->array_signed_const().size() < context->array_size[0]) {
        if (!context->is_global) {
            for (int i = context->array_signed_const().size() * sz;
                      i < (int)context->array_size[0] * sz; ++i) {
                int offset = context->offset + i;
                out(ASSIGN, res + "[" + std::to_string(offset) + "]", "0");
            }
        }
    }
    dbg("Leave Array_signed_const_array");
    return nullptr;
}

std::any 
Analyzer::visitFunc_def(CACTParser::Func_defContext *context) {
    dbg("Enter Func_def");

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
    std::string res = NPRE + funcName;
    std::string signature = "";
    signature = signature + FUNC + " " + res + " ( ";
    for (int i = 0; i < (int)context->func_f_param().size(); ++i) {
        signature = signature + context->func_f_param()[i]->res;
        if (i + 1 < (int)context->func_f_param().size())
            signature = signature + " , ";
    }
    signature = signature + " )";
    out(signature);
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
    out(ENDFUNC);
    dbg("Leave Func_def");
    return nullptr;
}

std::any 
Analyzer::visitFunc_f_param(CACTParser::Func_f_paramContext *context) {
    dbg("Enter Func_f_param");
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
    context->res = NPRE + varName;
    dbg("Leave Func_f_param");
    return nullptr;
}

std::any
Analyzer::visitComp_units_more(CACTParser::Comp_units_moreContext *context) {
    dbg("Enter Comp_units_more");
    context->comp_unit()->accept(this);
    context->comp_units()->accept(this);
    dbg("Leave Comp_units_more");
    return nullptr;
}

std::any
Analyzer::visitComp_units_empty(CACTParser::Comp_units_emptyContext *context) {
    dbg("Enter Comp_units_empty");

    dbg("Leave Comp_units_empty");
    return nullptr;
}

std::any
Analyzer::visitComp_unit_decl(CACTParser::Comp_unit_declContext *context) {
    dbg("Enter Comp_unit_decl");
    context->decl()->is_global = true;
    visitChildren(context);
    dbg("Leave Comp_unit_decl");
    return nullptr;
}

std::any 
Analyzer::visitComp_unit_func_def(CACTParser::Comp_unit_func_defContext *context) {
    dbg("Enter Comp_unit_func_def");
    visitChildren(context);
    dbg("Leave Comp_unit_func_def");
    return nullptr;
}