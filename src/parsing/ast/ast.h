/*#ifndef AST_H
#define AST_H*/
#pragma once

#include <sstream>
#include <vector>

#include "string_def.h"
#include "lexing/token.hpp"
#include "scope/itype.h"

class IdentNode;

class ast
{
public:
    ast();
    ast(ast* code);
    ast(tok* code);
    ast(ast* start, ast* end);
    ast(tok* start, tok* end);

    void set_pos(ast* code);
    void set_pos(tok* t);
    void set_pos_end(ast* end);
    void set_pos_end(tok* end);
    void set_pos_start(ast* start);
    void set_pos_start(tok* start);
    void print_pos(std::wostream& out);

    virtual ~ast();

    char* pos_file;
    int pos_st_line;
    int pos_st_line_char;
    int pos_en_line;
    int pos_en_line_char;
};

class uast : public virtual ast
{
public:
    uast();
    virtual ~uast();
    virtual void print(std::wostream& out) const;
};

class tast : public virtual ast
{
public:
    tast();
    virtual ~tast();
    virtual void print(std::wostream& out) const;
};

#include "utast.h"
#include "tast.h"
#include "uast.h"

//#endif // AST_H

