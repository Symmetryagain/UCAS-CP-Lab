#include "interpreter.h"

IdentType vtype_var_to_array(IdentType vtype)
{
    switch(vtype)
    {
    case IT_INT:
        return IT_ARRAY_INT;
    case IT_FLOAT:
        return IT_ARRAY_FLOAT;
    case IT_DOUBLE:
        return IT_ARRAY_DOUBLE;
    case IT_BOOL:
        return IT_ARRAY_BOOL;
    default:
        assert(false);
    }
}

IdentType vtype_array_to_var(IdentType vtype)
{
    switch(vtype)
    {
    case IT_ARRAY_INT:
        return IT_INT;
    case IT_ARRAY_FLOAT:
        return IT_FLOAT;
    case IT_ARRAY_DOUBLE:
        return IT_DOUBLE;
    case IT_ARRAY_BOOL:
        return IT_BOOL;
    default:
        assert(false);
    }
}

IdentType vtype_var_to_func(IdentType vtype)
{
    switch(vtype)
    {
    case IT_INT:
        return IT_FUNC_INT;
    case IT_FLOAT:
        return IT_FUNC_FLOAT;
    case IT_DOUBLE:
        return IT_FUNC_DOUBLE;
    case IT_BOOL:
        return IT_FUNC_BOOL;
    default:
        assert(false);
    }
}

IdentType vtype_func_to_var(IdentType vtype)
{
    switch(vtype)
    {
    case IT_FUNC_INT:
        return IT_INT;
    case IT_FUNC_FLOAT:
        return IT_FLOAT;
    case IT_FUNC_DOUBLE:
        return IT_DOUBLE;
    case IT_FUNC_BOOL:
        return IT_BOOL;
    case IT_FUNC_VOID:
        return IT_INT; // void function returns 0
    default:
        assert(false);
    }
}

IdentType get_func_type_by_prefix(string func_name)
{
    if(func_name == "%main")
    {
        return IT_FUNC_INT;
    }
    else if(func_name == "%print_int")
    {
        return IT_FUNC_VOID;
    }
    else if(func_name == "%print_float")
    {
        return IT_FUNC_VOID;
    }
    else if(func_name == "%print_double")
    {
        return IT_FUNC_VOID;
    }
    else if(func_name == "%print_bool")
    {
        return IT_FUNC_VOID;
    }
    else if(func_name == "%get_int")
    {
        return IT_FUNC_INT;
    }
    else if(func_name == "%get_float")
    {
        return IT_FUNC_FLOAT;
    }
    else if(func_name == "%get_double")
    {
        return IT_FUNC_DOUBLE;
    }
    assert(func_name[0] == '%');
    switch(func_name[1])
    {
    case 'i':
        return IT_FUNC_INT;
    case 'f':
        return IT_FUNC_FLOAT;
    case 'd':
        return IT_FUNC_DOUBLE;
    case 'b':
        return IT_FUNC_BOOL;
    case 'v':
        return IT_FUNC_VOID;
    default:
        assert(false);
    }
}

IdentType get_var_type_by_prefix(string var_name)
{
    assert(var_name[0] == '%');
    switch(var_name[1])
    {
    case 'i':
        return IT_INT;
    case 'f':
        return IT_FLOAT;
    case 'd':
        return IT_DOUBLE;
    case 'b':
        return IT_BOOL;
    case 'a':
        switch(var_name[2])
        {
        case 'i':
            return IT_ARRAY_INT;
        case 'f':
            return IT_ARRAY_FLOAT;
        case 'd':
            return IT_ARRAY_DOUBLE;
        case 'b':
            return IT_ARRAY_BOOL;
        default:
            assert(false);
        }
    default:
        assert(false);
    }
}

IdentType get_final_var_type_by_prefix(string var_name)
{
    switch(var_name[1])
    {
    case 'i':
        return IT_INT;
    case 'f':
        return IT_FLOAT;
    case 'd':
        return IT_DOUBLE;
    case 'b':
        return IT_BOOL;
    case 'a':
        switch(var_name[2])
        {
        case 'i':
            return IT_INT;
        case 'f':
            return IT_FLOAT;
        case 'd':
            return IT_DOUBLE;
        case 'b':
            return IT_BOOL;
        default:
            assert(false);
        }
    default:
        assert(false);
    }
}
