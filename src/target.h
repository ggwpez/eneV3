#ifndef TARGET_H
#define TARGET_H

#include <array>

#include "string_def.h"

#define __BITS__ target->bits
#define __BYTES__ target->bytes
#define eax target->regs[1][0]
#define ebx target->regs[1][1]
#define ecx target->regs[1][2]
#define edx target->regs[1][3]

struct target_t
{
    target_t(size_t bits,  schar* int_t,
             const std::array<std::array<const schar*, 4>, 4>& regs,
             const std::array<const schar*, 4>& s_strs);

    const size_t bits, bytes;
    const schar* const int_t;

    std::array<std::array<const schar*, 4>, 4> regs;
    std::array<const schar*, 4> s_strs;
};

extern target_t* target;

extern target_t target64;
extern target_t target32;
extern target_t target16;

#endif // TARGET_H
