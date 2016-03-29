#ifndef TARGET_H
#define TARGET_H

#include <array>

#include "string_def.h"
#include "scope/value_t.h"

#define __BITS__ target->bits
#define __BYTES__ target->bytes

#define rax target->regs[0][0]
#define rbx target->regs[0][1]
#define rcx target->regs[0][2]
#define rdx target->regs[0][3]

#define eax target->regs[1][0]
#define ebx target->regs[1][1]
#define ecx target->regs[1][2]
#define edx target->regs[1][3]

#define  ax target->regs[2][0]
#define  bx target->regs[2][1]
#define  cx target->regs[2][2]
#define  dx target->regs[2][3]

#define  al target->regs[3][0]
#define  bl target->regs[3][1]
#define  cl target->regs[3][2]
#define  dl target->regs[3][3]

#define _EX_NASM ".nasm"
#define _EX_GAS  ".s"
#define _EX_OBJ  ".obj"

#define EX_ASM(s) std::string(s == as::NASM ? _EX_NASM : _EX_GAS)

enum class as
{
    NASM,
    GAS,
    size
};

#define AS_L 2
struct target_t
{
    target_t(size_t bits,  schar* int_t,
             std::array<std::array<const schar*, 4>, 4> const& regs,
             std::array<const schar*, 4> const& s_strs,
             const std::array<const value_type, (int)value_type::size> &types,
             as assembler);

    const size_t bits, bytes;
    const schar* const int_t;

    std::array<std::array<const schar*, 4>, 4> regs;
    std::array<const schar*, 4> s_strs;
    std::array<const value_type, (int)value_type::size> types;
    as assembler;
};

extern target_t* target;

extern target_t* targets[];
extern target_t target_nasm_64;
extern target_t target_nasm_32;
extern target_t target_nasm_16;

extern target_t target_gas_64;
extern target_t target_gas_32;
extern target_t target_gas_16;

#endif // TARGET_H
