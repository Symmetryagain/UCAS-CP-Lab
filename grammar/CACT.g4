grammar CACT;

@header {
#include <vector>
#include <string>
#include <variant>
#include "btype.h"
#include "func_table.h"
#include "sym_table.h"
}

// Lexical Keywords

TRUE    : 'true'    ;
FALSE   : 'false'   ;
CONST   : 'const'   ;
INT     : 'int'     ;
BOOL    : 'bool'    ;
FLOAT   : 'float'   ;
DOUBLE  : 'double'  ;
VOID    : 'void'    ;
IF      : 'if'      ;
ELSE    : 'else'    ;
WHILE   : 'while'   ;
BREAK   : 'break'   ;
CONTINUE: 'continue';
RETURN  : 'return'  ;

// Lexical Identifier

Ident   : [A-Za-z_][0-9A-Za-z_]*  ;

// Lexical Constant

HexConst  : '0' [xX][0-9A-Fa-f]+  ;
DecConst  : [1-9]   [0-9]*    ;
OctConst  : '0'     [0-7]*    ;

FloatConst    : [0-9]+ '.' [0-9]+                   [Ff]
              | [0-9]+ '.'                          [Ff]
              |        '.' [0-9]+                   [Ff]
              | [0-9]+ '.' [0-9]+ [Ee] [+-]? [0-9]+ [Ff]
              | [0-9]+ '.'        [Ee] [+-]? [0-9]+ [Ff]
              |        '.' [0-9]+ [Ee] [+-]? [0-9]+ [Ff]
              | [0-9]+            [Ee] [+-]? [0-9]+ [Ff]
              ;

DoubleConst   : [0-9]+ '.' [0-9]+                   
              | [0-9]+ '.'                          
              |        '.' [0-9]+                   
              | [0-9]+ '.' [0-9]+ [Ee] [+-]? [0-9]+ 
              | [0-9]+ '.'        [Ee] [+-]? [0-9]+ 
              |        '.' [0-9]+ [Ee] [+-]? [0-9]+ 
              | [0-9]+            [Ee] [+-]? [0-9]+ 
              ;

// Lexical Hidden Tokens

LineComment   : '//' ~[\r\n]*   -> channel(HIDDEN)    ;
BlockComment  : '/*' .*? '*/'   -> channel(HIDDEN)    ;
NewLine       :('\r'|'\n'|'\r\n') -> channel(HIDDEN)  ;
WhiteSpace    :('\t'|' ')     -> channel(HIDDEN)      ;

// Lexical Operators

LBra  : '{' ;
LKet  : '}' ;
MBra  : '[' ;
MKet  : ']' ;
SBra  : '(' ;
SKet  : ')' ;
DAnd  : '&&';
DOr   : '||';
Times : '*' ; 
Plus  : '+' ;
Minus : '-' ;
Not   : '!' ;
Divide: '/' ;
Remain: '%' ;
LT    : '<' ;
GT    : '>' ;
LE    : '<=';
GE    : '>=';
EQ    : '==';
NE    : '!=';
Assign: '=' ;
Semi  : ';' ;
Comma : ',' ;

// Parser Expressions
// I don't know why but the first parser rule cannot have its locals
// If I delete "test" then antlr would return 'num_const' ERROR

test  : 'Always' CONTINUE 'Never' BREAK
      ;

num_const 
          locals [
            std::string value, 
            Btype btype,
            std::variant<int, float, double> r_value,
          ]
          : intconst    # num_const_int
          | FloatConst  # num_const_float
          | DoubleConst # num_const_double
          ;

const 
      locals [
        std::string value,
        Btype btype,
        std::variant<int, float, double, bool> r_value,
      ]
      : num_const # const_num_const
      | boolconst # const_bool_const
      ;

signed_num_const  
                  locals [
                    std::string value,
                    Btype btype,
                    bool sign,
                    std::variant<int, float, double> r_value,
                  ]
                  : num_const               # signed_const_const
                  | Plus signed_num_const   # signed_const_plus
                  | Minus signed_num_const  # signed_const_minus
                  ;

signed_const  
              locals [
                std::string value,
                Btype btype,
                std::variant<int, float, double, bool> r_value,
              ]       
              : signed_num_const  # signed_const_num_const
              | boolconst         # signed_const_bool_const
              ;
  
intconst  
          : DecConst    # int_constant
          | HexConst    # hex_constant
          | OctConst    # oct_constant
          ;

boolconst 
          locals [
            std::string value,
            Btype btype,
            bool r_value,
          ]
          : TRUE    # true_constant
          | FALSE   # false_constant
          ;

expr_1    
          locals [
            Btype btype,
            std::vector<size_t> array_size,
            std::string res,
            bool is_const,
            std::variant<int, float, double, bool> value,
            std::string code,
          ]
          : Ident                     # expr_1_ident
          | const                     # expr_1_constant
          | SBra expr_8 SKet          # expr_1_braket
          | Ident (MBra expr_8 MKet)+ # expr_1_array
          | func_call                 # expr_1_func_call
          ;

expr_2
          locals [
            Btype btype,
            std::vector<size_t> array_size,
            std::string res,
            bool is_const,
            std::variant<int, float, double, bool> value,
            std::string code,
          ]
          : expr_1        # expr_2_expr_1
          | Plus expr_2   # expr_2_plus
          | Minus expr_2  # expr_2_minus
          | Not expr_2    # expr_2_not
          ;

expr_3
          locals [
            Btype btype,
            std::vector<size_t> array_size,
            std::string res,
            bool is_const,
            std::variant<int, float, double, bool> value,
            std::string code,
          ]
          : expr_2                # expr_3_expr_2
          | expr_3 Times expr_2   # expr_3_times
          | expr_3 Divide expr_2  # expr_3_divide
          | expr_3 Remain expr_2  # expr_3_remain
          ;

expr_4
          locals [
            Btype btype,
            std::vector<size_t> array_size,
            std::string res,
            bool is_const,
            std::variant<int, float, double, bool> value,
            std::string code,
          ]
          : expr_3              # expr_4_expr_3
          | expr_4 Plus expr_3  # expr_4_plus
          | expr_4 Minus expr_3 # expr_4_minus
          ;

expr_5
          locals [
            Btype btype,
            std::vector<size_t> array_size,
            std::string res,
            bool is_const,
            std::variant<int, float, double, bool> value,
            std::string code,
          ]
          : expr_4            # expr_5_expr_4
          | expr_5 LT expr_4  # expr_5_lt
          | expr_5 GT expr_4  # expr_5_gt
          | expr_5 LE expr_4  # expr_5_le
          | expr_5 GE expr_4  # expr_5_ge
          ;

expr_6
          locals [
            Btype btype,
            std::vector<size_t> array_size,
            std::string res,
            bool is_const,
            std::variant<int, float, double, bool> value,
            std::string code,
          ]
          : expr_5            # expr_6_expr_5
          | expr_6 EQ expr_5  # expr_6_eq
          | expr_6 NE expr_5  # expr_6_ne
          ;

expr_7
          locals [
            Btype btype,
            std::vector<size_t> array_size,
            std::string res,
            bool is_const,
            std::variant<int, float, double, bool> value,
            std::string code,
          ]
          : expr_6              # expr_7_expr_6
          | expr_7 DAnd expr_6  # expr_7_dand
          ;

expr_8
          locals [
            Btype btype,
            std::vector<size_t> array_size,
            std::string res,
            bool is_const,
            std::variant<int, float, double, bool> value,
            std::string code,
          ]
          : expr_7            # expr_8_expr_7
          | expr_8 DOr expr_7 # expr_8_dor
          ;

func_call 
          locals [
            Btype btype,
            std::string res,
            std::string code,
          ]
          : Ident SBra (expr_8 (Comma expr_8)*)? SKet  # func_call_ident
          ;

type
        locals [
          Btype btype,
        ]
        : INT     # type_int
        | BOOL    # type_bool
        | FLOAT   # type_float
        | DOUBLE  # type_double
        | VOID    # type_void
        ;

// Parser Statements

stmt_assign   : l_value Assign expr_8 Semi
              ;

l_value   
          locals [
            Btype btype,
            std::string res,
          ]
          : Ident (MBra expr_8 MKet)*
          ;

stmt_expr : expr_8? Semi
          ;

stmt_block  
            locals [
              Btype need_type,
              bool has_return,
              std::string brk_target,
              std::string ctn_target,
            ]
            : block
            ;

stmt_return 
            locals [
              Btype need_type,
            ]
            : RETURN expr_8? Semi
            ;

stmt_if   
          locals [
            Btype need_type,
            bool has_return,
            std::string brk_target,
            std::string ctn_target,
          ]
          : IF SBra expr_8 SKet stmt (ELSE stmt)? 
          ;

stmt_while   
            locals [
              Btype need_type,
              bool has_return,
              std::string brk_target,
              std::string ctn_target,
            ]
            : WHILE SBra expr_8 SKet stmt
            ;

stmt_break  
            locals [
              std::string brk_target,
            ]
            : BREAK Semi
            ;

stmt_continue 
              locals [
                std::string ctn_target,
              ]
              : CONTINUE Semi
              ;

stmt    
        locals [
          Btype need_type,
          bool has_return,
          std::string brk_target,
          std::string ctn_target,
        ]
        : stmt_assign   # assign_stmt
        | stmt_expr     # expr_stmt
        | stmt_block    # block_stmt
        | stmt_return   # return_stmt
        | stmt_if       # if_stmt
        | stmt_while    # while_stmt
        | stmt_break    # break_stmt
        | stmt_continue # continue_stmt
        ;

decl_var  
          locals [
            bool is_global,
          ]
          : type var_def (Comma var_def)* Semi
          ;

decl_const  
            locals [
              bool is_global,
            ]
            : CONST type const_def (Comma const_def)* Semi
            ;

decl    
        locals [
          bool is_global,
        ]
        : decl_var    # var_decl
        | decl_const  # const_decl
        ;

block_item  
            locals [
              Btype need_type,
              bool has_return,
              std::string brk_target,
              std::string ctn_target,
            ]
            : decl    # block_item_decl
            | stmt    # block_item_stmt
            ;

block   
        locals [
          Btype need_type,
          bool has_return,
          std::string brk_target,
          std::string ctn_target,
        ]
        : LBra (block_item)* LKet
        ;

var_def   
          locals [
			      bool is_global,
            Btype need_type,
          ]
          : Ident (MBra intconst MKet)* (Assign array_signed_const)?
          ;

const_def 
          locals [
			      bool is_global,
            Btype need_type,
          ] 
          : Ident (MBra intconst MKet)*  Assign array_signed_const
          ;

array_signed_const  
                    locals [
                      Btype need_type,
                      std::vector<size_t> array_size,
                      std::string varName,
                      int offset,
                      bool at_top,
					            bool is_global,
                      std::variant<int, float, double, bool> r_value,
                    ]
                    : signed_const                                                # array_signed_const_const
                    | LBra (array_signed_const (Comma array_signed_const)*)? LKet # array_signed_const_array
                    ;

func_def  : type Ident SBra (func_f_param (Comma func_f_param)*)? SKet block
          ;

func_f_param  
              locals [
                FuncParamsType params,
                std::string res,
              ]
              : type Ident (MBra MKet)? (MBra intconst MKet)*
              ;

comp_units  : comp_unit comp_units  # comp_units_more
            | EOF                   # comp_units_empty
            ;

comp_unit : decl      # comp_unit_decl
          | func_def  # comp_unit_func_def
          ;
