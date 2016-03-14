#pragma once

#include "string_def.h"

#include <stdio.h>
#include <typeinfo>
#include <stdarg.h>

#define e_out std::wcerr

enum class err_t
{
    GEN,
    GEN_LEX,
    GEN_PAR,
    GEN_SC,
    GEN_SCR,
    GEN_IL,
    GEN_WAR,
    POST_PROCESSING_FAILED,
    IO_FILE_NOT_FOUND,
    LEX_WRONG,
    PAR_WRONG,
    PAR_WRONG_BUT,
    IL_TYPE_UNKNOWN,
    SC_VAR_NAME_UNKOWN,
    SC_FUN_NAME_UNKOWN,
    SC_TYPE_NAME_UNKOWN,
    SC_TYPE_EXISTS_ALREADY,
    SC_FUN_HEAD_EXISTS_ALREADY,
    SC_FUN_EXISTS_ALREADY,
    SC_VAR_EXISTS_ALREADY,
    size
};

int ERR(err_t type, ...) __attribute__ ((noreturn));
