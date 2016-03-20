#pragma once

#include "string_def.h"
#include "ast.h"

//#include <stdio.h>
//#include <typeinfo>
#include <cstdarg>
//#include <vector>

extern bool war_as_error;


enum class war_t
{
    INSTANCE_OF_VOID,               //why would you do "void: tmp;"
    CALLING_UMIMPL_FUNC,
    READING_UNINIT_MEM,
    READING_NON_PTR_TYPE,
    OP_INSUFF_OPS,
    ARG_COUNT_WRONG,
    size
};

void war_init();
void war_next();
void war_dump(std::wostream& out);
int WAR(war_t type, ...) __attribute__ ((noreturn));
