#ifndef TARGET_H
#define TARGET_H

#include <array>

#include "string_def.h"
#include "scope/value_t.h"

#define __BITS__ target->bits
#define __BYTES__ target->bytes
#define eax target->regs[1][0]
#define ebx target->regs[1][1]
#define ecx target->regs[1][2]
#define edx target->regs[1][3]

struct target_t
{
    target_t(size_t bits,  schar* int_t,
             std::array<std::array<const schar*, 4>, 4> const& regs,
             std::array<const schar*, 4> const& s_strs,
             const std::array<const value_type, (int)value_type::size> &types);

    const size_t bits, bytes;
    const schar* const int_t;

    std::array<std::array<const schar*, 4>, 4> regs;
    std::array<const schar*, 4> s_strs;
    std::array<const value_type, (int)value_type::size> types;
};

extern target_t* target;

extern target_t target64;
extern target_t target32;
extern target_t target16;

#endif // TARGET_H
