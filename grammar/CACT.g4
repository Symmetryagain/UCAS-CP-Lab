grammar CACT;

@header {
    #include <vector>
}

comp_units	: comp_unit comp_units	# comp_units_more
			| EOF					# comp_units_empty
			;
comp_unit	: c_decl 			# comp_unit_const_decl
			| v_decl			# comp_unit_var_decl
			| func_def			# comp_unit_func
			;
c_decl		: const_int_decl	# decl_const_int
			| const_bool_decl	# decl_const_bool
			| const_float_decl	# decl_const_float
			| const_double_decl # decl_const_double
			;
v_decl		: int_decl			# decl_int
			| bool_decl			# decl_bool
			| float_decl		# decl_float
			| double_decl		# decl_double
			;
func_def	: func_int_def		# def_int_func
			| func_bool_def		# def_bool_func
			| func_float_def	# def_float_func
			| func_double_def	# def_double_func
			| func_void_def		# def_void_func
			;
func_int_def	: INT    Ident SBra func_f_params? SKet block_int		;
func_bool_def	: BOOL   Ident SBra func_f_params? SKet block_bool		;
func_float_def	: FLOAT  Ident SBra func_f_params? SKet block_float		;
func_double_def	: DOUBLE Ident SBra func_f_params? SKet block_double	;
func_void_def	: VOID   Ident SBra func_f_params? SKet block_void		;

func_f_params		: func_f_param (Comma func_f_param)*				;
func_f_param		: INT    Ident (MBra MKet)? (MBra int_const MKet)*	# func_f_param_int
					| BOOL   Ident (MBra MKet)? (MBra int_const MKet)*	# func_f_param_bool
					| FLOAT  Ident (MBra MKet)? (MBra int_const MKet)*	# func_f_param_float
					| DOUBLE Ident (MBra MKet)? (MBra int_const MKet)*	# func_f_param_double
					;
func_call			: Ident SBra func_r_params? SKet					;
func_r_params		: func_r_param (Comma func_r_param)*				;
func_r_param		: exp	;

const_int_decl			: CONST INT    const_int_def    (Comma const_int_def   )* Semi;
const_bool_decl			: CONST BOOL   const_bool_def   (Comma const_bool_def  )* Semi;
const_float_decl		: CONST FLOAT  const_float_def  (Comma const_float_def )* Semi;
const_double_decl		: CONST DOUBLE const_double_def (Comma const_double_def)* Semi;

const_int_def			: const_int_var_def		# def_const_int_var
						| const_int_arr_def		# def_const_int_arr
						; 
const_bool_def			: const_bool_var_def	# def_const_bool_var
						| const_bool_arr_def	# def_const_bool_arr
						;
const_float_def			: const_float_var_def	# def_const_float_var
						| const_float_arr_def	# def_const_float_arr
						;
const_double_def		: const_double_var_def	# def_const_double_var
						| const_double_arr_def	# def_const_double_arr
						;

const_int_var_def		: Ident Assign sign_int_const		;
const_bool_var_def		: Ident Assign bool_const			;
const_float_var_def		: Ident Assign sign_float_const		;
const_double_var_def	: Ident Assign sign_double_const	;

const_int_arr_def		: Ident(MBra int_const MKet)+ Assign int_arr_init_braket		;
const_bool_arr_def		: Ident(MBra int_const MKet)+ Assign bool_arr_init_braket	;
const_float_arr_def		: Ident(MBra int_const MKet)+ Assign float_arr_init_braket	;
const_double_arr_def	: Ident(MBra int_const MKet)+ Assign double_arr_init_braket	;

int_arr_init_braket		: LBra int_arr_init_list? LKet		;
bool_arr_init_braket	: LBra bool_arr_init_list? LKet		;
float_arr_init_braket	: LBra float_arr_init_list? LKet	;
double_arr_init_braket	: LBra double_arr_init_list? LKet	;

int_arr_init_list			: int_arr_init_list_elem    (Comma int_arr_init_list_elem   )*	;
int_arr_init_list_elem		: sign_int_const			# int_arr_init_list_elem_int
							| int_arr_init_braket		# int_arr_init_list_elem_arr
							;
bool_arr_init_list			: bool_arr_init_list_elem	(Comma bool_arr_init_list_elem  )*	;
bool_arr_init_list_elem		: bool_const				# bool_arr_init_list_elem_bool
							| bool_arr_init_braket		# bool_arr_init_list_elem_arr
							;
float_arr_init_list			: float_arr_init_list_elem	(Comma float_arr_init_list_elem )*	;
float_arr_init_list_elem	: sign_float_const			# float_arr_init_list_elem_float
							| float_arr_init_braket		# float_arr_init_list_elem_arr
							;
double_arr_init_list		: double_arr_init_list_elem	(Comma double_arr_init_list_elem)*	;
double_arr_init_list_elem	: sign_double_const			# double_arr_init_list_elem_double
							| double_arr_init_braket	# double_arr_init_list_elem_arr
							;

int_decl		: INT int_def       (Comma int_def   )* Semi	;
bool_decl		: BOOL bool_def     (Comma bool_def  )* Semi	;
float_decl		: FLOAT float_def   (Comma float_def )* Semi	;
double_decl		: DOUBLE double_def (Comma double_def)* Semi	;

int_def			: int_var_def		# def_int_var
				| int_arr_def		# def_int_arr
				;
bool_def		: bool_var_def		# def_bool_var
				| bool_arr_def		# def_bool_arr
				;
float_def		: float_var_def		# def_float_var
				| float_arr_def		# def_float_arr
				;
double_def		: double_var_def	# def_double_var
				| double_arr_def	# def_double_arr
				;

int_var_def		: Ident	(Assign sign_int_const   )?	;
bool_var_def	: Ident	(Assign bool_const      )?	;
float_var_def	: Ident	(Assign sign_float_const )?	;
double_var_def	: Ident	(Assign sign_double_const)?	;

int_arr_def		: Ident(MBra int_const MKet)+ (Assign int_arr_init_braket   )?	;
bool_arr_def	: Ident(MBra int_const MKet)+ (Assign bool_arr_init_braket  )?	;
float_arr_def	: Ident(MBra int_const MKet)+ (Assign float_arr_init_braket )?	;
double_arr_def	: Ident(MBra int_const MKet)+ (Assign double_arr_init_braket)?	;

block_int		: LBra item_int* LKet	;
block_void		: LBra item_void* LKet	;
block_bool		: LBra item_bool* LKet	;
block_float		: LBra item_float* LKet	;
block_double	: LBra item_double* LKet	;

item			: c_decl	# item_const_decl
				| v_decl	# item_var_decl
				| exp Semi	# item_exp
				| assign	# item_assign
				| Semi		# item_none
				;
item_int		: item 			# item_int_item
				| block_int    	# item_int_block
				| return_int    # item_int_return
				| if_int    	# item_int_if
				| while_int		# item_int_while
				;
item_bool		: item 			# item_bool_item
				| block_bool   	# item_bool_block
				| return_bool   # item_bool_return
				| if_bool   	# item_bool_if
				| while_bool	# item_bool_while
				;
item_float		: item 			# item_float_item
				| block_float  	# item_float_block
				| return_float  # item_float_return
				| if_float  	# item_float_if
				| while_float 	# item_float_while
				;
item_double		: item 				# item_double_item
				| block_double 		# item_double_block
				| return_double 	# item_double_return
				| if_double 		# item_double_if
				| while_double		# item_double_while
				;
item_void		: item 			# item_void_item
				| block_void   	# item_void_block
				| return_void   # item_void_return
				| if_void   	# item_void_if
				| while_void	# item_void_while
				;

return_int		: RETURN int_exp    Semi	;
return_void		: RETURN            Semi	;
return_bool		: RETURN bool_exp   Semi	;
return_float	: RETURN float_exp  Semi	;
return_double	: RETURN double_exp Semi	;

if_int			: IF SBra bool_exp SKet block_int    (ELSE block_int)?		;
if_void			: IF SBra bool_exp SKet block_void   (ELSE block_void)?		;
if_bool			: IF SBra bool_exp SKet block_bool   (ELSE block_bool)?		;
if_float		: IF SBra bool_exp SKet block_float  (ELSE block_float)?	;
if_double		: IF SBra bool_exp SKet block_double (ELSE block_double)?	;

while_int		: WHILE SBra bool_exp SKet block_while_int		;
while_void		: WHILE SBra bool_exp SKet block_while_void		;
while_bool		: WHILE SBra bool_exp SKet block_while_bool		;
while_float		: WHILE SBra bool_exp SKet block_while_float	;
while_double	: WHILE SBra bool_exp SKet block_while_double	;

block_while_int		: LBra item_while_int*    LKet	;
block_while_void	: LBra item_while_void*   LKet	;
block_while_bool	: LBra item_while_bool*   LKet	;
block_while_float	: LBra item_while_float*  LKet	;
block_while_double	: LBra item_while_double* LKet	;

item_while			: BREAK Semi 	# item_while_break
					| CONTINUE Semi	# item_while_continue
					;

item_while_int		: item_while 	# item_while_int_while
					| item_int		# item_while_int_item
					;
item_while_void		: item_while 	# item_while_void_while
					| item_void		# item_while_void_item
					;
item_while_bool		: item_while 	# item_while_bool_while
					| item_bool		# item_while_bool_item
					;
item_while_float	: item_while 	# item_while_float_while
					| item_float	# item_while_float_item
					;
item_while_double	: item_while 	# item_while_double_while
					| item_double	# item_while_double_item
					;

assign	: int_assign 		# assign_int 
		| bool_assign 		# assign_bool 
		| float_assign 		# assign_float 
		| double_assign 	# assign_double 
		;

int_assign		: Ident(MBra int_exp MKet)* Assign int_exp    Semi	;
bool_assign		: Ident(MBra int_exp MKet)* Assign bool_exp   Semi	;
float_assign	: Ident(MBra int_exp MKet)* Assign float_exp  Semi	;
double_assign	: Ident(MBra int_exp MKet)* Assign double_exp Semi	;

exp			: void_exp 		# exp_void
			| bool_exp 		# exp_bool
			| int_exp 		# exp_int
			| float_exp 	# exp_float
			| double_exp	# exp_double
			;
void_exp	: func_call	;
bool_exp	:                 bool_exp_8		;
bool_exp_8	: bool_exp_8 DOr  bool_exp_7	# bool_8_7_or
			|                 bool_exp_7	# bool_8_7_none
			;
bool_exp_7	: bool_exp_7 DAnd bool_exp_6	# bool_7_6_and
			|                 bool_exp_6	# bool_7_6_none
			;
bool_exp_6	: bool_exp_6 EQ   bool_exp_5	# bool_6_5_eq
			| bool_exp_6 NE   bool_exp_5	# bool_6_5_ne
			|                 bool_exp_5	# bool_6_5_none
			|                 comp_int_6	# bool_6_5_comp_int
			|                 comp_float_6	# bool_6_5_comp_float
			|                 comp_double_6	# bool_6_5_comp_double
			;
bool_exp_5	:	              bool_exp_2	# bool_5_2_not
			|				  comp_int_5	# bool_5_2_comp_int
			|				  comp_float_5	# bool_5_2_comp_float
			|				  comp_double_5	# bool_5_2_comp_double
			;
bool_exp_2	:             Not bool_exp_2	# bool_2_1_not
			|                 bool_exp_1	# bool_2_1_none
			;
bool_exp_1	:            SBra bool_exp_8 SKet	# bool_1_8_braket
			| bool_const 						# bool_1_bool_const
			| func_call 						# bool_1_func_call
			| Ident								# bool_1_ident
			| bool_exp_1   MBra int_exp  MKet	# bool_1_array_index
			;

comp_int_6		: int_exp EQ int_exp		# comp_int_6_eq
				| int_exp NE int_exp		# comp_int_6_ne
				;
comp_int_5		: int_exp LT int_exp		# comp_int_5_lt
				| int_exp GT int_exp		# comp_int_5_gt
				| int_exp LE int_exp		# comp_int_5_le
				| int_exp GE int_exp		# comp_int_5_ge
				;
int_exp		:                  int_exp_4		;
int_exp_4	: int_exp_4 Plus   int_exp_3	# int_4_3_plus
			| int_exp_4 Minus  int_exp_3	# int_4_3_minus
			|                  int_exp_3	# int_4_3_none
			;
int_exp_3	: int_exp_3 Times  int_exp_2	# int_3_2_times
			| int_exp_3 Divide int_exp_2	# int_3_2_divide
			| int_exp_3 Remain int_exp_2	# int_3_2_remain
			|                  int_exp_2	# int_3_2_none
			;
int_exp_2	:            Plus  int_exp_2	# int_2_1_plus
			|            Minus int_exp_2	# int_2_1_minus
			|                  int_exp_1	# int_2_1_none
			;
int_exp_1	:             SBra int_exp_4 SKet	# int_1_4_braket
			| int_const 						# int_1_int_const
			| func_call 						# int_1_func_call
			| Ident								# int_1_ident
			| int_exp_1   MBra int_exp   MKet	# int_1_array_index
			;

comp_float_6	: float_exp EQ float_exp		# comp_float_6_eq
				| float_exp NE float_exp		# comp_float_6_ne
				;
comp_float_5	: float_exp LT float_exp		# comp_float_5_lt
				| float_exp GT float_exp		# comp_float_5_gt
				| float_exp LE float_exp		# comp_float_5_le
				| float_exp GE float_exp		# comp_float_5_ge
				;
float_exp		:              		 float_exp_4	;
float_exp_4		: float_exp_4 Plus   float_exp_3	# float_4_3_plus
				| float_exp_4 Minus  float_exp_3	# float_4_3_minus
				|              		 float_exp_3	# float_4_3_none
				;
float_exp_3		: float_exp_3 Times  float_exp_2	# float_3_2_times
				| float_exp_3 Divide float_exp_2	# float_3_2_divide
				|		 		     float_exp_2	# float_3_2_none
				;
float_exp_2		:              Plus  float_exp_2	# float_2_1_plus
				|              Minus float_exp_2	# float_2_1_minus
				|		 	    	 float_exp_1	# float_2_1_none
				;
float_exp_1		:               SBra float_exp_4 SKet	# float_1_4_braket
				| FloatConst 							# float_1_float_const
				| func_call 							# float_1_func_call
				| Ident									# float_1_ident
				| float_exp_1   MBra int_exp     MKet	# float_1_array_index
				;

comp_double_6	: double_exp EQ double_exp		# comp_double_6_eq
				| double_exp NE double_exp		# comp_double_6_ne
				;
comp_double_5	: double_exp LT double_exp		# comp_double_5_lt
				| double_exp GT double_exp		# comp_double_5_gt
				| double_exp LE double_exp		# comp_double_5_le
				| double_exp GE double_exp		# comp_double_5_ge
				;
double_exp		:              		  double_exp_4	;
double_exp_4	: double_exp_4 Plus   double_exp_3	# double_4_3_plus
				| double_exp_4 Minus  double_exp_3	# double_4_3_minus
				|              		  double_exp_3	# double_4_3_none
				;
double_exp_3	: double_exp_3 Times  double_exp_2	# double_3_2_times
				| double_exp_3 Divide double_exp_2	# double_3_2_divide
				|		 		      double_exp_2	# double_3_2_none
				;
double_exp_2	:              Plus   double_exp_2	# double_2_1_plus
				|              Minus  double_exp_2	# double_2_1_minus
				|		 	    	  double_exp_1	# double_2_1_none
				;
double_exp_1	:                SBra double_exp_4 SKet	# double_1_4_braket
				| DoubleConst 							# double_1_double_const
				| func_call 							# double_1_func_call
				| Ident									# double_1_ident
				| double_exp_1   MBra int_exp      MKet	# double_1_array_index
				;

int_const		: DecConst 			# dec_const
				| OctConst 			# oct_const
				| HexConst			# hex_const
				;

sign_int_const		: Plus  int_const	# plus_int_const
					| Minus int_const	# minus_int_const
					|       int_const	# none_int_const
					;
bool_const			: TRUE 				# true_const
					| FALSE				# false_const
					;
sign_float_const	: Plus  FloatConst		
					| Minus FloatConst
					|       FloatConst		
					;

sign_double_const	: Plus  DoubleConst		
					| Minus DoubleConst
					|       DoubleConst		
					;

TRUE		: 'true'		;
FALSE		: 'false'		;
CONST		: 'const'		;
INT			: 'int'			;
BOOL		: 'bool'		;
FLOAT		: 'float'		;
DOUBLE		: 'double'		;
VOID		: 'void'		;
IF			: 'if'			;
ELSE		: 'else'		;
WHILE		: 'while'		;
BREAK		: 'break'		;
CONTINUE	: 'continue'	;
RETURN		: 'return'		;

// const
FloatConst		: [0-9]+ '.' [0-9]+                   [Ff]
				| [0-9]+ '.'                          [Ff]
				|        '.' [0-9]+                   [Ff]
				| [0-9]+ '.' [0-9]+ [Ee] [+-]? [0-9]+ [Ff]
				| [0-9]+ '.'        [Ee] [+-]? [0-9]+ [Ff]
				|        '.' [0-9]+ [Ee] [+-]? [0-9]+ [Ff]
				| [0-9]+            [Ee] [+-]? [0-9]+ [Ff]
				;

DoubleConst		: [0-9]+ '.' [0-9]+                   
				| [0-9]+ '.'                          
				|        '.' [0-9]+                   
				| [0-9]+ '.' [0-9]+ [Ee] [+-]? [0-9]+ 
				| [0-9]+ '.'        [Ee] [+-]? [0-9]+ 
				|        '.' [0-9]+ [Ee] [+-]? [0-9]+ 
				| [0-9]+            [Ee] [+-]? [0-9]+ 
				;
HexConst	: [0][x|X][0-9A-Fa-f]+	;
DecConst	: [1-9]   [0-9]*		;
OctConst	: [0]     [0-7]*		;

Ident			: [A-Za-z_][0-9A-Za-z_]*	;

// hidden

LineComment		: '//' ~[\r\n]*		-> channel(HIDDEN)	;
BlockComment	: '/*' .*? '*/'		-> channel(HIDDEN)	;
NewLine			:('\r'|'\n'|'\r\n')	-> channel(HIDDEN)	;
WhiteSpace		:('\t'|' ')			-> channel(HIDDEN)	;

// operators

SBra	: '('	;
SKet	: ')'	;
MBra	: '['	;
MKet	: ']'	;
LBra	: '{'	;
LKet	: '}'	;
DAnd	: '&&'	;
DOr		: '||'	;
Times	: '*'	; 
Plus	: '+'	;
Minus	: '-'	;
Not		: '!'	;
Divide	: '/'	;
Remain	: '%'	;
LT		: '<'	;
GT		: '>'	;
LE		: '<='	;
GE		: '>='	;
EQ		: '=='	;
NE		: '!='	;
Assign	: '='	;
Semi	: ';'	;
Comma	: ','	;
