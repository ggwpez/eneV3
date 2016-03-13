#pragma once

#include "string_def.h"
#include "ast.h"

#include <stdio.h>
#include <typeinfo>
#include <stdarg.h>

extern bool war_as_error;

enum class war_t
{
    INSTANCE_OF_VOID,               //why would you do "void: tmp;"
    size
};

int WAR(war_t type, ...) __attribute__ ((noreturn));
