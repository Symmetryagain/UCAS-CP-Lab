grammar Hello;

@header {
    #include <vector>
}

/****** lexer  ******/

TRUE : 'true' ;
FALSE : 'false' ;
CONST : 'const' ;
INT : 'int' ;
BOOL : 'bool' ;
FLOAT : 'float' ;
DOUBLE : 'double' ;
VOID : 'void' ;
IF : 'if' ;
ELSE : 'else' ;
WHILE : 'while' ;
BREAK : 'break' ;
CONTINUE : 'continue' ;
RETURN : 'return' ;

DecimalConst : 0 | ['-'](1-9){(0-9)}
OctalConst : 0(0-9){(0-9)}
HexadecConst : 0(x|X)(0-9a-fA-F){(0-9a-fA-F)}
IntConst : DecimalConst | OctalConst | HexadecConst

FloatConstNormal : ['-'] ( (0-9){(0-9)} '.' | '.' (0-9) ) {0-9} [(fF)]
FloatConstExp : ['-'] (0-9){(0-9)} ['.' (0-9){(0-9)}] (eE) ['+' | '-'] (0-9){(0-9)} [(fF)]
FloatConst : FloatConstNormal | FloatConstExp

DoubleConstNormal : [-] ( (0-9){0-9} '.' | '.' (0-9) ) {0-9}
DoubleConstExp : ['-'] (0-9){(0-9)} ['.' (0-9){(0-9)}] (eE) ['+' | '-'] (0-9){(0-9)}
DoubleConst : DoubleConstNormal | DoubleConstExp

BoolConst : TRUE | FALSE ;
Ident : (a-zA-Z_){(a-zA-Z0-9_)} ;

LineComment : '//'{~('\r' '\n')}
BlockComment : '/*'{.}'*/'
NewLine : '\r'['\n'] | '\n'
WhiteSpace : {' ' | '\t'}

/****** parser ******/

Block : '{' { BlockItem } '}'
BlockItem : Decl | Stmt
Stmt : LVal '=' Exp ';'
     | [Exp] ';'
     | Block 
     | RETURN [Exp] ';'
     | IF '(' Cond ')' Stmt [ ELSE Stmt ]
     | WHILE '(' Cond ')' Stmt
     | BREAK ';'
     | CONTINUE ';'
Exp : AddExp | BoolConst
ConstExp : Number | BoolConst
Cond : LOrExp
LVal : Ident { '[' Exp ']' }
PrimaryExp : '(' Exp ')' | LVal | Number
Number : IntConst | DoubleConst | FloatConst
UnaryExp : PrimaryExp | ('+' | '-' | '!') UnaryExp | Ident '(' FuncArgs ')'
FuncArgs : Exp { ',' Exp }
MulExp : UnaryExp | MulExp ('*' | '/' | '%') UnaryExp
AddExp : MulExp | AddExp ('+' | '-') MulExp
RelExp : AddExp [ ('<=' | '>=' | '<' | '>') AddExp ]
EqExp : RelExp [ ('==' | '!=') RelExp ]
LAndExp : EqExp [ ('&&') EqExp ]
LOrExp : LAndExp [ ('||') LAndExp ]

CompUnit : [CompUnit] (Decl | FuncDef)
Decl : ConstDecl | VarDecl
ConstDecl : CONST BType ConstDef { ',' ConstDef } ';'
BType : INT | BOOL | FLOAT | DOUBLE
ConstDef : Ident { '[' IntConst ']' } '=' ConstInitVal
ConstInitVal : ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
VarDecl : BType VarDef { ',' VarDef } ';'
VarDef : Ident { '[' IntConst ']' } [ '=' ConstInitVal ]
FuncDef : FuncType Ident '(' [FuncParams] ')' Block
FuncType : VOID | INT | FLOAT | DOUBLE | BOOL
FuncParams : FuncParam { ',' FuncParam }
FuncParam : BType Ident [ '[' [IntConst] ']' { '[' IntConst ']' } ]

/****** skips  ******/
WS : [NewLine | WhiteSpace | LineComment | BlockComment]+ -> skip ; 