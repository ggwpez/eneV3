#pragma once

#include "token.hpp"
#include "string_def.h"

#include <vector>
#include <wchar.h>

class lexer
{
public:
    lexer(char* file_name);
    ~lexer();

    std::vector<tok *> *lex();

private:
    char* file_name;
    schar* input;
    size_t length;

    tok* praep(int s, int& l);
    op   oper(schar c);
    tok_type tok_typ(schar c);
    tok* number(int s, int& l);
    tok* ident(int s, int& l);
    tok* string(int s, int& l);
    tok* ddot(int s, int& l);
};
