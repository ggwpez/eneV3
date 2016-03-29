#pragma once

#include "ast.h"
//#include "uast.h"

class BreakNode : public uast, public tast
{
public:
    BreakNode(BreakNode* n);
    void print(std::wostream& out) const;
};

class GoOnNode : public uast, public tast
{
public:
    GoOnNode(GoOnNode* n);
    GoOnNode(tok* n);
    void print(std::wostream& out) const;
};

class StringNode : public uast, public tast
{
public:
    StringNode(schar* str);
    StringNode(tok* str);
    StringNode(StringNode* n);
    ~StringNode();
    void print(std::wostream& out) const;

    schar* str;
};

class ASMNode : public uast, public tast
{
public:
    ASMNode(StringNode* str);
    ASMNode(ASMNode* n);
    void print(std::wostream& out) const;

    StringNode* str;
};

class NumNode : public uast, public tast
{
public:
    NumNode(int num);
    NumNode(NumNode* n);
    NumNode(tok* t);
    void print(std::wostream& out) const;

    int num;
};

class IdentNode : public uast, public tast
{
public:
    IdentNode();
    IdentNode(schar* str);
    IdentNode(tok* code);
    IdentNode(IdentNode* UNode);
    void print(std::wostream& out) const;
    bool operator==(const IdentNode& other) const;

    schar* str;
};

class BoolNode : public uast, public tast
{
public:
    BoolNode(BoolNode* n);
    void print(std::wostream& out) const;
};

class PopNode : public uast, public tast
{
public:
    PopNode();
    PopNode(IdentNode* target);
    PopNode(PopNode* n);
    void print(std::wostream& out) const;

    IdentNode* target;
};
