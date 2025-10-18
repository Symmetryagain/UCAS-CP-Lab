#include <iostream>

// #include "antlr4-runtime.h"
#include "tree/ErrorNode.h"

#include "HelloLexer.h"
#include "HelloParser.h"
#include "HelloVisitor.h"

using namespace antlr4;

class Analyzer : public HelloVisitor {
public:
	std::any visitComp_units_more(HelloParser::Comp_units_moreContext *context)
	override { return nullptr;

	}

	std::any visitComp_units_empty(HelloParser::Comp_units_emptyContext *context)
	override { return nullptr;

	}

    std::any visitComp_unit_const_decl(HelloParser::Comp_unit_const_declContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_unit_var_decl(HelloParser::Comp_unit_var_declContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_unit_func(HelloParser::Comp_unit_funcContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_const_int(HelloParser::Decl_const_intContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_const_bool(HelloParser::Decl_const_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_const_float(HelloParser::Decl_const_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_const_double(HelloParser::Decl_const_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_int(HelloParser::Decl_intContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_bool(HelloParser::Decl_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_float(HelloParser::Decl_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitDecl_double(HelloParser::Decl_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_int_func(HelloParser::Def_int_funcContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_bool_func(HelloParser::Def_bool_funcContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_float_func(HelloParser::Def_float_funcContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_double_func(HelloParser::Def_double_funcContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_void_func(HelloParser::Def_void_funcContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_int_def(HelloParser::Func_int_defContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_bool_def(HelloParser::Func_bool_defContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_float_def(HelloParser::Func_float_defContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_double_def(HelloParser::Func_double_defContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_void_def(HelloParser::Func_void_defContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_f_params(HelloParser::Func_f_paramsContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_f_param_int(HelloParser::Func_f_param_intContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_f_param_bool(HelloParser::Func_f_param_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_f_param_float(HelloParser::Func_f_param_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_f_param_double(HelloParser::Func_f_param_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_call(HelloParser::Func_callContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_r_params(HelloParser::Func_r_paramsContext *context)
	override { return nullptr;
		
	}

    std::any visitFunc_r_param(HelloParser::Func_r_paramContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_int_decl(HelloParser::Const_int_declContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_bool_decl(HelloParser::Const_bool_declContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_float_decl(HelloParser::Const_float_declContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_double_decl(HelloParser::Const_double_declContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_int_var(HelloParser::Def_const_int_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_int_arr(HelloParser::Def_const_int_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_bool_var(HelloParser::Def_const_bool_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_bool_arr(HelloParser::Def_const_bool_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_float_var(HelloParser::Def_const_float_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_float_arr(HelloParser::Def_const_float_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_double_var(HelloParser::Def_const_double_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_const_double_arr(HelloParser::Def_const_double_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_int_var_def(HelloParser::Const_int_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_bool_var_def(HelloParser::Const_bool_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_float_var_def(HelloParser::Const_float_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_double_var_def(HelloParser::Const_double_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_int_arr_def(HelloParser::Const_int_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_bool_arr_def(HelloParser::Const_bool_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_float_arr_def(HelloParser::Const_float_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitConst_double_arr_def(HelloParser::Const_double_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_arr_init_braket(HelloParser::Int_arr_init_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_arr_init_braket(HelloParser::Bool_arr_init_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_arr_init_braket(HelloParser::Float_arr_init_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_arr_init_braket(HelloParser::Double_arr_init_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_arr_init_list(HelloParser::Int_arr_init_listContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_arr_init_list_elem_int(HelloParser::Int_arr_init_list_elem_intContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_arr_init_list_elem_arr(HelloParser::Int_arr_init_list_elem_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_arr_init_list(HelloParser::Bool_arr_init_listContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_arr_init_list_elem_bool(HelloParser::Bool_arr_init_list_elem_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_arr_init_list_elem_arr(HelloParser::Bool_arr_init_list_elem_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_arr_init_list(HelloParser::Float_arr_init_listContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_arr_init_list_elem_float(HelloParser::Float_arr_init_list_elem_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_arr_init_list_elem_arr(HelloParser::Float_arr_init_list_elem_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_arr_init_list(HelloParser::Double_arr_init_listContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_arr_init_list_elem_double(HelloParser::Double_arr_init_list_elem_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_arr_init_list_elem_arr(HelloParser::Double_arr_init_list_elem_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_decl(HelloParser::Int_declContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_decl(HelloParser::Bool_declContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_decl(HelloParser::Float_declContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_decl(HelloParser::Double_declContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_int_var(HelloParser::Def_int_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_int_arr(HelloParser::Def_int_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_bool_var(HelloParser::Def_bool_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_bool_arr(HelloParser::Def_bool_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_float_var(HelloParser::Def_float_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_float_arr(HelloParser::Def_float_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_double_var(HelloParser::Def_double_varContext *context)
	override { return nullptr;
		
	}

    std::any visitDef_double_arr(HelloParser::Def_double_arrContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_var_def(HelloParser::Int_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_var_def(HelloParser::Bool_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_var_def(HelloParser::Float_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_var_def(HelloParser::Double_var_defContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_arr_def(HelloParser::Int_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_arr_def(HelloParser::Bool_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_arr_def(HelloParser::Float_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_arr_def(HelloParser::Double_arr_defContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_int(HelloParser::Block_intContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_void(HelloParser::Block_voidContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_bool(HelloParser::Block_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_float(HelloParser::Block_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_double(HelloParser::Block_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_const_decl(HelloParser::Item_const_declContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_var_decl(HelloParser::Item_var_declContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_exp(HelloParser::Item_expContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_assign(HelloParser::Item_assignContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_none(HelloParser::Item_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_int_item(HelloParser::Item_int_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_int_block(HelloParser::Item_int_blockContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_int_return(HelloParser::Item_int_returnContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_int_if(HelloParser::Item_int_ifContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_int_while(HelloParser::Item_int_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_bool_item(HelloParser::Item_bool_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_bool_block(HelloParser::Item_bool_blockContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_bool_return(HelloParser::Item_bool_returnContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_bool_if(HelloParser::Item_bool_ifContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_bool_while(HelloParser::Item_bool_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_float_item(HelloParser::Item_float_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_float_block(HelloParser::Item_float_blockContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_float_return(HelloParser::Item_float_returnContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_float_if(HelloParser::Item_float_ifContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_float_while(HelloParser::Item_float_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_double_item(HelloParser::Item_double_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_double_block(HelloParser::Item_double_blockContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_double_return(HelloParser::Item_double_returnContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_double_if(HelloParser::Item_double_ifContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_double_while(HelloParser::Item_double_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_void_item(HelloParser::Item_void_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_void_block(HelloParser::Item_void_blockContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_void_return(HelloParser::Item_void_returnContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_void_if(HelloParser::Item_void_ifContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_void_while(HelloParser::Item_void_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitReturn_int(HelloParser::Return_intContext *context)
	override { return nullptr;
		
	}

    std::any visitReturn_void(HelloParser::Return_voidContext *context)
	override { return nullptr;
		
	}

    std::any visitReturn_bool(HelloParser::Return_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitReturn_float(HelloParser::Return_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitReturn_double(HelloParser::Return_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitIf_int(HelloParser::If_intContext *context)
	override { return nullptr;
		
	}

    std::any visitIf_void(HelloParser::If_voidContext *context)
	override { return nullptr;
		
	}

    std::any visitIf_bool(HelloParser::If_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitIf_float(HelloParser::If_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitIf_double(HelloParser::If_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitWhile_int(HelloParser::While_intContext *context)
	override { return nullptr;
		
	}

    std::any visitWhile_void(HelloParser::While_voidContext *context)
	override { return nullptr;
		
	}

    std::any visitWhile_bool(HelloParser::While_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitWhile_float(HelloParser::While_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitWhile_double(HelloParser::While_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_while_int(HelloParser::Block_while_intContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_while_void(HelloParser::Block_while_voidContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_while_bool(HelloParser::Block_while_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_while_float(HelloParser::Block_while_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitBlock_while_double(HelloParser::Block_while_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_break(HelloParser::Item_while_breakContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_continue(HelloParser::Item_while_continueContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_int_while(HelloParser::Item_while_int_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_int_item(HelloParser::Item_while_int_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_void_while(HelloParser::Item_while_void_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_void_item(HelloParser::Item_while_void_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_bool_while(HelloParser::Item_while_bool_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_bool_item(HelloParser::Item_while_bool_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_float_while(HelloParser::Item_while_float_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_float_item(HelloParser::Item_while_float_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_double_while(HelloParser::Item_while_double_whileContext *context)
	override { return nullptr;
		
	}

    std::any visitItem_while_double_item(HelloParser::Item_while_double_itemContext *context)
	override { return nullptr;
		
	}

    std::any visitAssign_int(HelloParser::Assign_intContext *context)
	override { return nullptr;
		
	}

    std::any visitAssign_bool(HelloParser::Assign_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitAssign_float(HelloParser::Assign_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitAssign_double(HelloParser::Assign_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_assign(HelloParser::Int_assignContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_assign(HelloParser::Bool_assignContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_assign(HelloParser::Float_assignContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_assign(HelloParser::Double_assignContext *context)
	override { return nullptr;
		
	}

    std::any visitExp_void(HelloParser::Exp_voidContext *context)
	override { return nullptr;
		
	}

    std::any visitExp_bool(HelloParser::Exp_boolContext *context)
	override { return nullptr;
		
	}

    std::any visitExp_int(HelloParser::Exp_intContext *context)
	override { return nullptr;
		
	}

    std::any visitExp_float(HelloParser::Exp_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitExp_double(HelloParser::Exp_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitVoid_exp(HelloParser::Void_expContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_exp(HelloParser::Bool_expContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_8_7_or(HelloParser::Bool_8_7_orContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_8_7_none(HelloParser::Bool_8_7_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_7_6_none(HelloParser::Bool_7_6_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_7_6_and(HelloParser::Bool_7_6_andContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_6_5_comp_double(HelloParser::Bool_6_5_comp_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_6_5_ne(HelloParser::Bool_6_5_neContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_6_5_none(HelloParser::Bool_6_5_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_6_5_comp_float(HelloParser::Bool_6_5_comp_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_6_5_eq(HelloParser::Bool_6_5_eqContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_6_5_comp_int(HelloParser::Bool_6_5_comp_intContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_5_2_not(HelloParser::Bool_5_2_notContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_5_2_comp_int(HelloParser::Bool_5_2_comp_intContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_5_2_comp_float(HelloParser::Bool_5_2_comp_floatContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_5_2_comp_double(HelloParser::Bool_5_2_comp_doubleContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_2_1_not(HelloParser::Bool_2_1_notContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_2_1_none(HelloParser::Bool_2_1_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_1_8_braket(HelloParser::Bool_1_8_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_1_bool_const(HelloParser::Bool_1_bool_constContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_1_func_call(HelloParser::Bool_1_func_callContext *context)
	override { return nullptr;
		
	}

    std::any visitBool_1_ident(HelloParser::Bool_1_identContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_int_6_eq(HelloParser::Comp_int_6_eqContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_int_6_ne(HelloParser::Comp_int_6_neContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_int_5_lt(HelloParser::Comp_int_5_ltContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_int_5_gt(HelloParser::Comp_int_5_gtContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_int_5_le(HelloParser::Comp_int_5_leContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_int_5_ge(HelloParser::Comp_int_5_geContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_exp(HelloParser::Int_expContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_4_3_plus(HelloParser::Int_4_3_plusContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_4_3_none(HelloParser::Int_4_3_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_4_3_minus(HelloParser::Int_4_3_minusContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_3_2_times(HelloParser::Int_3_2_timesContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_3_2_divide(HelloParser::Int_3_2_divideContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_3_2_remain(HelloParser::Int_3_2_remainContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_3_2_none(HelloParser::Int_3_2_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_2_1_plus(HelloParser::Int_2_1_plusContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_2_1_minus(HelloParser::Int_2_1_minusContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_2_1_none(HelloParser::Int_2_1_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_1_4_braket(HelloParser::Int_1_4_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_1_int_const(HelloParser::Int_1_int_constContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_1_func_call(HelloParser::Int_1_func_callContext *context)
	override { return nullptr;
		
	}

    std::any visitInt_1_ident(HelloParser::Int_1_identContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_float_6_eq(HelloParser::Comp_float_6_eqContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_float_6_ne(HelloParser::Comp_float_6_neContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_float_5_lt(HelloParser::Comp_float_5_ltContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_float_5_gt(HelloParser::Comp_float_5_gtContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_float_5_le(HelloParser::Comp_float_5_leContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_float_5_ge(HelloParser::Comp_float_5_geContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_exp(HelloParser::Float_expContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_4_3_none(HelloParser::Float_4_3_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_4_3_minus(HelloParser::Float_4_3_minusContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_4_3_plus(HelloParser::Float_4_3_plusContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_3_2_times(HelloParser::Float_3_2_timesContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_3_2_divide(HelloParser::Float_3_2_divideContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_3_2_none(HelloParser::Float_3_2_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_2_1_plus(HelloParser::Float_2_1_plusContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_2_1_minus(HelloParser::Float_2_1_minusContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_2_1_none(HelloParser::Float_2_1_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_1_4_braket(HelloParser::Float_1_4_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_1_float_const(HelloParser::Float_1_float_constContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_1_func_call(HelloParser::Float_1_func_callContext *context)
	override { return nullptr;
		
	}

    std::any visitFloat_1_ident(HelloParser::Float_1_identContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_double_6_eq(HelloParser::Comp_double_6_eqContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_double_6_ne(HelloParser::Comp_double_6_neContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_double_5_lt(HelloParser::Comp_double_5_ltContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_double_5_gt(HelloParser::Comp_double_5_gtContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_double_5_le(HelloParser::Comp_double_5_leContext *context)
	override { return nullptr;
		
	}

    std::any visitComp_double_5_ge(HelloParser::Comp_double_5_geContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_exp(HelloParser::Double_expContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_4_3_minus(HelloParser::Double_4_3_minusContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_4_3_none(HelloParser::Double_4_3_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_4_3_plus(HelloParser::Double_4_3_plusContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_3_2_none(HelloParser::Double_3_2_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_3_2_times(HelloParser::Double_3_2_timesContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_3_2_divide(HelloParser::Double_3_2_divideContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_2_1_plus(HelloParser::Double_2_1_plusContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_2_1_minus(HelloParser::Double_2_1_minusContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_2_1_none(HelloParser::Double_2_1_noneContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_1_4_braket(HelloParser::Double_1_4_braketContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_1_double_const(HelloParser::Double_1_double_constContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_1_func_call(HelloParser::Double_1_func_callContext *context)
	override { return nullptr;
		
	}

    std::any visitDouble_1_ident(HelloParser::Double_1_identContext *context)
	override { return nullptr;
		
	}

    std::any visitDec_const(HelloParser::Dec_constContext *context)
	override { return nullptr;
		
	}

    std::any visitOct_const(HelloParser::Oct_constContext *context)
	override { return nullptr;
		
	}

    std::any visitHex_const(HelloParser::Hex_constContext *context)
	override { return nullptr;
		
	}

    std::any visitPlus_int_const(HelloParser::Plus_int_constContext *context)
	override { return nullptr;
		
	}

    std::any visitMinus_int_const(HelloParser::Minus_int_constContext *context)
	override { return nullptr;
		
	}

    std::any visitNone_int_const(HelloParser::None_int_constContext *context)
	override { return nullptr;
		
	}

    std::any visitTrue_const(HelloParser::True_constContext *context)
	override { return nullptr;
		
	}

    std::any visitFalse_const(HelloParser::False_constContext *context)
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
    stream.open("../test/test.hello");
  }

  if (!stream.is_open()) {
    std::cerr << "Failed to open file" << std::endl;
    return 9;
  }

  ANTLRInputStream input(stream);
  HelloLexer lexer(&input);
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

  HelloParser parser(&tokens);

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