#include "interpreter.h"

#include <vector>

vector<string> splitString(string const &str)
{
    istringstream  iss(str);
    vector<string> tokens;
    string         token;

    while(iss >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}

OpType get_op_type(string lstr)
{
    vector<string> tokens = splitString(lstr);
    if(tokens.size() == 0)
    {
        return OP_EMPTY;
    }
    else if(tokens[0] == "@func")
    {
        return OP_FUNC_DEF;
    }
    else if(tokens[0] == "@endfunc")
    {
        return OP_FUNC_END;
    }
    else if(tokens[0] == "call")
    {
        return OP_FUNC_CALL;
    }
    else if(tokens[0] == "!global")
    {
        assert(tokens.size() > 1);
        int idx = 1;
        if(tokens[idx] == "const") idx = 2;
        if(tokens[idx] == "assign")
        {
            return OP_G_ASSIGN;
        }
        else if(tokens[idx] == "@array")
        {
            return OP_G_ARRAY;
        }
        else if(tokens[idx] == "@var")
        {
            return OP_G_VAR;
        }
        else
        {
            assert(false);
        }
    }
    else if(tokens[0] == "assign")
    {
        return OP_ASSIGN;
    }
    else if(tokens[0] == "@array")
    {
        return OP_ARRAY;
    }
    else if(tokens[0] == "@var")
    {
        return OP_VAR;
    }
    else if(tokens[0] == "DAnd")
    {
        return OP_AND;
    }
    else if(tokens[0] == "DOr")
    {
        return OP_OR;
    }
    else if(tokens[0] == "Add")
    {
        return OP_ADD;
    }
    else if(tokens[0] == "Addr")
    {
        return OP_ADDR;
    }
    else if(tokens[0] == "Sub")
    {
        return OP_SUB;
    }
    else if(tokens[0] == "Mul")
    {
        return OP_MUL;
    }
    else if(tokens[0] == "Div")
    {
        return OP_DIV;
    }
    else if(tokens[0] == "Rem")
    {
        return OP_REM;
    }
    else if(tokens[0] == "LT")
    {
        return OP_LT;
    }
    else if(tokens[0] == "LE")
    {
        return OP_LE;
    }
    else if(tokens[0] == "GT")
    {
        return OP_GT;
    }
    else if(tokens[0] == "GE")
    {
        return OP_GE;
    }
    else if(tokens[0] == "EQ")
    {
        return OP_EQ;
    }
    else if(tokens[0] == "NE")
    {
        return OP_NE;
    }
    else if(tokens[0] == "Neg")
    {
        return OP_NEG;
    }
    else if(tokens[0] == "Not")
    {
        return OP_NOT;
    }
    else if(tokens[0] == "Pos")
    {
        return OP_POS;
    }
    else if(tokens[0] == "branch")
    {
        return OP_BRANCH;
    }
    else if(tokens[0] == "label")
    {
        return OP_LABEL;
    }
    else if(tokens[0] == "return")
    {
        return OP_RETURN;
    }
    else if(tokens[0] == "retire")
    {
        return OP_RETIRE;
    }
    assert(false);
}
