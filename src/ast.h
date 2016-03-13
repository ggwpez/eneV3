/*#ifndef AST_H
#define AST_H*/
#pragma once

#include <sstream>
#include <vector>

#include "string_def.h"
#include "token.hpp"
#include "scope/itype.h"

class IdentNode;

class uast
{
public:
    uast();
    virtual ~uast();
    virtual void print(std::wostream& out) const;
};

class tast
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

