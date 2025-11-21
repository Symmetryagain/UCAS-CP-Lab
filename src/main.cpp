#include <iostream>

// #include "antlr4-runtime.h"
#include "tree/ErrorNode.h"
#include "tree/ParseTree.h"

#include "CACTLexer.h"
#include "CACTParser.h"
#include "CACTVisitor.h"

using namespace antlr4;

class Analyzer : public CACTVisitor {
public:
  std::any 
  visitExpr(CACTParser::ExprContext *context) override {
    std::cout << "Enter Expr" << std::endl;

    std::cout << "Leave Expr" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_1_ident(CACTParser::Expr_1_identContext *context) override {
    std::cout << "Enter Expr_1_ident" << std::endl;

    std::cout << "Leave Expr_1_ident" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_1_constant(CACTParser::Expr_1_constantContext *context) override {
    std::cout << "Enter Expr_1_constant" << std::endl;

    std::cout << "Leave Expr_1_constant" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_1_braket(CACTParser::Expr_1_braketContext *context) override {
    std::cout << "Enter Expr_1_braket" << std::endl;

    std::cout << "Leave Expr_1_braket" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_1_func_call(CACTParser::Expr_1_func_callContext *context) override {
    std::cout << "Enter Expr_1_func_call" << std::endl;

    std::cout << "Leave Expr_1_func_call" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_1_array(CACTParser::Expr_1_arrayContext *context) override {
    std::cout << "Enter Expr_1_array" << std::endl;

    std::cout << "Leave Expr_1_array" << std::endl;
    return nullptr;
  }

  std::any
  visitNum_const_int(CACTParser::Num_const_intContext *context) override {
    std::cout << "Enter Num_const_int" << std::endl;

    std::cout << "Leave Num_const_int" << std::endl;
    return nullptr;
  }

  std::any
  visitNum_const_float(CACTParser::Num_const_floatContext *context) override {
    std::cout << "Enter Num_const_float" << std::endl;

    std::cout << "Leave Num_const_float" << std::endl;
    return nullptr;
  }

  std::any
  visitNum_const_double(CACTParser::Num_const_doubleContext *context) override {
    std::cout << "Enter Num_const_double" << std::endl;

    std::cout << "Leave Num_const_double" << std::endl;
    return nullptr;
  }

  std::any
  visitConst_num_const(CACTParser::Const_num_constContext *context) override {
    std::cout << "Enter Const_num_const" << std::endl;

    std::cout << "Leave Const_num_const" << std::endl;
    return nullptr;
  }

  std::any
  visitConst_bool_const(CACTParser::Const_bool_constContext *context) override {
    std::cout << "Enter Const_bool_const" << std::endl;

    std::cout << "Leave Const_bool_const" << std::endl;
    return nullptr;
  }

  std::any 
  visitSigned_const_const(CACTParser::Signed_const_constContext *context) override {
    std::cout << "Enter Signed_const_const" << std::endl;

    std::cout << "Leave Signed_const_const" << std::endl;
    return nullptr;
  }

  std::any
  visitSigned_const_plus(CACTParser::Signed_const_plusContext *context) override {
    std::cout << "Enter Signed_const_plus" << std::endl;

    std::cout << "Leave Signed_const_plus" << std::endl;
    return nullptr;
  }

  std::any 
  visitSigned_const_minus(CACTParser::Signed_const_minusContext *context) override {
    std::cout << "Enter Signed_const_minus" << std::endl;

    std::cout << "Leave Signed_const_minus" << std::endl;
    return nullptr;
  }

  std::any 
  visitSigned_const_num_const(CACTParser::Signed_const_num_constContext *context) override {
    std::cout << "Enter Signed_const_num_const" << std::endl;

    std::cout << "Leave Signed_const_num_const" << std::endl;
    return nullptr;
  }

  std::any 
  visitSigned_const_bool_const(CACTParser::Signed_const_bool_constContext *context) override {
    std::cout << "Enter Signed_const_bool_const" << std::endl;

    std::cout << "Leave Signed_const_bool_const" << std::endl;
    return nullptr;
  }

  std::any
  visitInt_constant(CACTParser::Int_constantContext *context) override {
    std::cout << "Enter Int_constant" << std::endl;

    std::cout << "Leave Int_constant" << std::endl;
    return nullptr;
  }

  std::any
  visitHex_constant(CACTParser::Hex_constantContext *context) override {
    std::cout << "Enter Hex_constant" << std::endl;

    std::cout << "Leave Hex_constant" << std::endl;
    return nullptr;
  }

  std::any
  visitOct_constant(CACTParser::Oct_constantContext *context) override {
    std::cout << "Enter Oct_constant" << std::endl;

    std::cout << "Leave Oct_constant" << std::endl;
    return nullptr;
  }

  std::any
  visitTrue_constant(CACTParser::True_constantContext *context) override {
    std::cout << "Enter True_constant" << std::endl;

    std::cout << "Leave True_constant" << std::endl;
    return nullptr;
  }

  std::any
  visitFalse_constant(CACTParser::False_constantContext *context) override {
    std::cout << "Enter False_constant" << std::endl;

    std::cout << "Leave False_constant" << std::endl;
    return nullptr;
  }

  std::any 
  visitFloatconst(CACTParser::FloatconstContext *context) override {
    std::cout << "Enter Floatconst" << std::endl;

    std::cout << "Leave Floatconst" << std::endl;
    return nullptr;
  }

  std::any 
  visitDoubleconst(CACTParser::DoubleconstContext *context) override {
    std::cout << "Enter Doubleconst" << std::endl;

    std::cout << "Leave Doubleconst" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_2_expr_1(CACTParser::Expr_2_expr_1Context *context) override {
    std::cout << "Enter Expr_2_expr_1" << std::endl;

    std::cout << "Leave Expr_2_expr_1" << std::endl;
    return nullptr;
  }

  std::any 
  visitExpr_2_plus(CACTParser::Expr_2_plusContext *context) override {
    std::cout << "Enter Expr_2_plus" << std::endl;

    std::cout << "Leave Expr_2_plus" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_2_minus(CACTParser::Expr_2_minusContext *context) override {
    std::cout << "Enter Expr_2_minus" << std::endl;

    std::cout << "Leave Expr_2_minus" << std::endl;
    return nullptr;
  }

  std::any visitExpr_2_not(CACTParser::Expr_2_notContext *context) override {
    std::cout << "Enter Expr_2_not" << std::endl;

    std::cout << "Leave Expr_2_not" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_3_divide(CACTParser::Expr_3_divideContext *context) override {
    std::cout << "Enter Expr_3_divide" << std::endl;

    std::cout << "Leave Expr_3_divide" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_3_times(CACTParser::Expr_3_timesContext *context) override {
    std::cout << "Enter Expr_3_times" << std::endl;

    std::cout << "Leave Expr_3_times" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_3_remain(CACTParser::Expr_3_remainContext *context) override {
    std::cout << "Enter Expr_3_remain" << std::endl;

    std::cout << "Leave Expr_3_remain" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_3_expr_2(CACTParser::Expr_3_expr_2Context *context) override {
    std::cout << "Enter Expr_3_expr_2" << std::endl;

    std::cout << "Leave Expr_3_expr_2" << std::endl;
    return nullptr;
  }

  std::any 
  visitExpr_4_plus(CACTParser::Expr_4_plusContext *context) override {
    std::cout << "Enter Expr_4_plus" << std::endl;

    std::cout << "Leave Expr_4_plus" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_4_expr_3(CACTParser::Expr_4_expr_3Context *context) override {
    std::cout << "Enter Expr_4_expr_3" << std::endl;

    std::cout << "Leave Expr_4_expr_3" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_4_minus(CACTParser::Expr_4_minusContext *context) override {
    std::cout << "Enter Expr_4_minus" << std::endl;

    std::cout << "Leave Expr_4_minus" << std::endl;
    return nullptr;
  }

  std::any 
  visitExpr_5_ge(CACTParser::Expr_5_geContext *context) override {
    std::cout << "Enter Expr_5_ge" << std::endl;

    std::cout << "Leave Expr_5_ge" << std::endl;
    return nullptr;
  }

  std::any 
  visitExpr_5_gt(CACTParser::Expr_5_gtContext *context) override {
    std::cout << "Enter Expr_5_gt" << std::endl;

    std::cout << "Leave Expr_5_gt" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_5_expr_4(CACTParser::Expr_5_expr_4Context *context) override {
    std::cout << "Enter Expr_5_expr_4" << std::endl;

    std::cout << "Leave Expr_5_expr_4" << std::endl;
    return nullptr;
  }

  std::any 
  visitExpr_5_le(CACTParser::Expr_5_leContext *context) override {
    std::cout << "Enter Expr_5_le" << std::endl;

    std::cout << "Leave Expr_5_le" << std::endl;
    return nullptr;
  }

  std::any 
  visitExpr_5_lt(CACTParser::Expr_5_ltContext *context) override {
    std::cout << "Enter Expr_5_lt" << std::endl;

    std::cout << "Leave Expr_5_lt" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_6_expr_5(CACTParser::Expr_6_expr_5Context *context) override {
    std::cout << "Enter Expr_6_expr_5" << std::endl;

    std::cout << "Leave Expr_6_expr_5" << std::endl;
    return nullptr;
  }

  std::any 
  visitExpr_6_ne(CACTParser::Expr_6_neContext *context) override {
    std::cout << "Enter Expr_6_ne" << std::endl;

    std::cout << "Leave Expr_6_ne" << std::endl;
    return nullptr;
  }

  std::any 
  visitExpr_6_eq(CACTParser::Expr_6_eqContext *context) override {
    std::cout << "Enter Expr_6_eq" << std::endl;

    std::cout << "Leave Expr_6_eq" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_7_expr_6(CACTParser::Expr_7_expr_6Context *context) override {
    std::cout << "Enter Expr_7_expr_6" << std::endl;

    std::cout << "Leave Expr_7_expr_6" << std::endl;
    return nullptr;
  }

  std::any 
  visitExpr_7_dand(CACTParser::Expr_7_dandContext *context) override {
    std::cout << "Enter Expr_7_dand" << std::endl;

    std::cout << "Leave Expr_7_dand" << std::endl;
    return nullptr;
  }

  std::any 
  visitExpr_8_dor(CACTParser::Expr_8_dorContext *context) override {
    std::cout << "Enter Expr_8_dor" << std::endl;

    std::cout << "Leave Expr_8_dor" << std::endl;
    return nullptr;
  }

  std::any
  visitExpr_8_expr_7(CACTParser::Expr_8_expr_7Context *context) override {
    std::cout << "Enter Expr_8_expr_7" << std::endl;

    std::cout << "Leave Expr_8_expr_7" << std::endl;
    return nullptr;
  }

  std::any
  visitFunc_call_ident(CACTParser::Func_call_identContext *context) override {
    std::cout << "Enter Func_call_ident" << std::endl;

    std::cout << "Leave Func_call_ident" << std::endl;
    return nullptr;
  }

  std::any
  visitArg_list_exprs(CACTParser::Arg_list_exprsContext *context) override {
    std::cout << "Enter Arg_list_exprs" << std::endl;

    std::cout << "Leave Arg_list_exprs" << std::endl;
    return nullptr;
  }

  std::any 
  visitType_int(CACTParser::Type_intContext *context) override {
    std::cout << "Enter Type_int" << std::endl;

    std::cout << "Leave Type_int" << std::endl;
    return nullptr;
  }

  std::any 
  visitType_bool(CACTParser::Type_boolContext *context) override {
    std::cout << "Enter Type_bool" << std::endl;

    std::cout << "Leave Type_bool" << std::endl;
    return nullptr;
  }

  std::any 
  visitType_float(CACTParser::Type_floatContext *context) override {
    std::cout << "Enter Type_float" << std::endl;

    std::cout << "Leave Type_float" << std::endl;
    return nullptr;
  }

  std::any 
  visitType_double(CACTParser::Type_doubleContext *context) override {
    std::cout << "Enter Type_double" << std::endl;

    std::cout << "Leave Type_double" << std::endl;
    return nullptr;
  }

  std::any 
  visitType_void(CACTParser::Type_voidContext *context) override {
    std::cout << "Enter Type_void" << std::endl;

    std::cout << "Leave Type_void" << std::endl;
    return nullptr;
  }

  std::any 
  visitStmt_assign(CACTParser::Stmt_assignContext *context) override {
    std::cout << "Enter Stmt_assign" << std::endl;

    std::cout << "Leave Stmt_assign" << std::endl;
    return nullptr;
  }

  std::any 
  visitL_value(CACTParser::L_valueContext *context) override {
    std::cout << "Enter L_value" << std::endl;

    std::cout << "Leave L_value" << std::endl;
    return nullptr;
  }

  std::any 
  visitStmt_expr(CACTParser::Stmt_exprContext *context) override {
    std::cout << "Enter Stmt_expr" << std::endl;

    std::cout << "Leave Stmt_expr" << std::endl;
    return nullptr;
  }

  std::any 
  visitStmt_block(CACTParser::Stmt_blockContext *context) override {
    std::cout << "Enter Stmt_block" << std::endl;

    std::cout << "Leave Stmt_block" << std::endl;
    return nullptr;
  }

  std::any 
  visitStmt_return(CACTParser::Stmt_returnContext *context) override {
    std::cout << "Enter Stmt_return" << std::endl;

    std::cout << "Leave Stmt_return" << std::endl;
    return nullptr;
  }

  std::any 
  visitStmt_if(CACTParser::Stmt_ifContext *context) override {
    std::cout << "Enter Stmt_if" << std::endl;

    std::cout << "Leave Stmt_if" << std::endl;
    return nullptr;
  }

  std::any 
  visitStmt_while(CACTParser::Stmt_whileContext *context) override {
    std::cout << "Enter Stmt_while" << std::endl;

    std::cout << "Leave Stmt_while" << std::endl;
    return nullptr;
  }

  std::any 
  visitStmt_break(CACTParser::Stmt_breakContext *context) override {
    std::cout << "Enter Stmt_break" << std::endl;

    std::cout << "Leave Stmt_break" << std::endl;
    return nullptr;
  }

  std::any
  visitStmt_continue(CACTParser::Stmt_continueContext *context) override {
    std::cout << "Enter Stmt_continue" << std::endl;

    std::cout << "Leave Stmt_continue" << std::endl;
    return nullptr;
  }

  std::any 
  visitAssign_stmt(CACTParser::Assign_stmtContext *context) override {
    std::cout << "Enter Assign_stmt" << std::endl;

    std::cout << "Leave Assign_stmt" << std::endl;
    return nullptr;
  }

  std::any 
  visitExpr_stmt(CACTParser::Expr_stmtContext *context) override {
    std::cout << "Enter Expr_stmt" << std::endl;

    std::cout << "Leave Expr_stmt" << std::endl;
    return nullptr;
  }

  std::any 
  visitBlock_stmt(CACTParser::Block_stmtContext *context) override {
    std::cout << "Enter Block_stmt" << std::endl;

    std::cout << "Leave Block_stmt" << std::endl;
    return nullptr;
  }

  std::any 
  visitReturn_stmt(CACTParser::Return_stmtContext *context) override {
    std::cout << "Enter Return_stmt" << std::endl;

    std::cout << "Leave Return_stmt" << std::endl;
    return nullptr;
  }

  std::any 
  visitIf_stmt(CACTParser::If_stmtContext *context) override {
    std::cout << "Enter If_stmt" << std::endl;

    std::cout << "Leave If_stmt" << std::endl;
    return nullptr;
  }

  std::any 
  visitWhile_stmt(CACTParser::While_stmtContext *context) override {
    std::cout << "Enter While_stmt" << std::endl;

    std::cout << "Leave While_stmt" << std::endl;
    return nullptr;
  }

  std::any 
  visitBreak_stmt(CACTParser::Break_stmtContext *context) override {
    std::cout << "Enter Break_stmt" << std::endl;

    std::cout << "Leave Break_stmt" << std::endl;
    return nullptr;
  }

  std::any
  visitContinue_stmt(CACTParser::Continue_stmtContext *context) override {
    std::cout << "Enter Continue_stmt" << std::endl;

    std::cout << "Leave Continue_stmt" << std::endl;
    return nullptr;
  }

  std::any 
  visitDecl_var(CACTParser::Decl_varContext *context) override {
    std::cout << "Enter Decl_var" << std::endl;

    std::cout << "Leave Decl_var" << std::endl;
    return nullptr;
  }

  std::any 
  visitDecl_const(CACTParser::Decl_constContext *context) override {
    std::cout << "Enter Decl_const" << std::endl;

    std::cout << "Leave Decl_const" << std::endl;
    return nullptr;
  }

  std::any 
  visitVar_decl(CACTParser::Var_declContext *context) override {
    std::cout << "Enter Var_decl" << std::endl;

    std::cout << "Leave Var_decl" << std::endl;
    return nullptr;
  }

  std::any 
  visitConst_decl(CACTParser::Const_declContext *context) override {
    std::cout << "Enter Const_decl" << std::endl;

    std::cout << "Leave Const_decl" << std::endl;
    return nullptr;
  }

  std::any
  visitBlock_item_decl(CACTParser::Block_item_declContext *context) override {
    std::cout << "Enter Block_item_decl" << std::endl;

    std::cout << "Leave Block_item_decl" << std::endl;
    return nullptr;
  }

  std::any
  visitBlock_item_stmt(CACTParser::Block_item_stmtContext *context) override {
    std::cout << "Enter Block_item_stmt" << std::endl;

    std::cout << "Leave Block_item_stmt" << std::endl;
    return nullptr;
  }

  std::any 
  visitBlock(CACTParser::BlockContext *context) override {
    std::cout << "Enter Block" << std::endl;

    std::cout << "Leave Block" << std::endl;
    return nullptr;
  }

  std::any 
  visitVar_def(CACTParser::Var_defContext *context) override {
    std::cout << "Enter Var_def" << std::endl;

    std::cout << "Leave Var_def" << std::endl;
    return nullptr;
  }

  std::any 
  visitConst_def(CACTParser::Const_defContext *context) override {
    std::cout << "Enter Const_def" << std::endl;

    std::cout << "Leave Const_def" << std::endl;
    return nullptr;
  }

  std::any 
  visitArray_signed_const_const(CACTParser::Array_signed_const_constContext *context) override {
    std::cout << "Enter Array_signed_const_const" << std::endl;

    std::cout << "Leave Array_signed_const_const" << std::endl;
    return nullptr;
  }

  std::any 
  visitArray_signed_const_array(CACTParser::Array_signed_const_arrayContext *context) override {
    std::cout << "Enter Array_signed_const_array" << std::endl;

    std::cout << "Leave Array_signed_const_array" << std::endl;
    return nullptr;
  }

  std::any 
  visitFunc_def(CACTParser::Func_defContext *context) override {
    std::cout << "Enter Func_def" << std::endl;

    std::cout << "Leave Func_def" << std::endl;
    return nullptr;
  }

  std::any
  visitFunc_f_param(CACTParser::Func_f_paramContext *context) override {
    std::cout << "Enter Func_f_param" << std::endl;

    std::cout << "Leave Func_f_param" << std::endl;
    return nullptr;
  }

  std::any
  visitComp_units_more(CACTParser::Comp_units_moreContext *context) override {
    std::cout << "Enter Comp_units_more" << std::endl;
    visitChildren(context);
    std::cout << "Leave Comp_units_more" << std::endl;
    return nullptr;
  }

  std::any
  visitComp_units_empty(CACTParser::Comp_units_emptyContext *context) override {
    std::cout << "Enter Comp_units_empty" << std::endl;
    visitChildren(context);
    std::cout << "Leave Comp_units_empty" << std::endl;
    return nullptr;
  }

  std::any
  visitComp_unit_decl(CACTParser::Comp_unit_declContext *context) override {
    std::cout << "Enter Comp_unit_decl" << std::endl;
    visitChildren(context);
    std::cout << "Leave Comp_unit_decl" << std::endl;
    return nullptr;
  }

  std::any 
  visitComp_unit_func_def(CACTParser::Comp_unit_func_defContext *context) override {
    std::cout << "Enter Comp_unit_func_def" << std::endl;
    visitChildren(context);
    std::cout << "Leave Comp_unit_func_def" << std::endl;
    return nullptr;
  }
};

//     std::any visitErrorNode(tree::ErrorNode * node) override {
//         std::cout << "visit error node!" << std::endl;
//         return nullptr;
//     }
// };

// 添加树形打印功能
void printTree(antlr4::tree::ParseTree *tree,
               const std::vector<std::string> &ruleNames,
               const std::string &indent = "", bool isLast = true) {
  std::cout << indent;

  if (isLast) {
    std::cout << "└── ";
  } else {
    std::cout << "├── ";
  }

  // 打印节点信息
  if (tree->children.empty()) {
    // 叶子节点（终结符）
    std::cout << tree->toString() << std::endl;
  } else {
    // 内部节点（非终结符）
    auto *ctx = dynamic_cast<antlr4::ParserRuleContext *>(tree);
    if (ctx) {
      std::cout << ruleNames[ctx->getRuleIndex()] << std::endl;
    } else {
      std::cout << "Unknown" << std::endl;
    }
  }

  // 递归打印子节点
  std::string newIndent = indent + (isLast ? "    " : "│   ");
  for (size_t i = 0; i < tree->children.size(); i++) {
    printTree(tree->children[i], ruleNames, newIndent,
              i == tree->children.size() - 1);
  }
}

int main(int argc, const char *argv[]) {
  std::ifstream stream;
  if (argc > 1) {
    stream.open(argv[1]);
  } else {
    std::cerr << "No input file specified" << std::endl;
    return 9;
  }

  if (!stream.is_open()) {
    std::cerr << "Failed to open file" << std::endl;
    return 9;
  }

  ANTLRInputStream input(stream);
  CACTLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  // 打印所有Token
  std::cout << "=== Token Stream ===" << std::endl;
  tokens.fill();
  for (auto token : tokens.getTokens()) {
    if (token->getType() != antlr4::Token::EOF) {
      std::cout << "Line " << token->getLine() << ":"
                << token->getCharPositionInLine() << " "
                << lexer.getVocabulary().getSymbolicName(token->getType())
                << " = '" << token->getText() << "'" << std::endl;
    }
  }

  CACTParser parser(&tokens);

  // 获取解析树
  auto tree = parser.comp_units();

  // 打印解析树
  std::cout << "=== Parse Tree ===" << std::endl;
  std::cout << tree->toStringTree(&parser) << std::endl;

  std::cout << "\n=== Detailed AST ===" << std::endl;
  printTree(tree, parser.getRuleNames());

  if (lexer.getNumberOfSyntaxErrors() > 0) {
    std::cout << "lex error: " << lexer.getNumberOfSyntaxErrors() << std::endl;
    std::cout << "False" << std::endl;
    return 1;
  }

  if (parser.getNumberOfSyntaxErrors() > 0) {
    std::cout << "syntax error: " << parser.getNumberOfSyntaxErrors()
              << std::endl;
    std::cout << "False" << std::endl;
    return 2;
  }

  std::cout << "=== End ===" << std::endl;
  std::cout << "Number of errors: "
            << lexer.getNumberOfSyntaxErrors() +
                   parser.getNumberOfSyntaxErrors()
            << std::endl;

  std::cout << "True" << std::endl;

  Analyzer visitor;
  visitor.visit(tree);

  return parser.getNumberOfSyntaxErrors() + lexer.getNumberOfSyntaxErrors();
}