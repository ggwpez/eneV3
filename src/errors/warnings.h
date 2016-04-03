#pragma once

#include "string_def.h"
#include "parsing/ast/ast.h"

//#include <stdio.h>
//#include <typeinfo>
#include <cstdarg>
//#include <vector>

extern bool war_as_error;

enum class war_t : int
{
    INSTANCE_OF_VOID,               //why would you do "void: tmp;"
    CALLING_UMIMPL_FUNC,
    READING_UNINIT_MEM,
    READING_NON_PTR_TYPE,
    ASSIGN_NO_TYPE,
    NO_RET_TYPE,
    WRONG_RET_TYPE,
    VOID_RET,
    OP_INSUFF_OPS,
    ARG_COUNT_WRONG,
    size
};

struct war_alloc
{
    war_alloc(std::string const& file_name);
    ~war_alloc();

    std::string const& file_name;
    std::vector<std::wostringstream*>* ss;
};

void war_init();
void war_enter(std::string const& file_name);
void war_next();
void war_dump(std::wostream& out);
void war_close();
int WAR(war_t type, ...);
