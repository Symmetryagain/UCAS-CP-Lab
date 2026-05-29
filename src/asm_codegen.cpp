#include "asm_gen.h"
#include "ir.h"
#include "context.h"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

using namespace std;

// Data structures populated by asm_parse.cpp
extern vector<GlobalVar>            gv_list;
extern map<string, size_t>          gv_map;
extern vector<GlobalArr>            ga_list;
extern map<string, size_t>          ga_map;
extern vector<Funct>                f_list;
extern map<string, size_t>          f_map;

// Shared helpers from asm_parse.cpp
extern string normalize_name(string prev_name);
extern vector<string> split_string(string const & str);
extern PType get_ptype_from_string(string str);
extern GType get_gtype_from_char(char ch);
extern uint64_t str_to_bin(GType gtype, string str);

extern void init_extern_func();
extern void parse_ir();
extern void show_ir();

// Instruction mnemonics by size
static string inst_load_i(size_t size)
{
    switch(size)
    {
    case 1: return "lb";
    case 2: return "lh";
    case 4: return "lw";
    case 8: return "ld";
    default: assert(false);
    }
}

static string inst_store_i(size_t size)
{
    switch(size)
    {
    case 1: return "sb";
    case 2: return "sh";
    case 4: return "sw";
    case 8: return "sd";
    default: assert(false);
    }
}

static string inst_load_f(size_t size)
{
    switch(size)
    {
    case 4: return "flw";
    case 8: return "fld";
    default: assert(false);
    }
}

static string inst_store_f(size_t size)
{
    switch(size)
    {
    case 4: return "fsw";
    case 8: return "fsd";
    default: assert(false);
    }
}

// RISC-V instruction helpers
static void gen0r(Funct& f, string op)
{
    f.asm_lines.push_back(op);
}

static void gen1r(Funct& f, string op, string rd)
{
    f.asm_lines.push_back(op + " " + rd);
}

static void gen2r(Funct& f, string op, string rd, string rs)
{
    f.asm_lines.push_back(op + " " + rd + ", " + rs);
}

static void gen3r(Funct& f, string op, string rd, string rs, string rt)
{
    f.asm_lines.push_back(op + " " + rd + ", " + rs + ", " + rt);
}

static void gen_sp_expand(Funct& f)
{
    gen3r(f, "addi", "sp", "sp", "-" + to_string(f.sp_size));
}

static void gen_sp_shrink(Funct& f)
{
    gen3r(f, "addi", "sp", "sp", to_string(f.sp_size));
}

static void func_call(Funct& f, string func)
{
    gen1r(f, "call", normalize_name(func));
}

// Memory ↔ register transfer helpers
static void ir2m(Funct& f, string ireg, string memvar)
{
    assert(ireg == "ra" || ireg == "sp" || ireg[0] == 's' || ireg[0] == 'a' || ireg == "zero");
    PType  ptype = get_ptype_from_string(memvar);
    size_t size  = TypeSize[ptype];
    if(f.lv_map.count(memvar) != 0)
    {
        size_t sp_pos = f.lv_list[f.lv_map[memvar]].sp_pos;
        gen2r(f, inst_store_i(size), ireg, to_string(sp_pos) + "(sp)");
    }
    else
    {
        assert(gv_map.count(memvar) != 0);
        gen2r(f, "la", "t0", normalize_name(memvar));
        gen2r(f, inst_store_i(size), ireg, "0(t0)");
    }
}

static void fr2m(Funct& f, string freg, string memvar)
{
    assert(freg[0] == 'f' && (freg[1] == 's' || freg[1] == 'a'));
    PType  ptype = get_ptype_from_string(memvar);
    size_t size  = TypeSize[ptype];
    assert(ptype == P_FLOAT || ptype == P_DOUBLE);
    if(f.lv_map.count(memvar) != 0)
    {
        size_t sp_pos = f.lv_list[f.lv_map[memvar]].sp_pos;
        gen2r(f, inst_store_f(size), freg, to_string(sp_pos) + "(sp)");
    }
    else
    {
        assert(gv_map.count(memvar) != 0);
        gen2r(f, "la", "t0", normalize_name(memvar));
        gen2r(f, inst_store_f(size), freg, "0(t0)");
    }
}

static void m2ir(Funct& f, PType ptype, string memvar, string ireg)
{
    assert(ireg == "ra" || ireg == "sp" || ireg[0] == 's' || ireg[0] == 'a');
    if(memvar[0] == '%' || memvar[0] == '$')
    {
        PType  ptype_mem = get_ptype_from_string(memvar);
        size_t size      = TypeSize[ptype];
        assert(ptype_mem == ptype);
        if(f.lv_map.count(memvar) != 0)
        {
            size_t sp_pos = f.lv_list[f.lv_map[memvar]].sp_pos;
            gen2r(f, inst_load_i(size), ireg, to_string(sp_pos) + "(sp)");
        }
        else if(f.la_map.count(memvar) != 0)
        {
            size_t sp_pos = f.la_list[f.la_map[memvar]].sp_pos;
            gen3r(f, "addi", ireg, "sp", to_string(sp_pos));
        }
        else if(gv_map.count(memvar) != 0)
        {
            gen2r(f, "la", "t0", normalize_name(memvar));
            gen2r(f, inst_load_i(size), ireg, "0(t0)");
        }
        else
        {
            assert(ga_map.count(memvar) != 0);
            gen2r(f, "la", ireg, normalize_name(memvar));
        }
    }
    else
    {
        switch(ptype)
        {
        case P_BOOL:
            assert((memvar == "true") || (memvar == "false"));
            if(memvar == "true") gen2r(f, "li", ireg, "1");
            else                  gen2r(f, "li", ireg, "0");
            break;
        case P_INT:
            {
                int ival = stoi(memvar, 0, 0);
                gen2r(f, "li", ireg, to_string(ival));
            }
            break;
        default:
            assert(false);
        }
    }
}

static void m2fr(Funct& f, PType ptype, string memvar, string freg)
{
    assert(freg[0] == 'f' && (freg[1] == 's' || freg[1] == 'a'));
    assert(ptype == P_FLOAT || ptype == P_DOUBLE);
    if(memvar[0] == '%')
    {
        PType  ptype_mem = get_ptype_from_string(memvar);
        size_t size      = TypeSize[ptype];
        assert(ptype_mem == ptype);
        if(f.lv_map.count(memvar) != 0)
        {
            size_t sp_pos = f.lv_list[f.lv_map[memvar]].sp_pos;
            gen2r(f, inst_load_f(size), freg, to_string(sp_pos) + "(sp)");
        }
        else
        {
            assert(gv_map.count(memvar) != 0);
            gen2r(f, "la", "t0", normalize_name(memvar));
            gen2r(f, inst_load_f(size), freg, "0(t0)");
        }
    }
    else
    {
        uint64_t val = 0;
        switch(ptype)
        {
        case P_FLOAT:
            val = str_to_bin(G_FLOAT, memvar);
            gen2r(f, "li", "t0", to_string(val));
            gen2r(f, "fmv.w.x", freg, "t0");
            break;
        case P_DOUBLE:
            val = str_to_bin(G_DOUBLE, memvar);
            gen2r(f, "li", "t0", to_string(val));
            gen2r(f, "fmv.d.x", freg, "t0");
            break;
        default:
            assert(false);
        }
    }
}

static void gen_return(Funct& f, string memvar)
{
    switch(f.ftype)
    {
    case F_VOID:
        assert(memvar == "");
        break;
    case F_INT:
        m2ir(f, P_INT, memvar, "a0");
        break;
    case F_BOOL:
        m2ir(f, P_BOOL, memvar, "a0");
        break;
    case F_FLOAT:
        m2fr(f, P_FLOAT, memvar, "fa0");
        break;
    case F_DOUBLE:
        m2fr(f, P_DOUBLE, memvar, "fa0");
        break;
    default:
        assert(0);
    }
    m2ir(f, P_REG, "$ra", "ra");
    gen_sp_shrink(f);
    gen0r(f, "ret");
}

static void gen_branch(Funct& f, string label, string memvar)
{
    if(memvar[0] == '%')
    {
        assert(get_ptype_from_string(memvar) == P_BOOL);
        if(f.lv_map.count(memvar) != 0)
        {
            size_t sp_pos = f.lv_list[f.lv_map[memvar]].sp_pos;
            gen2r(f, "lb", "t1", to_string(sp_pos) + "(sp)");
        }
        else
        {
            assert(gv_map.count(memvar) != 0);
            gen2r(f, "la", "t0", normalize_name(memvar));
            gen2r(f, "lb", "t1", "0(t0)");
        }
        gen2r(f, "bnez", "t1", normalize_name(label));
    }
    else
    {
        assert((memvar == "true") || (memvar == "false"));
        if(memvar == "true")
        {
            gen1r(f, "j", normalize_name(label));
        }
    }
}

static void gen_label(Funct& f, string label)
{
    gen0r(f, normalize_name(label) + ":");
}

static void gen_assign(Funct& f, string dst, string src)
{
    assert(dst[0] == '%' || dst[0] == '$');
    PType dsttype = get_ptype_from_string(dst);
    GType dstvtype;
    switch(dsttype)
    {
    case P_INT:     case P_PTR_INT:    dstvtype = G_INT;    break;
    case P_BOOL:    case P_PTR_BOOL:   dstvtype = G_BOOL;   break;
    case P_FLOAT:   case P_PTR_FLOAT:  dstvtype = G_FLOAT;  break;
    case P_DOUBLE:  case P_PTR_DOUBLE: dstvtype = G_DOUBLE; break;
    default:        assert(false);
    }

    GType srcvtype;
    if(src[0] == '%' || src[0] == '$')
    {
        PType srctype;
        srctype = get_ptype_from_string(src);
        switch(srctype)
        {
        case P_INT:    case P_PTR_INT:    srcvtype = G_INT;    break;
        case P_BOOL:   case P_PTR_BOOL:   srcvtype = G_BOOL;   break;
        case P_FLOAT:  case P_PTR_FLOAT:  srcvtype = G_FLOAT;  break;
        case P_DOUBLE: case P_PTR_DOUBLE: srcvtype = G_DOUBLE; break;
        default:       assert(false);
        }

        switch(srctype)
        {
        case P_INT: case P_BOOL: case P_FLOAT: case P_DOUBLE:
            m2ir(f, srctype, src, "s5");
            break;
        case P_PTR_INT: case P_PTR_BOOL: case P_PTR_FLOAT: case P_PTR_DOUBLE:
            {
                assert(src.back() == ']');
                size_t name_idx;
                for(name_idx = 0; name_idx < src.size() && src[name_idx] != '[';
                    name_idx++);
                assert(name_idx != src.size());
                string src_arr = src.substr(0, name_idx);
                string src_idx = src.substr(name_idx + 1, src.size() - name_idx - 2);
                size_t size = TypeSize[srcvtype];
                if(optimize_level > 0)
                {
                    switch(size)
                    {
                    case 1: m2ir(f, P_INT, src_idx, "s2"); break;
                    case 2: m2ir(f, P_INT, src_idx, "s0"); gen3r(f, "slliw", "s2", "s0", "1"); break;
                    case 4: m2ir(f, P_INT, src_idx, "s0"); gen3r(f, "slliw", "s2", "s0", "2"); break;
                    case 8: m2ir(f, P_INT, src_idx, "s0"); gen3r(f, "slliw", "s2", "s0", "3"); break;
                    default: assert(false);
                    }
                }
                else
                {
                    m2ir(f, P_INT, src_idx, "s0");
                    m2ir(f, P_INT, to_string(size), "s1");
                    gen3r(f, "mulw", "s2", "s0", "s1");
                }
                m2ir(f, srctype, src_arr, "s3");
                gen3r(f, "add", "s4", "s3", "s2");
                gen2r(f, inst_load_i(size), "s5", "0(s4)");
            }
            break;
        default:
            assert(false);
        }
    }
    else
    {
        srcvtype = dstvtype;
        uint64_t val;
        val = str_to_bin(dstvtype, src);
        gen2r(f, "li", "s5", to_string(val));
    }
    assert(srcvtype == dstvtype);

    switch(dsttype)
    {
    case P_INT: case P_BOOL: case P_FLOAT: case P_DOUBLE:
        ir2m(f, "s5", dst);
        break;
    case P_PTR_INT: case P_PTR_BOOL: case P_PTR_FLOAT: case P_PTR_DOUBLE:
        {
            assert(dst.back() == ']');
            size_t name_idx;
            for(name_idx = 0; name_idx < dst.size() && dst[name_idx] != '[';
                name_idx++);
            assert(name_idx != dst.size());
            string dst_arr = dst.substr(0, name_idx);
            string dst_idx = dst.substr(name_idx + 1, dst.size() - name_idx - 2);
            size_t size = TypeSize[dstvtype];
            if(optimize_level > 0)
            {
                switch(size)
                {
                case 1: m2ir(f, P_INT, dst_idx, "s8"); break;
                case 2: m2ir(f, P_INT, dst_idx, "s6"); gen3r(f, "slliw", "s8", "s6", "1"); break;
                case 4: m2ir(f, P_INT, dst_idx, "s6"); gen3r(f, "slliw", "s8", "s6", "2"); break;
                case 8: m2ir(f, P_INT, dst_idx, "s6"); gen3r(f, "slliw", "s8", "s6", "3"); break;
                }
            }
            else
            {
                m2ir(f, P_INT, dst_idx, "s6");
                m2ir(f, P_INT, to_string(size), "s7");
                gen3r(f, "mulw", "s8", "s6", "s7");
            }
            m2ir(f, dsttype, dst_arr, "s9");
            gen3r(f, "add", "s10", "s9", "s8");
            gen2r(f, inst_store_i(size), "s5", "0(s10)");
        }
        break;
    default:
        assert(false);
    }
}

static void gen_addr(Funct& f, string dst_p, string src_p, string offset)
{
    PType ptrtype = get_ptype_from_string(src_p);
    assert(get_ptype_from_string(dst_p) == ptrtype);
    PType ptype;
    switch(ptrtype)
    {
    case P_PTR_BOOL:   ptype = P_BOOL;   break;
    case P_PTR_INT:    ptype = P_INT;    break;
    case P_PTR_FLOAT:  ptype = P_FLOAT;  break;
    case P_PTR_DOUBLE: ptype = P_DOUBLE; break;
    default:           assert(0);
    }
    size_t size = TypeSize[ptype];
    if(optimize_level > 0)
    {
        switch(size)
        {
        case 1: m2ir(f, P_INT, offset, "s2"); break;
        case 2: m2ir(f, P_INT, offset, "s0"); gen3r(f, "slliw", "s2", "s0", "1"); break;
        case 4: m2ir(f, P_INT, offset, "s0"); gen3r(f, "slliw", "s2", "s0", "2"); break;
        case 8: m2ir(f, P_INT, offset, "s0"); gen3r(f, "slliw", "s2", "s0", "3"); break;
        default: assert(false);
        }
    }
    else
    {
        m2ir(f, P_INT, offset, "s0");
        m2ir(f, P_INT, to_string(size), "s1");
        gen3r(f, "mulw", "s2", "s0", "s1");
    }
    m2ir(f, ptrtype, src_p, "s3");
    gen3r(f, "add", "s4", "s3", "s2");
    ir2m(f, "s4", dst_p);
}

// IR → assembly translation
static void o0_gen_asm_func(Funct& f)
{
    for(string ir_line : f.ir_lines)
    {
        vector<string> tokens = split_string(ir_line);

        if(tokens.size() == 0)
        {
            continue;
        }
        else if(tokens[0] == "@func")
        {
            gen_sp_expand(f);
            ir2m(f, "ra", "$ra");
            for(Param p : f.p_list)
            {
                if(p.reg_pos < 8)
                    ir2m(f, "a" + to_string(p.reg_pos), p.name);
                else
                    fr2m(f, "fa" + to_string(p.reg_pos - 8), p.name);
            }
        }
        else if(tokens[0] == "@endfunc")
        {
            if(f.ftype == F_VOID) gen_return(f, "");
            return;
        }
        else if(tokens[0] == "call")
        {
            string res_var   = tokens[1];
            string func_name = tokens[2];
            Funct   g         = f_list[f_map[func_name]];
            size_t pas       = 0;
            for(size_t pos = 4; pos < tokens.size() - 1; pos++)
            {
                if(tokens[pos] == ",") continue;
                if(g.p_list[pas].reg_pos < 8)
                    m2ir(f, g.p_list[pas].ptype, tokens[pos],
                         "a" + to_string(g.p_list[pas].reg_pos));
                else
                    m2fr(f, g.p_list[pas].ptype, tokens[pos],
                         "fa" + to_string(g.p_list[pas].reg_pos - 8));
                pas++;
            }
            func_call(f, func_name);
            switch(g.ftype)
            {
            case F_INT:   case F_BOOL:   ir2m(f, "a0", res_var);  break;
            case F_FLOAT: case F_DOUBLE: fr2m(f, "fa0", res_var); break;
            case F_VOID:  assert(res_var == "void");               break;
            default:      assert(false);
            }
        }
        else if(tokens[0] == "!global")
        {
            cerr << "global in function: " << f.name << ": " << ir_line << endl;
            assert(false);
        }
        else if(tokens[0] == "assign") { gen_assign(f, tokens[1], tokens[2]); }
        else if(tokens[0] == "@array") { continue; }
        else if(tokens[0] == "@var")   { continue; }
        else if(tokens[0] == "DAnd")
        {
            m2ir(f, P_BOOL, tokens[2], "s0");
            m2ir(f, P_BOOL, tokens[3], "s1");
            gen3r(f, "and", "s2", "s0", "s1");
            ir2m(f, "s2", tokens[1]);
        }
        else if(tokens[0] == "DOr")
        {
            m2ir(f, P_BOOL, tokens[2], "s0");
            m2ir(f, P_BOOL, tokens[3], "s1");
            gen3r(f, "or", "s2", "s0", "s1");
            ir2m(f, "s2", tokens[1]);
        }
        else if(tokens[0] == "Add")
        {
            switch(tokens[1][0])
            {
            case 'i':
                if(optimize_level > 0 && tokens[4][0] != '%')
                {
                    int imm = stoi(tokens[4], 0, 0);
                    if(imm < 2048 && imm >= -2048)
                    {
                        m2ir(f, P_INT, tokens[3], "s0");
                        gen3r(f, "addi", "s1", "s0", to_string(imm));
                        ir2m(f, "s1", tokens[2]);
                        break;
                    }
                }
                m2ir(f, P_INT, tokens[3], "s0");
                m2ir(f, P_INT, tokens[4], "s1");
                gen3r(f, "addw", "s2", "s0", "s1");
                ir2m(f, "s2", tokens[2]);
                break;
            case 'f':
                m2fr(f, P_FLOAT, tokens[3], "fs0");
                m2fr(f, P_FLOAT, tokens[4], "fs1");
                gen3r(f, "fadd.s", "fs2", "fs0", "fs1");
                fr2m(f, "fs2", tokens[2]);
                break;
            case 'd':
                m2fr(f, P_DOUBLE, tokens[3], "fs0");
                m2fr(f, P_DOUBLE, tokens[4], "fs1");
                gen3r(f, "fadd.d", "fs2", "fs0", "fs1");
                fr2m(f, "fs2", tokens[2]);
                break;
            default: assert(false);
            }
        }
        else if(tokens[0] == "Addr")   { gen_addr(f, tokens[1], tokens[2], tokens[3]); }
        else if(tokens[0] == "Sub")
        {
            switch(tokens[1][0])
            {
            case 'i':
                if(optimize_level > 0 && tokens[4][0] != '%')
                {
                    int imm = -stoi(tokens[4], 0, 0);
                    if(imm < 2048 && imm >= -2048)
                    {
                        m2ir(f, P_INT, tokens[3], "s0");
                        gen3r(f, "addi", "s1", "s0", to_string(imm));
                        ir2m(f, "s1", tokens[2]);
                        break;
                    }
                }
                m2ir(f, P_INT, tokens[3], "s0");
                m2ir(f, P_INT, tokens[4], "s1");
                gen3r(f, "sub", "s2", "s0", "s1");
                ir2m(f, "s2", tokens[2]);
                break;
            case 'f':
                m2fr(f, P_FLOAT, tokens[3], "fs0");
                m2fr(f, P_FLOAT, tokens[4], "fs1");
                gen3r(f, "fsub.s", "fs2", "fs0", "fs1");
                fr2m(f, "fs2", tokens[2]);
                break;
            case 'd':
                m2fr(f, P_DOUBLE, tokens[3], "fs0");
                m2fr(f, P_DOUBLE, tokens[4], "fs1");
                gen3r(f, "fsub.d", "fs2", "fs0", "fs1");
                fr2m(f, "fs2", tokens[2]);
                break;
            default: assert(false);
            }
        }
        else if(tokens[0] == "Mul")
        {
            switch(tokens[1][0])
            {
            case 'i':
                if(optimize_level > 0 && tokens[4][0] != '%')
                {
                    if(stoi(tokens[4], 0, 0) == 0 ||
                       (tokens[3][0] != '%' && stoi(tokens[3], 0, 0) == 0))
                        { ir2m(f, "zero", tokens[2]); break; }
                    if(tokens[3][0] != '%')
                    {
                        int res = stoi(tokens[3], 0, 0) * stoi(tokens[4], 0, 0);
                        assert(res != 0);
                        gen2r(f, "li", "s0", to_string(res));
                        ir2m(f, "s0", tokens[2]); break;
                    }
                    int imm4 = stoi(tokens[4], 0, 0);
                    if(imm4 == 1) { m2ir(f, P_INT, tokens[3], "s0"); ir2m(f, "s0", tokens[2]); break; }
                    if(imm4 > 0 && (imm4 & (imm4 - 1)) == 0)
                    {
                        int shift = 0; while(imm4 != 1) { imm4 >>= 1; shift++; }
                        m2ir(f, P_INT, tokens[3], "s0");
                        gen3r(f, "slliw", "s1", "s0", to_string(shift));
                        ir2m(f, "s1", tokens[2]); break;
                    }
                }
                m2ir(f, P_INT, tokens[3], "s0");
                m2ir(f, P_INT, tokens[4], "s1");
                gen3r(f, "mulw", "s2", "s0", "s1");
                ir2m(f, "s2", tokens[2]);
                break;
            case 'f':
                m2fr(f, P_FLOAT, tokens[3], "fs0");
                m2fr(f, P_FLOAT, tokens[4], "fs1");
                gen3r(f, "fmul.s", "fs2", "fs0", "fs1");
                fr2m(f, "fs2", tokens[2]);
                break;
            case 'd':
                m2fr(f, P_DOUBLE, tokens[3], "fs0");
                m2fr(f, P_DOUBLE, tokens[4], "fs1");
                gen3r(f, "fmul.d", "fs2", "fs0", "fs1");
                fr2m(f, "fs2", tokens[2]);
                break;
            default: assert(false);
            }
        }
        else if(tokens[0] == "Div")
        {
            switch(tokens[1][0])
            {
            case 'i':
                if(optimize_level > 0 && tokens[4][0] != '%' && stoi(tokens[4], 0, 0) == 1)
                {
                    m2ir(f, P_INT, tokens[3], "s0");
                    ir2m(f, "s0", tokens[2]);
                }
                else
                {
                    m2ir(f, P_INT, tokens[3], "s0");
                    m2ir(f, P_INT, tokens[4], "s1");
                    gen3r(f, "divw", "s2", "s0", "s1");
                    ir2m(f, "s2", tokens[2]);
                }
                break;
            case 'f':
                m2fr(f, P_FLOAT, tokens[3], "fs0");
                m2fr(f, P_FLOAT, tokens[4], "fs1");
                gen3r(f, "fdiv.s", "fs2", "fs0", "fs1");
                fr2m(f, "fs2", tokens[2]);
                break;
            case 'd':
                m2fr(f, P_DOUBLE, tokens[3], "fs0");
                m2fr(f, P_DOUBLE, tokens[4], "fs1");
                gen3r(f, "fdiv.d", "fs2", "fs0", "fs1");
                fr2m(f, "fs2", tokens[2]);
                break;
            default: assert(false);
            }
        }
        else if(tokens[0] == "Rem")
        {
            bool done = false;
            if(optimize_level > 0 && tokens[3][0] != '%')
            {
                int imm3 = stoi(tokens[3], 0, 0);
                if(imm3 == 1)
                {
                    ir2m(f, "zero", tokens[1]);
                    done = true;
                }
                else if(imm3 > 0 && (imm3 & (imm3 - 1)) == 0 && imm3 < 2048)
                {
                    m2ir(f, P_INT, tokens[2], "s0");
                    gen3r(f, "andi", "s1", "s0", to_string(imm3 - 1));
                    ir2m(f, "s1", tokens[1]);
                    done = true;
                }
            }
            if(!done)
            {
                m2ir(f, P_INT, tokens[2], "s0");
                m2ir(f, P_INT, tokens[3], "s1");
                gen3r(f, "remw", "s2", "s0", "s1");
                ir2m(f, "s2", tokens[1]);
            }
        }
        else if(tokens[0] == "LT")
        {
            switch(tokens[1][0])
            {
            case 'i': m2ir(f, P_INT, tokens[3], "s0"); m2ir(f, P_INT, tokens[4], "s1"); gen3r(f, "slt", "s2", "s0", "s1"); break;
            case 'f': m2fr(f, P_FLOAT, tokens[3], "fs0"); m2fr(f, P_FLOAT, tokens[4], "fs1"); gen3r(f, "flt.s", "s2", "fs0", "fs1"); break;
            case 'd': m2fr(f, P_DOUBLE, tokens[3], "fs0"); m2fr(f, P_DOUBLE, tokens[4], "fs1"); gen3r(f, "flt.d", "s2", "fs0", "fs1"); break;
            default:  assert(false);
            }
            ir2m(f, "s2", tokens[2]);
        }
        else if(tokens[0] == "LE")
        {
            switch(tokens[1][0])
            {
            case 'i': m2ir(f, P_INT, tokens[3], "s0"); m2ir(f, P_INT, tokens[4], "s1"); gen3r(f, "slt", "s2", "s1", "s0"); gen2r(f, "seqz", "s2", "s2"); break;
            case 'f': m2fr(f, P_FLOAT, tokens[3], "fs0"); m2fr(f, P_FLOAT, tokens[4], "fs1"); gen3r(f, "fle.s", "s2", "fs0", "fs1"); break;
            case 'd': m2fr(f, P_DOUBLE, tokens[3], "fs0"); m2fr(f, P_DOUBLE, tokens[4], "fs1"); gen3r(f, "fle.d", "s2", "fs0", "fs1"); break;
            default:  assert(false);
            }
            ir2m(f, "s2", tokens[2]);
        }
        else if(tokens[0] == "GT")
        {
            switch(tokens[1][0])
            {
            case 'i': m2ir(f, P_INT, tokens[3], "s0"); m2ir(f, P_INT, tokens[4], "s1"); gen3r(f, "slt", "s2", "s1", "s0"); break;
            case 'f': m2fr(f, P_FLOAT, tokens[3], "fs0"); m2fr(f, P_FLOAT, tokens[4], "fs1"); gen3r(f, "flt.s", "s2", "fs1", "fs0"); break;
            case 'd': m2fr(f, P_DOUBLE, tokens[3], "fs0"); m2fr(f, P_DOUBLE, tokens[4], "fs1"); gen3r(f, "flt.d", "s2", "fs1", "fs0"); break;
            default:  assert(false);
            }
            ir2m(f, "s2", tokens[2]);
        }
        else if(tokens[0] == "GE")
        {
            switch(tokens[1][0])
            {
            case 'i': m2ir(f, P_INT, tokens[3], "s0"); m2ir(f, P_INT, tokens[4], "s1"); gen3r(f, "slt", "s2", "s0", "s1"); gen2r(f, "seqz", "s2", "s2"); break;
            case 'f': m2fr(f, P_FLOAT, tokens[3], "fs0"); m2fr(f, P_FLOAT, tokens[4], "fs1"); gen3r(f, "fle.s", "s2", "fs1", "fs0"); break;
            case 'd': m2fr(f, P_DOUBLE, tokens[3], "fs0"); m2fr(f, P_DOUBLE, tokens[4], "fs1"); gen3r(f, "fle.d", "s2", "fs1", "fs0"); break;
            default:  assert(false);
            }
            ir2m(f, "s2", tokens[2]);
        }
        else if(tokens[0] == "EQ")
        {
            switch(tokens[1][0])
            {
            case 'i': m2ir(f, P_INT, tokens[3], "s0"); m2ir(f, P_INT, tokens[4], "s1"); gen3r(f, "xor", "s2", "s0", "s1"); gen2r(f, "seqz", "s3", "s2"); break;
            case 'f': m2fr(f, P_FLOAT, tokens[3], "fs0"); m2fr(f, P_FLOAT, tokens[4], "fs1"); gen3r(f, "feq.s", "s3", "fs1", "fs0"); break;
            case 'd': m2fr(f, P_DOUBLE, tokens[3], "fs0"); m2fr(f, P_DOUBLE, tokens[4], "fs1"); gen3r(f, "feq.d", "s3", "fs1", "fs0"); break;
            default:  assert(false);
            }
            ir2m(f, "s3", tokens[2]);
        }
        else if(tokens[0] == "NE")
        {
            switch(tokens[1][0])
            {
            case 'i': m2ir(f, P_INT, tokens[3], "s0"); m2ir(f, P_INT, tokens[4], "s1"); gen3r(f, "xor", "s2", "s0", "s1"); gen2r(f, "snez", "s3", "s2"); break;
            case 'f': m2fr(f, P_FLOAT, tokens[3], "fs0"); m2fr(f, P_FLOAT, tokens[4], "fs1"); gen3r(f, "feq.s", "s2", "fs1", "fs0"); gen2r(f, "seqz", "s3", "s2"); break;
            case 'd': m2fr(f, P_DOUBLE, tokens[3], "fs0"); m2fr(f, P_DOUBLE, tokens[4], "fs1"); gen3r(f, "feq.d", "s2", "fs1", "fs0"); gen2r(f, "seqz", "s3", "s2"); break;
            default:  assert(false);
            }
            ir2m(f, "s3", tokens[2]);
        }
        else if(tokens[0] == "Neg")
        {
            switch(tokens[1][0])
            {
            case 'i': m2ir(f, P_INT, tokens[3], "s0"); gen3r(f, "sub", "s1", "zero", "s0"); ir2m(f, "s1", tokens[2]); break;
            case 'f': m2fr(f, P_FLOAT, tokens[3], "fs0"); gen2r(f, "fneg.s", "fs1", "fs0"); fr2m(f, "fs1", tokens[2]); break;
            case 'd': m2fr(f, P_DOUBLE, tokens[3], "fs0"); gen2r(f, "fneg.d", "fs1", "fs0"); fr2m(f, "fs1", tokens[2]); break;
            default:  assert(false);
            }
        }
        else if(tokens[0] == "Not")
        {
            m2ir(f, P_BOOL, tokens[2], "s0");
            gen2r(f, "seqz", "s1", "s0");
            ir2m(f, "s1", tokens[1]);
        }
        else if(tokens[0] == "Pos")     { assert(false); }
        else if(tokens[0] == "branch")  { gen_branch(f, tokens[1], tokens[2]); }
        else if(tokens[0] == "label")   { gen_label(f, tokens[1]); }
        else if(tokens[0] == "return")  { gen_return(f, tokens.size() > 1 ? tokens[1] : ""); }
        else if(tokens[0] == "retire")  { continue; }
        else { cerr << "unknown ir: " << ir_line << endl; }
    }
}

static void o0_gen_asm()
{
    for(Funct& f : f_list)
    {
        if(!f.is_used || f.is_extern) continue;
        o0_gen_asm_func(f);
    }
}

static void put_global()
{
    for(GlobalVar gv : gv_list)
    {
        string new_name = normalize_name(gv.name);
        size_t var_size = TypeSize[gv.gtype];
        if(gv.is_unused) continue;
        if(gv.is_bss)       asm_lines.push_back(".bss");
        else if(gv.is_global_const) asm_lines.push_back(".section .rodata");
        else                 asm_lines.push_back(".data");
        asm_lines.push_back(".balign " + to_string(var_size));
        if(gv.is_bss)
        {
            asm_lines.push_back(".lcomm " + new_name + ", " + to_string(var_size));
            continue;
        }
        asm_lines.push_back(".global " + new_name);
        asm_lines.push_back(".size " + new_name + ", " + to_string(var_size));
        asm_lines.push_back(new_name + ":");
        for(int i = 0; i < var_size; i++)
            asm_lines.push_back("\t.byte " + to_string((gv.value >> (i << 3)) & 0xFF));
        asm_lines.push_back("");
    }
    for(GlobalArr ga : ga_list)
    {
        string new_name = normalize_name(ga.name);
        size_t arr_lign = TypeSize[ga.gtype];
        size_t arr_lens = ga.length;
        if(ga.is_unused) continue;
        if(ga.is_bss)       asm_lines.push_back(".bss");
        else if(ga.is_global_const) asm_lines.push_back(".section .rodata");
        else                 asm_lines.push_back(".data");
        asm_lines.push_back(".balign " + to_string(arr_lign));
        if(ga.is_bss)
        {
            asm_lines.push_back(".lcomm " + new_name + ", " + to_string(arr_lens * arr_lign));
            continue;
        }
        asm_lines.push_back(".global " + new_name);
        asm_lines.push_back(".size " + new_name + ", " + to_string(arr_lens * arr_lign));
        asm_lines.push_back(new_name + ":");
        for(int i = 0; i < arr_lens; i++)
            for(int j = 0; j < arr_lign; j++)
                asm_lines.push_back("\t.byte " + to_string((ga.values[i] >> (j << 3)) & 0xFF));
        asm_lines.push_back("");
    }
}

static void put_func()
{
    for(Funct f : f_list)
    {
        string new_name = normalize_name(f.name);
        if(!f.is_used) continue;
        if(f.is_extern) { asm_lines.push_back(".extern " + new_name); continue; }
        asm_lines.push_back(".text");
        asm_lines.push_back(".balign 2");
        asm_lines.push_back(".global " + new_name);
        asm_lines.push_back(new_name + ":");
        for(string asm_line : f.asm_lines)
            asm_lines.push_back("\t" + asm_line);
        asm_lines.push_back(".type " + new_name + ", @function");
        asm_lines.push_back(".size " + new_name + ", .-" + new_name);
    }
}

// O1 dead code elimination
map<string, set<string>> o1_func_rely;

static void o1_handle_func_rely()
{
    queue<string> qf;
    qf.push("%main");
    f_list[f_map["%main"]].is_used = true;
    while(!qf.empty())
    {
        string func_name = qf.front();
        assert(f_map.count(func_name) != 0);
        qf.pop();
        for(string sub_name : o1_func_rely[func_name])
        {
            assert(f_map.count(sub_name) != 0);
            if(f_list[f_map[sub_name]].is_used == false)
            {
                f_list[f_map[sub_name]].is_used = true;
                qf.push(sub_name);
            }
        }
    }
}

// Main entry point
void asmgen()
{
    init_extern_func();
    parse_ir();
    show_ir();
    if(optimize_level == 1)
    {
        o1_handle_func_rely();
    }
    o0_gen_asm();
    put_global();
    put_func();
}
