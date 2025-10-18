grammar Hello;

@header {
    #include <vector>
}

comp_unit	: (decl|func_def)+	;
decl        : const_int_decl
			| const_bool_decl
			| const_float_decl
			| const_double_decl
			| int_decl
			| bool_decl
			| float_decl
			| double_decl	;
func_def	: func_int_def
			| func_bool_def
			| func_float_def
			| func_double_def
			| func_void_def	;
func_int_def	: INT    Ident SBra func_f_params? SKet block_int		;
func_bool_def	: BOOL   Ident SBra func_f_params? SKet block_bool		;
func_float_def	: FLOAT  Ident SBra func_f_params? SKet block_float		;
func_double_def	: DOUBLE Ident SBra func_f_params? SKet block_double	;
func_void_def	: VOID   Ident SBra func_f_params? SKet block_void		;

func_f_params		: func_f_param (Comma func_f_param)*				;
func_f_param		: V_Type Ident (MBra MKet)? (MBra IntConst MKet)*	;
func_call			: Ident SBra func_r_params? SKet					;
func_r_params		: func_r_param (Comma func_r_param)*				;
func_r_param		: exp	;

const_int_decl			: CONST INT const_int_def (Comma const_int_def)* Semi	;
const_int_def			: const_int_var_def | const_int_arr_def	;
const_int_var_def		: Ident Assign SignIntConst	;
const_int_arr_def		: Ident(MBra IntConst MKet)+ Assign int_arr_init_braket;
int_arr_init_braket		: LBra int_arr_init_list? LKet	;
int_arr_init_list		: SignIntConst (Comma SignIntConst)*
						| int_arr_init_braket (Comma int_arr_init_braket)*	;

const_bool_decl			: CONST BOOL const_bool_def (Comma const_bool_def)* Semi;
const_bool_def			: const_bool_var_def | const_bool_arr_def	;
const_bool_var_def		: Ident Assign BoolConst	;
const_bool_arr_def		: Ident(MBra IntConst MKet)+ Assign bool_arr_init_braket;
bool_arr_init_braket	: LBra bool_arr_init_list? LKet	;
bool_arr_init_list		: BoolConst (Comma BoolConst)*
						| bool_arr_init_braket (Comma bool_arr_init_braket)*	;

const_float_decl		: CONST FLOAT const_float_def (Comma const_float_def)* Semi;
const_float_def			: const_float_var_def | const_float_arr_def	;
const_float_var_def		: Ident Assign SignFloatConst	;
const_float_arr_def		: Ident(MBra IntConst MKet)+ Assign float_arr_init_braket;
float_arr_init_braket	: LBra float_arr_init_list? LKet	;
float_arr_init_list		: SignFloatConst (Comma SignFloatConst)*	;

const_double_decl		: CONST DOUBLE const_double_def (Comma const_double_def)* Semi;
const_double_def		: const_double_var_def | const_double_arr_def	;
const_double_var_def	: Ident Assign SignDoubleConst	;
const_double_arr_def	: Ident(MBra IntConst MKet)+ Assign double_arr_init_braket;
double_arr_init_braket	: LBra double_arr_init_list? LKet	;
double_arr_init_list	: SignDoubleConst (Comma SignDoubleConst)*	;

bool_decl		: BOOL bool_def (Comma bool_def)* Semi	;
bool_def		: bool_var_def | bool_arr_def	;
bool_var_def	: Ident	(Assign BoolConst)?	;
bool_arr_def	: Ident(MBra IntConst MKet)+ (Assign bool_arr_init_braket)? Semi	;

int_decl		: INT int_def (Comma int_def)* Semi	;
int_def			: int_var_def | int_arr_def	;
int_var_def		: Ident	(Assign SignIntConst)?	;
int_arr_def		: Ident(MBra IntConst MKet)+ (Assign int_arr_init_braket)? Semi	;

float_decl		: FLOAT float_def (Comma float_def)* Semi	;
float_def		: float_var_def | float_arr_def	;
float_var_def	: Ident	(Assign SignFloatConst)?	;
float_arr_def	: Ident(MBra IntConst MKet)+ (Assign float_arr_init_braket)? Semi	;

double_decl		: DOUBLE double_def (Comma double_def)* Semi	;
double_def		: double_var_def | double_arr_def	;
double_var_def	: Ident	(Assign SignDoubleConst)?	;
double_arr_def	: Ident(MBra IntConst MKet)+ (Assign double_arr_init_braket)? Semi	;

block_int		: LBra item_int* LKet	;
block_void		: LBra item_void* LKet	;
block_bool		: LBra item_bool* LKet	;
block_float		: LBra item_float* LKet	;
block_double	: LBra item_double* LKet	;

item			: decl | exp? Semi | assign ;
item_int		: item | block_int    | return_int    | if_int    | while_int		;
item_bool		: item | block_bool   | return_bool   | if_bool   | while_bool		;
item_float		: item | block_float  | return_float  | if_float  | while_float		;
item_double		: item | block_double | return_double | if_double | while_double	;
item_void		: item | block_void   | return_void   | if_void   | while_void		;

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

item_while			: BREAK Semi | CONTINUE Semi	;

item_while_int		: item_while | item_int		;
item_while_void		: item_while | item_void	;
item_while_bool		: item_while | item_bool	;
item_while_float	: item_while | item_float	;
item_while_double	: item_while | item_double	;

assign	: int_assign | bool_assign | float_assign | double_assign ;

int_assign		: Ident(MBra int_exp MKet)* Assign int_exp    Semi	;
bool_assign		: Ident(MBra int_exp MKet)* Assign bool_exp   Semi	;
float_assign	: Ident(MBra int_exp MKet)* Assign float_exp  Semi	;
double_assign	: Ident(MBra int_exp MKet)* Assign double_exp Semi	;

exp			: void_exp | bool_exp | int_exp | float_exp | double_exp	;
void_exp	: func_call	;
bool_exp	:                 bool_exp_8		;
bool_exp_8	: bool_exp_8 DOr  bool_exp_7
			|                 bool_exp_7		;
bool_exp_7	: bool_exp_7 DAnd bool_exp_6	
			|                 bool_exp_6		;
bool_exp_6	: bool_exp_6 EQ   bool_exp_2
			| bool_exp_6 NE   bool_exp_2
			|                 bool_exp_2
			|                 comp_int_6
			|                 comp_float_6
			|                 comp_double_6	;
bool_exp_2	:             Not bool_exp_2
			|                 bool_exp_1		;
bool_exp_1	:            SBra bool_exp_8 SKet
			| BoolConst | func_call | Ident		;

comp_int_6		: int_exp LT int_exp	
				| int_exp GT int_exp	
				| int_exp LE int_exp	
				| int_exp GE int_exp		
				| int_exp EQ int_exp
				| int_exp NE int_exp		;
int_exp		:                  int_exp_4		;
int_exp_4	: int_exp_4 Plus   int_exp_3
			| int_exp_4 Minus  int_exp_3		
			|                  int_exp_3		;
int_exp_3	: int_exp_3 Times  int_exp_2
			| int_exp_3 Divide int_exp_2
			| int_exp_3 Remain int_exp_2		
			|                  int_exp_2		;
int_exp_2	:            Plus  int_exp_2
			|            Minus int_exp_2
			|                  int_exp_1		;
int_exp_1	:             SBra int_exp_4 SKet
			| IntConst | func_call | Ident		;

comp_float_6	: float_exp LT float_exp	
				| float_exp GT float_exp	
				| float_exp LE float_exp	
				| float_exp GE float_exp	
				| float_exp EQ float_exp
				| float_exp NE float_exp			;
float_exp		:                    float_exp_4	;
float_exp_4		: float_exp_4 Plus   float_exp_3
				| float_exp_4 Minus  float_exp_3	
				|                    float_exp_3	;
float_exp_3		: float_exp_3 Times  float_exp_2
				| float_exp_3 Divide float_exp_2	
				|                    float_exp_2	;
float_exp_2		:              Plus  float_exp_2
				|              Minus float_exp_2
				|                    float_exp_1	;
float_exp_1		:               SBra float_exp_4 SKet
				| FloatConst | func_call | Ident	;

comp_double_6	: double_exp LT double_exp	
				| double_exp GT double_exp	
				| double_exp LE double_exp	
				| double_exp GE double_exp	
				| double_exp EQ double_exp
				| double_exp NE double_exp				;
double_exp		:                     double_exp_4		;
double_exp_4	: double_exp_4 Plus   double_exp_3
				| double_exp_4 Minus  double_exp_3		
				|                     double_exp_3		;
double_exp_3	: double_exp_3 Times  double_exp_2
				| double_exp_3 Divide double_exp_2		
				|                     double_exp_2		;
double_exp_2	:               Plus  double_exp_2
				|               Minus double_exp_2
				|                     double_exp_1		;
double_exp_1	:                SBra double_exp_4 SKet
				| DoubleConst | func_call | Ident		;

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

BoolConst	: TRUE 
			| FALSE					;

DecConst	: [1-9]   [0-9]*		;
OctConst	: [0]     [0-7]*		;
HexConst	: [0][x|X][0-9A-Fa-f]+	;
IntConst	: DecConst 
			| OctConst 
			| HexConst				;
SignIntConst	: [+|-] IntConst	;

FractConst	: [0-9]*'.'[0-9]+		
			| [0-9]+'.'				;
ExpPart		: [E|e] [+|-]+ [0-9]+	;
FloatConst	: FractConst ExpPart+ [F|f]		
			| [0-9]+     ExpPart  [F|f]	;
SignFloatConst	: [+|-] FloatConst	;
DoubleConst	: FractConst ExpPart+		
			| [0-9]+     ExpPart		;
SignDoubleConst	: [+|-] DoubleConst	;

Ident		: [A-Za-z_][0-9A-Za-z_]*	;
V_Type		: INT | BOOL | FLOAT | DOUBLE ;

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
