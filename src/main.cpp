#include <iostream>

// #include "antlr4-runtime.h"
#include "tree/ErrorNode.h"

#include "CACTLexer.h"
#include "CACTParser.h"
#include "CACTVisitor.h"

using namespace antlr4;

class Analyzer : public CACTVisitor {
public:
	std::any visitComp_units_more(CACTParser::Comp_units_moreContext *context)
	override { return nullptr;

	}

	std::any visitComp_units_empty(CACTParser::Comp_units_emptyContext *context)
	override { return nullptr;

	}

    std::any visitComp_unit_const_decl(CACTParser::Comp_unit_const_declContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_unit_var_decl(CACTParser::Comp_unit_var_declContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_unit_func(CACTParser::Comp_unit_funcContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_const_int(CACTParser::Decl_const_intContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_const_bool(CACTParser::Decl_const_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_const_float(CACTParser::Decl_const_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_const_double(CACTParser::Decl_const_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_int(CACTParser::Decl_intContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_bool(CACTParser::Decl_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_float(CACTParser::Decl_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_double(CACTParser::Decl_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_int_func(CACTParser::Def_int_funcContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_bool_func(CACTParser::Def_bool_funcContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_float_func(CACTParser::Def_float_funcContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_double_func(CACTParser::Def_double_funcContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_void_func(CACTParser::Def_void_funcContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_int_def(CACTParser::Func_int_defContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_bool_def(CACTParser::Func_bool_defContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_float_def(CACTParser::Func_float_defContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_double_def(CACTParser::Func_double_defContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_void_def(CACTParser::Func_void_defContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_f_params(CACTParser::Func_f_paramsContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_f_param_int(CACTParser::Func_f_param_intContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_f_param_bool(CACTParser::Func_f_param_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_f_param_float(CACTParser::Func_f_param_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_f_param_double(CACTParser::Func_f_param_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_call(CACTParser::Func_callContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_r_params(CACTParser::Func_r_paramsContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_r_param(CACTParser::Func_r_paramContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_int_decl(CACTParser::Const_int_declContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_bool_decl(CACTParser::Const_bool_declContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_float_decl(CACTParser::Const_float_declContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_double_decl(CACTParser::Const_double_declContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_int_var(CACTParser::Def_const_int_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_int_arr(CACTParser::Def_const_int_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_bool_var(CACTParser::Def_const_bool_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_bool_arr(CACTParser::Def_const_bool_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_float_var(CACTParser::Def_const_float_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_float_arr(CACTParser::Def_const_float_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_double_var(CACTParser::Def_const_double_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_double_arr(CACTParser::Def_const_double_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_int_var_def(CACTParser::Const_int_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_bool_var_def(CACTParser::Const_bool_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_float_var_def(CACTParser::Const_float_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_double_var_def(CACTParser::Const_double_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_int_arr_def(CACTParser::Const_int_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_bool_arr_def(CACTParser::Const_bool_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_float_arr_def(CACTParser::Const_float_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_double_arr_def(CACTParser::Const_double_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_arr_init_braket(CACTParser::Int_arr_init_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_arr_init_braket(CACTParser::Bool_arr_init_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_arr_init_braket(CACTParser::Float_arr_init_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_arr_init_braket(CACTParser::Double_arr_init_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_arr_init_list(CACTParser::Int_arr_init_listContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_arr_init_list_elem_int(CACTParser::Int_arr_init_list_elem_intContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_arr_init_list_elem_arr(CACTParser::Int_arr_init_list_elem_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_arr_init_list(CACTParser::Bool_arr_init_listContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_arr_init_list_elem_bool(CACTParser::Bool_arr_init_list_elem_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_arr_init_list_elem_arr(CACTParser::Bool_arr_init_list_elem_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_arr_init_list(CACTParser::Float_arr_init_listContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_arr_init_list_elem_float(CACTParser::Float_arr_init_list_elem_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_arr_init_list_elem_arr(CACTParser::Float_arr_init_list_elem_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_arr_init_list(CACTParser::Double_arr_init_listContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_arr_init_list_elem_double(CACTParser::Double_arr_init_list_elem_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_arr_init_list_elem_arr(CACTParser::Double_arr_init_list_elem_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_decl(CACTParser::Int_declContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_decl(CACTParser::Bool_declContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_decl(CACTParser::Float_declContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_decl(CACTParser::Double_declContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_int_var(CACTParser::Def_int_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_int_arr(CACTParser::Def_int_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_bool_var(CACTParser::Def_bool_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_bool_arr(CACTParser::Def_bool_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_float_var(CACTParser::Def_float_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_float_arr(CACTParser::Def_float_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_double_var(CACTParser::Def_double_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_double_arr(CACTParser::Def_double_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_var_def(CACTParser::Int_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_var_def(CACTParser::Bool_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_var_def(CACTParser::Float_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_var_def(CACTParser::Double_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_arr_def(CACTParser::Int_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_arr_def(CACTParser::Bool_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_arr_def(CACTParser::Float_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_arr_def(CACTParser::Double_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_int(CACTParser::Block_intContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_void(CACTParser::Block_voidContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_bool(CACTParser::Block_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_float(CACTParser::Block_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_double(CACTParser::Block_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_const_decl(CACTParser::Item_const_declContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_var_decl(CACTParser::Item_var_declContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_exp(CACTParser::Item_expContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_assign(CACTParser::Item_assignContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_none(CACTParser::Item_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_int_item(CACTParser::Item_int_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_int_block(CACTParser::Item_int_blockContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_int_return(CACTParser::Item_int_returnContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_int_if(CACTParser::Item_int_ifContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_int_while(CACTParser::Item_int_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_bool_item(CACTParser::Item_bool_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_bool_block(CACTParser::Item_bool_blockContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_bool_return(CACTParser::Item_bool_returnContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_bool_if(CACTParser::Item_bool_ifContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_bool_while(CACTParser::Item_bool_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_float_item(CACTParser::Item_float_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_float_block(CACTParser::Item_float_blockContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_float_return(CACTParser::Item_float_returnContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_float_if(CACTParser::Item_float_ifContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_float_while(CACTParser::Item_float_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_double_item(CACTParser::Item_double_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_double_block(CACTParser::Item_double_blockContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_double_return(CACTParser::Item_double_returnContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_double_if(CACTParser::Item_double_ifContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_double_while(CACTParser::Item_double_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_void_item(CACTParser::Item_void_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_void_block(CACTParser::Item_void_blockContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_void_return(CACTParser::Item_void_returnContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_void_if(CACTParser::Item_void_ifContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_void_while(CACTParser::Item_void_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitReturn_int(CACTParser::Return_intContext *context)
	override { return nullptr;
		
	}

    std::any visitReturn_void(CACTParser::Return_voidContext *context)
	override { return nullptr;
		
	}

    std::any visitReturn_bool(CACTParser::Return_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitReturn_float(CACTParser::Return_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitReturn_double(CACTParser::Return_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitIf_int(CACTParser::If_intContext *context)
	override { return nullptr;
		
	}

    std::any visitIf_void(CACTParser::If_voidContext *context)
	override { return nullptr;
		
	}

    std::any visitIf_bool(CACTParser::If_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitIf_float(CACTParser::If_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitIf_double(CACTParser::If_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitWhile_int(CACTParser::While_intContext *context)
	override { return nullptr;
		
	}

    std::any visitWhile_void(CACTParser::While_voidContext *context)
	override { return nullptr;
		
	}

    std::any visitWhile_bool(CACTParser::While_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitWhile_float(CACTParser::While_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitWhile_double(CACTParser::While_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_while_int(CACTParser::Block_while_intContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_while_void(CACTParser::Block_while_voidContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_while_bool(CACTParser::Block_while_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_while_float(CACTParser::Block_while_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_while_double(CACTParser::Block_while_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_break(CACTParser::Item_while_breakContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_continue(CACTParser::Item_while_continueContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_int_while(CACTParser::Item_while_int_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_int_item(CACTParser::Item_while_int_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_void_while(CACTParser::Item_while_void_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_void_item(CACTParser::Item_while_void_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_bool_while(CACTParser::Item_while_bool_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_bool_item(CACTParser::Item_while_bool_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_float_while(CACTParser::Item_while_float_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_float_item(CACTParser::Item_while_float_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_double_while(CACTParser::Item_while_double_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_double_item(CACTParser::Item_while_double_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitAssign_int(CACTParser::Assign_intContext *context)
	override { return nullptr;
		
	}

    std::any visitAssign_bool(CACTParser::Assign_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitAssign_float(CACTParser::Assign_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitAssign_double(CACTParser::Assign_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_assign(CACTParser::Int_assignContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_assign(CACTParser::Bool_assignContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_assign(CACTParser::Float_assignContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_assign(CACTParser::Double_assignContext *context)
	override { return nullptr;
		
	}

    std::any visitExp_void(CACTParser::Exp_voidContext *context)
	override { return nullptr;
		
	}

    std::any visitExp_bool(CACTParser::Exp_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitExp_int(CACTParser::Exp_intContext *context)
	override { return nullptr;
		
	}

    std::any visitExp_float(CACTParser::Exp_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitExp_double(CACTParser::Exp_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitVoid_exp(CACTParser::Void_expContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_exp(CACTParser::Bool_expContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_8_7_or(CACTParser::Bool_8_7_orContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_8_7_none(CACTParser::Bool_8_7_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_7_6_none(CACTParser::Bool_7_6_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_7_6_and(CACTParser::Bool_7_6_andContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_6_5_comp_double(CACTParser::Bool_6_5_comp_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_6_5_ne(CACTParser::Bool_6_5_neContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_6_5_none(CACTParser::Bool_6_5_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_6_5_comp_float(CACTParser::Bool_6_5_comp_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_6_5_eq(CACTParser::Bool_6_5_eqContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_6_5_comp_int(CACTParser::Bool_6_5_comp_intContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_5_2_not(CACTParser::Bool_5_2_notContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_5_2_comp_int(CACTParser::Bool_5_2_comp_intContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_5_2_comp_float(CACTParser::Bool_5_2_comp_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_5_2_comp_double(CACTParser::Bool_5_2_comp_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_2_1_not(CACTParser::Bool_2_1_notContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_2_1_none(CACTParser::Bool_2_1_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_1_8_braket(CACTParser::Bool_1_8_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_1_bool_const(CACTParser::Bool_1_bool_constContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_1_func_call(CACTParser::Bool_1_func_callContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_1_ident(CACTParser::Bool_1_identContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_int_6_eq(CACTParser::Comp_int_6_eqContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_int_6_ne(CACTParser::Comp_int_6_neContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_int_5_lt(CACTParser::Comp_int_5_ltContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_int_5_gt(CACTParser::Comp_int_5_gtContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_int_5_le(CACTParser::Comp_int_5_leContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_int_5_ge(CACTParser::Comp_int_5_geContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_exp(CACTParser::Int_expContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_4_3_plus(CACTParser::Int_4_3_plusContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_4_3_none(CACTParser::Int_4_3_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_4_3_minus(CACTParser::Int_4_3_minusContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_3_2_times(CACTParser::Int_3_2_timesContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_3_2_divide(CACTParser::Int_3_2_divideContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_3_2_remain(CACTParser::Int_3_2_remainContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_3_2_none(CACTParser::Int_3_2_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_2_1_plus(CACTParser::Int_2_1_plusContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_2_1_minus(CACTParser::Int_2_1_minusContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_2_1_none(CACTParser::Int_2_1_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_1_4_braket(CACTParser::Int_1_4_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_1_int_const(CACTParser::Int_1_int_constContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_1_func_call(CACTParser::Int_1_func_callContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_1_ident(CACTParser::Int_1_identContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_float_6_eq(CACTParser::Comp_float_6_eqContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_float_6_ne(CACTParser::Comp_float_6_neContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_float_5_lt(CACTParser::Comp_float_5_ltContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_float_5_gt(CACTParser::Comp_float_5_gtContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_float_5_le(CACTParser::Comp_float_5_leContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_float_5_ge(CACTParser::Comp_float_5_geContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_exp(CACTParser::Float_expContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_4_3_none(CACTParser::Float_4_3_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_4_3_minus(CACTParser::Float_4_3_minusContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_4_3_plus(CACTParser::Float_4_3_plusContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_3_2_times(CACTParser::Float_3_2_timesContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_3_2_divide(CACTParser::Float_3_2_divideContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_3_2_none(CACTParser::Float_3_2_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_2_1_plus(CACTParser::Float_2_1_plusContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_2_1_minus(CACTParser::Float_2_1_minusContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_2_1_none(CACTParser::Float_2_1_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_1_4_braket(CACTParser::Float_1_4_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_1_float_const(CACTParser::Float_1_float_constContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_1_func_call(CACTParser::Float_1_func_callContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_1_ident(CACTParser::Float_1_identContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_double_6_eq(CACTParser::Comp_double_6_eqContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_double_6_ne(CACTParser::Comp_double_6_neContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_double_5_lt(CACTParser::Comp_double_5_ltContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_double_5_gt(CACTParser::Comp_double_5_gtContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_double_5_le(CACTParser::Comp_double_5_leContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_double_5_ge(CACTParser::Comp_double_5_geContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_exp(CACTParser::Double_expContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_4_3_minus(CACTParser::Double_4_3_minusContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_4_3_none(CACTParser::Double_4_3_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_4_3_plus(CACTParser::Double_4_3_plusContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_3_2_none(CACTParser::Double_3_2_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_3_2_times(CACTParser::Double_3_2_timesContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_3_2_divide(CACTParser::Double_3_2_divideContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_2_1_plus(CACTParser::Double_2_1_plusContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_2_1_minus(CACTParser::Double_2_1_minusContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_2_1_none(CACTParser::Double_2_1_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_1_4_braket(CACTParser::Double_1_4_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_1_double_const(CACTParser::Double_1_double_constContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_1_func_call(CACTParser::Double_1_func_callContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_1_ident(CACTParser::Double_1_identContext *context)
	override { return nullptr;
		
	}

    std::any visitDec_const(CACTParser::Dec_constContext *context)
	override { return nullptr;
		
	}

    std::any visitOct_const(CACTParser::Oct_constContext *context)
	override { return nullptr;
		
	}

    std::any visitHex_const(CACTParser::Hex_constContext *context)
	override { return nullptr;
		
	}

    std::any visitPlus_int_const(CACTParser::Plus_int_constContext *context)
	override { return nullptr;
		
	}

    std::any visitMinus_int_const(CACTParser::Minus_int_constContext *context)
	override { return nullptr;
		
	}

    std::any visitNone_int_const(CACTParser::None_int_constContext *context)
	override { return nullptr;
		
	}

    std::any visitTrue_const(CACTParser::True_constContext *context)
	override { return nullptr;
		
	}

    std::any visitFalse_const(CACTParser::False_constContext *context)
	override { return nullptr;
		
	}

    std::any visitErrorNode(tree::ErrorNode * node) override {
        std::cout << "visit error node!" << std::endl;
        return nullptr;
    }
};

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

  Analyzer visitor;
  visitor.visit(parser.comp_units());

    if (lexer.getNumberOfSyntaxErrors() > 0) {
        std::cout << "lex error: " << lexer.getNumberOfSyntaxErrors() << std::endl;
        std::cout << "False" << std::endl;
        return 1;
    }

    if (parser.getNumberOfSyntaxErrors() > 0) {
        std::cout << "syntax error: " << parser.getNumberOfSyntaxErrors() << std::endl;
        std::cout << "False" << std::endl;
        return 2;
    }

	std::cout << "=== End ===" << std::endl;
  	std::cout << "Number of errors: " << lexer.getNumberOfSyntaxErrors() + parser.getNumberOfSyntaxErrors() << std::endl;

    std::cout << "True" << std::endl;

    return parser.getNumberOfSyntaxErrors() + lexer.getNumberOfSyntaxErrors();
}