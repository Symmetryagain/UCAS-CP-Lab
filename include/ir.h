#ifndef CACT_IR_H
#define CACT_IR_H

// IR opcodes shared between compiler (Analyzer) and asmgen
#define NPRE    "%"
#define GLOBAL  "!global"
#define VAR     "@var"
#define ARRAY   "@array"
#define FUNC    "@func"
#define ENDFUNC "@endfunc"
#define CALL    "call"
#define VOID    "void"
#define LABEL   "label"
#define RETURN  "return"
#define ASSIGN  "assign"
#define NEG     "Neg"
#define NOT     "Not"
#define BRANCH  "branch"
#define ADD     "Add"
#define SUB     "Sub"
#define MUL     "Mul"
#define DIV     "Div"
#define REM     "Rem"
#define GE      "GE"
#define GT      "GT"
#define LE      "LE"
#define LT      "LT"
#define EQ      "EQ"
#define NE      "NE"
#define DAND    "DAnd"
#define DOR     "DOr"
#define ADDR    "Addr"

#endif // CACT_IR_H
