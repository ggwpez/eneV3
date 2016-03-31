#ifndef FMOD_H
#define FMOD_H

#include "string_def.h"

extern schar* FMod_strings[];
enum class FMod : int
{
    _NONE       = 0,
    CALL_C      = 1,
    CALL_STD    = 2,
    CALL_REG    = 4,
    GLOBAL      = 8,
    _SIZE
};

#endif // FMOD_H
