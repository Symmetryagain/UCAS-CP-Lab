#ifndef CACT_ASM_GEN_H
#define CACT_ASM_GEN_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "context.h"

enum GType
{
    G_INT    = 0,
    G_BOOL   = 1,
    G_FLOAT  = 2,
    G_DOUBLE = 3,
};

constexpr size_t TypeSize[] = {4, 1, 4, 8, 8, 8, 8, 8, 8};

struct GlobalVar
{
    std::string   name;
    GType         gtype;
    uint64_t      value;
    bool          is_bss;
    bool          is_global_const;
    bool          is_unused;
};

extern std::vector<GlobalVar>   gv_list;
extern std::map<std::string, size_t> gv_map;

struct GlobalArr
{
    std::string                name;
    GType                      gtype;
    size_t                     length;
    std::vector<uint64_t>      values;
    bool                       is_bss;
    bool                       is_global_const;
    bool                       is_unused;
};

extern std::vector<GlobalArr>   ga_list;
extern std::map<std::string, size_t> ga_map;

enum FType
{
    F_INT    = 0,
    F_BOOL   = 1,
    F_FLOAT  = 2,
    F_DOUBLE = 3,
    F_VOID   = 12
};

enum PType
{
    P_INT        = 0,
    P_BOOL       = 1,
    P_FLOAT      = 2,
    P_DOUBLE     = 3,
    P_PTR_INT    = 4,
    P_PTR_BOOL   = 5,
    P_PTR_FLOAT  = 6,
    P_PTR_DOUBLE = 7,
    P_REG        = 8,
};

struct Param
{
    std::string name;
    PType       ptype;
    size_t      reg_pos;
};

struct LocalVar
{
    std::string name;
    PType       ptype;
    size_t      sp_pos;
};

struct LocalArr
{
    std::string name;
    GType       gtype;
    size_t      length;
    size_t      sp_pos;
};

struct Funct
{
    std::string                  name;
    FType                        ftype;
    size_t                       gpr_a = 0;
    size_t                       fpr_a = 0;
    std::vector<Param>           p_list;
    std::map<std::string, size_t> p_map;
    std::vector<LocalVar>        lv_list;
    std::map<std::string, size_t> lv_map;
    std::vector<LocalArr>        la_list;
    std::map<std::string, size_t> la_map;
    size_t                       sp_size;
    std::vector<std::string>     ir_lines;
    std::vector<std::string>     asm_lines;
    bool                         is_extern;
    bool                         is_used;
};

extern std::vector<Funct>         f_list;
extern std::map<std::string, size_t> f_map;

void asmgen();

#endif // CACT_ASM_GEN_H
