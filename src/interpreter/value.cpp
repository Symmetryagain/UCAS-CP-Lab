#include "interpreter.h"

extern Table                       gtable;
extern map<string, vector<string>> func_params;

string get_array_name(string rstr)
{
    cerr << "get_array_name: " << rstr << endl;
    assert(rstr[0] == '%');
    assert(rstr[1] == 'a');
    size_t pos = rstr.find('[');
    assert(pos != string::npos);
    return rstr.substr(0, pos);
}

int get_array_index(Local_State lstate, string rstr)
{
    cerr << "get_array_index: " << rstr << endl;
    assert(rstr[0] == '%');
    assert(rstr[1] == 'a');
    size_t pos1 = rstr.find('[');
    size_t pos2 = rstr.find(']');
    assert(pos1 != string::npos);
    assert(pos2 != string::npos);
    assert(pos1 < pos2);
    string index_str = rstr.substr(pos1 + 1, pos2 - pos1 - 1);
    return get_val_int(lstate, index_str).ival;
}

Value get_val_int(Local_State lstate, string rstr)
{
    cerr << "get_val_int: " << rstr << endl;
    Value val;
    if(rstr[0] == '%')
    {
        if(rstr[1] == 'a')
        {
            assert(rstr[2] == 'i');
            string array_name = get_array_name(rstr);
            int    index      = get_array_index(lstate, rstr);
            if(gtable[array_name].val_type == IT_NONE)
            {
                assert(lstate.ltable[array_name].val_type == IT_ARRAY_INT);
                val = lstate.ltable[array_name].val.aval[index];
            }
            else
            {
                assert(gtable[array_name].val_type == IT_ARRAY_INT);
                val = gtable[array_name].val.aval[index];
            }
        }
        else
        {
            assert(rstr[1] == 'i');
            if(gtable[rstr].val_type == IT_NONE)
            {
                assert(lstate.ltable[rstr].val_type == IT_INT);
                val = lstate.ltable[rstr].val;
            }
            else
            {
                assert(gtable[rstr].val_type == IT_INT);
                val = gtable[rstr].val;
            }
        }
    }
    else
    {
    val.ival = std::stoi(rstr, nullptr, 0);
    }
    cerr << "get_val_int: " << val.ival << endl;
    return val;
}

Value get_val_float(Local_State lstate, string rstr)
{
    cerr << "get_val_float: " << rstr << endl;
    Value val;
    if(rstr[0] == '%')
    {
        if(rstr[1] == 'a')
        {
            assert(rstr[2] == 'f');
            string array_name = get_array_name(rstr);
            int    index      = get_array_index(lstate, rstr);
            if(gtable[array_name].val_type == IT_NONE)
            {
                assert(lstate.ltable[array_name].val_type == IT_ARRAY_FLOAT);
                val = lstate.ltable[array_name].val.aval[index];
            }
            else
            {
                assert(gtable[array_name].val_type == IT_ARRAY_FLOAT);
                val = gtable[array_name].val.aval[index];
            }
        }
        else
        {
            assert(rstr[1] == 'f');
            if(gtable[rstr].val_type == IT_NONE)
            {
                assert(lstate.ltable[rstr].val_type == IT_FLOAT);
                val = lstate.ltable[rstr].val;
            }
            else
            {
                assert(gtable[rstr].val_type == IT_FLOAT);
                val = gtable[rstr].val;
            }
        }
    }
    else
    {
        val.fval = atof(rstr.c_str());
    }
    cerr << "get_val_float: " << val.fval << endl;
    return val;
}

Value get_val_double(Local_State lstate, string rstr)
{
    cerr << "get_val_double: " << rstr << endl;
    Value val;
    if(rstr[0] == '%')
    {
        if(rstr[1] == 'a')
        {
            assert(rstr[2] == 'd');
            string array_name = get_array_name(rstr);
            int    index      = get_array_index(lstate, rstr);
            if(gtable[array_name].val_type == IT_NONE)
            {
                assert(lstate.ltable[array_name].val_type == IT_ARRAY_DOUBLE);
                val = lstate.ltable[array_name].val.aval[index];
            }
            else
            {
                assert(gtable[array_name].val_type == IT_ARRAY_DOUBLE);
                val = gtable[array_name].val.aval[index];
            }
        }
        else
        {
            assert(rstr[1] == 'd');
            if(gtable[rstr].val_type == IT_NONE)
            {
                assert(lstate.ltable[rstr].val_type == IT_DOUBLE);
                val = lstate.ltable[rstr].val;
            }
            else
            {
                assert(gtable[rstr].val_type == IT_DOUBLE);
                val = gtable[rstr].val;
            }
        }
    }
    else
    {
        val.dval = atof(rstr.c_str());
    }
    cerr << "get_val_double: " << val.dval << endl;
    return val;
}

Value get_val_bool(Local_State lstate, string rstr)
{
    cerr << "get_val_bool: " << rstr << endl;
    Value val;
    if(rstr[0] == '%')
    {
        if(rstr[1] == 'a')
        {
            assert(rstr[2] == 'b');
            string array_name = get_array_name(rstr);
            int    index      = get_array_index(lstate, rstr);
            if(gtable[array_name].val_type == IT_NONE)
            {
                assert(lstate.ltable[array_name].val_type == IT_ARRAY_BOOL);
                val = lstate.ltable[array_name].val.aval[index];
            }
            else
            {
                assert(gtable[array_name].val_type == IT_ARRAY_BOOL);
                val = gtable[array_name].val.aval[index];
            }
        }
        else
        {
            assert(rstr[1] == 'b');
            if(gtable[rstr].val_type == IT_NONE)
            {
                assert(lstate.ltable[rstr].val_type == IT_BOOL);
                val = lstate.ltable[rstr].val;
            }
            else
            {
                assert(gtable[rstr].val_type == IT_BOOL);
                val = gtable[rstr].val;
            }
        }
    }
    else
    {
        val.bval = (rstr == "true");
    }
    cerr << "get_val_bool: " << val.bval << endl;
    return val;
}

Value get_val_array(Local_State lstate, string rstr)
{
    assert(rstr[0] == '%');
    assert(rstr[1] == 'a');
    cerr << "get_val_array: " << rstr << endl;
    if(gtable[rstr].val_type == IT_NONE)
    {
        assert(lstate.ltable[rstr].val_type == IT_ARRAY_INT ||
               lstate.ltable[rstr].val_type == IT_ARRAY_FLOAT ||
               lstate.ltable[rstr].val_type == IT_ARRAY_DOUBLE ||
               lstate.ltable[rstr].val_type == IT_ARRAY_BOOL);
        cerr << "get_val_array: " << lstate.ltable[rstr].val.aval << endl;
        return lstate.ltable[rstr].val;
    }
    else
    {
        assert(gtable[rstr].val_type == IT_ARRAY_INT ||
               gtable[rstr].val_type == IT_ARRAY_FLOAT ||
               gtable[rstr].val_type == IT_ARRAY_DOUBLE ||
               gtable[rstr].val_type == IT_ARRAY_BOOL);
        cerr << "get_val_array: " << gtable[rstr].val.aval << endl;
        return gtable[rstr].val;
    }
}

Value get_val(IdentType ident_type, Local_State lstate, string rstr)
{
    switch(ident_type)
    {
    case IT_INT:
        return get_val_int(lstate, rstr);
    case IT_FLOAT:
        return get_val_float(lstate, rstr);
    case IT_DOUBLE:
        return get_val_double(lstate, rstr);
    case IT_BOOL:
        return get_val_bool(lstate, rstr);
    case IT_ARRAY_INT:
    case IT_ARRAY_FLOAT:
    case IT_ARRAY_DOUBLE:
    case IT_ARRAY_BOOL:
        return get_val_array(lstate, rstr);
    default:
        assert(false);
    }
}

void print_val(Value val, IdentType vtype)
{
    switch(vtype)
    {
    case IT_INT:
        cerr << val.ival;
        break;
    case IT_FLOAT:
        cerr << val.fval;
        break;
    case IT_DOUBLE:
        cerr << val.dval;
        break;
    case IT_BOOL:
        cerr << (val.bval ? "true" : "false");
        break;
    case IT_ARRAY_INT:
    case IT_ARRAY_FLOAT:
    case IT_ARRAY_DOUBLE:
    case IT_ARRAY_BOOL:
        cerr << val.aval;
        break;
    default:
        assert(false);
    }
}
