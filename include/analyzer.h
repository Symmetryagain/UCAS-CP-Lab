#ifndef CACT_ANALYZER_H
#define CACT_ANALYZER_H

#include "CACTLexer.h"
#include "CACTParser.h"
#include "CACTVisitor.h"

class Analyzer : public CACTVisitor {
public:
    std::any visitTest(CACTParser::TestContext *context) override;
    std::any visitExpr_1_ident(CACTParser::Expr_1_identContext *context) override;
    std::any visitExpr_1_constant(CACTParser::Expr_1_constantContext *context) override;
    std::any visitExpr_1_braket(CACTParser::Expr_1_braketContext *context) override;
    std::any visitExpr_1_func_call(CACTParser::Expr_1_func_callContext *context) override;
    std::any visitExpr_1_array(CACTParser::Expr_1_arrayContext *context) override;
    std::any visitNum_const_int(CACTParser::Num_const_intContext *context) override;
    std::any visitNum_const_float(CACTParser::Num_const_floatContext *context) override;
    std::any visitNum_const_double(CACTParser::Num_const_doubleContext *context) override;
    std::any visitConst_num_const(CACTParser::Const_num_constContext *context) override;
    std::any visitConst_bool_const(CACTParser::Const_bool_constContext *context) override;
    std::any visitSigned_const_const(CACTParser::Signed_const_constContext *context) override;
    std::any visitSigned_const_plus(CACTParser::Signed_const_plusContext *context) override;
    std::any visitSigned_const_minus(CACTParser::Signed_const_minusContext *context) override;
    std::any visitSigned_const_num_const(CACTParser::Signed_const_num_constContext *context) override;
    std::any visitSigned_const_bool_const(CACTParser::Signed_const_bool_constContext *context) override;
    std::any visitInt_constant(CACTParser::Int_constantContext *context) override;
    std::any visitHex_constant(CACTParser::Hex_constantContext *context) override;
    std::any visitOct_constant(CACTParser::Oct_constantContext *context) override;
    std::any visitTrue_constant(CACTParser::True_constantContext *context) override;
    std::any visitFalse_constant(CACTParser::False_constantContext *context) override;
    std::any visitExpr_2_expr_1(CACTParser::Expr_2_expr_1Context *context) override;
    std::any visitExpr_2_plus(CACTParser::Expr_2_plusContext *context) override;
    std::any visitExpr_2_minus(CACTParser::Expr_2_minusContext *context) override;
    std::any visitExpr_2_not(CACTParser::Expr_2_notContext *context) override;
    std::any visitExpr_3_divide(CACTParser::Expr_3_divideContext *context) override;
    std::any visitExpr_3_times(CACTParser::Expr_3_timesContext *context) override;
    std::any visitExpr_3_remain(CACTParser::Expr_3_remainContext *context) override;
    std::any visitExpr_3_expr_2(CACTParser::Expr_3_expr_2Context *context) override;
    std::any visitExpr_4_plus(CACTParser::Expr_4_plusContext *context) override;
    std::any visitExpr_4_expr_3(CACTParser::Expr_4_expr_3Context *context) override;
    std::any visitExpr_4_minus(CACTParser::Expr_4_minusContext *context) override;
    std::any visitExpr_5_ge(CACTParser::Expr_5_geContext *context) override;
    std::any visitExpr_5_gt(CACTParser::Expr_5_gtContext *context) override;
    std::any visitExpr_5_expr_4(CACTParser::Expr_5_expr_4Context *context) override;
    std::any visitExpr_5_le(CACTParser::Expr_5_leContext *context) override;
    std::any visitExpr_5_lt(CACTParser::Expr_5_ltContext *context) override;
    std::any visitExpr_6_expr_5(CACTParser::Expr_6_expr_5Context *context) override;
    std::any visitExpr_6_ne(CACTParser::Expr_6_neContext *context) override;
    std::any visitExpr_6_eq(CACTParser::Expr_6_eqContext *context) override;
    std::any visitExpr_7_expr_6(CACTParser::Expr_7_expr_6Context *context) override;
    std::any visitExpr_7_dand(CACTParser::Expr_7_dandContext *context) override;
    std::any visitExpr_8_dor(CACTParser::Expr_8_dorContext *context) override;
    std::any visitExpr_8_expr_7(CACTParser::Expr_8_expr_7Context *context) override;
    std::any visitFunc_call_ident(CACTParser::Func_call_identContext *context) override;
    std::any visitType_int(CACTParser::Type_intContext *context) override;
    std::any visitType_bool(CACTParser::Type_boolContext *context) override;
    std::any visitType_float(CACTParser::Type_floatContext *context) override;
    std::any visitType_double(CACTParser::Type_doubleContext *context) override;
    std::any visitType_void(CACTParser::Type_voidContext *context) override;
    std::any visitStmt_assign(CACTParser::Stmt_assignContext *context) override;
    std::any visitL_value(CACTParser::L_valueContext *context) override;
    std::any visitStmt_expr(CACTParser::Stmt_exprContext *context) override;
    std::any visitStmt_block(CACTParser::Stmt_blockContext *context) override;
    std::any visitStmt_return(CACTParser::Stmt_returnContext *context) override;
    std::any visitStmt_if(CACTParser::Stmt_ifContext *context) override;
    std::any visitStmt_while(CACTParser::Stmt_whileContext *context) override;
    std::any visitStmt_break(CACTParser::Stmt_breakContext *context) override;
    std::any visitStmt_continue(CACTParser::Stmt_continueContext *context) override;
    std::any visitAssign_stmt(CACTParser::Assign_stmtContext *context) override;
    std::any visitExpr_stmt(CACTParser::Expr_stmtContext *context) override;
    std::any visitBlock_stmt(CACTParser::Block_stmtContext *context) override;
    std::any visitReturn_stmt(CACTParser::Return_stmtContext *context) override;
    std::any visitIf_stmt(CACTParser::If_stmtContext *context) override;
    std::any visitWhile_stmt(CACTParser::While_stmtContext *context) override;
    std::any visitBreak_stmt(CACTParser::Break_stmtContext *context) override;
    std::any visitContinue_stmt(CACTParser::Continue_stmtContext *context) override;
    std::any visitDecl_var(CACTParser::Decl_varContext *context) override;
    std::any visitDecl_const(CACTParser::Decl_constContext *context) override;
    std::any visitVar_decl(CACTParser::Var_declContext *context) override;
    std::any visitConst_decl(CACTParser::Const_declContext *context) override;
    std::any visitBlock_item_decl(CACTParser::Block_item_declContext *context) override;
    std::any visitBlock_item_stmt(CACTParser::Block_item_stmtContext *context) override;
    std::any visitBlock(CACTParser::BlockContext *context) override;
    std::any visitVar_def(CACTParser::Var_defContext *context) override;
    std::any visitConst_def(CACTParser::Const_defContext *context) override;
    std::any visitArray_signed_const_const(CACTParser::Array_signed_const_constContext *context) override;
    std::any visitArray_signed_const_array(CACTParser::Array_signed_const_arrayContext *context) override;
    std::any visitFunc_def(CACTParser::Func_defContext *context) override;
    std::any visitFunc_f_param(CACTParser::Func_f_paramContext *context) override;
    std::any visitComp_units_more(CACTParser::Comp_units_moreContext *context) override;
    std::any visitComp_units_empty(CACTParser::Comp_units_emptyContext *context) override;
    std::any visitComp_unit_decl(CACTParser::Comp_unit_declContext *context) override;
    std::any visitComp_unit_func_def(CACTParser::Comp_unit_func_defContext *context) override;
};

#endif // _Analyzer_H