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
    POST_PROCESSING_LD_FAILED,
    POST_PROCESSING_AS_FAILED,
    IO_FILE_NOT_FOUND,
    IO_CMD_ARG_UNKNOWN,
    IO_CMD_ARG_NO_INPUT,
    IO_CMD_ARG_NO_OUTPUT,
    IO_CMD_ARG_NO_BITS,
    LEX_WRONG,
    PAR_WRONG,
    PAR_WRONG_BUT,
    CMP_BITS_UNSUPP,
    IL_TYPE_UNKNOWN,
    IL_CANT_ASSIGN_TO_TYPE,
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
