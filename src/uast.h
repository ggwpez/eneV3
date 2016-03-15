//#ifndef AST_H
//#define AST_H
#pragma once
#include <iostream>
#include <vector>

#include "scope/itype.h"
#include "token.hpp"
#include "string_def.h"
#include "ast.h"

typedef std::vector<uast*> uast_arr;

class ProgramUNode : public uast
{
public:
    ProgramUNode(uast_arr* code);
    ~ProgramUNode();
    void print(std::wostream& out) const;

   uast_arr* code;
};

class BlockUNode : public uast
{
public:
    BlockUNode(uast_arr* code);
    ~BlockUNode();
    void print(std::wostream& out) const;

   uast_arr* code;
};

class ExpressionUNode : public uast
{
public:
    ExpressionUNode(uast* exp);
    void print(std::wostream& out) const;

    uast* exp;
};

class ExpressionTermUNode : public uast
{
public:
    ExpressionTermUNode(uast_arr* exps);
    ~ExpressionTermUNode();
    void print(std::wostream& out) const;

   uast_arr* exps;
};

class AssignUNode : public uast
{
public:
    AssignUNode(ExpressionTermUNode* term);
    void print(std::wostream& out) const;

    ExpressionTermUNode* term;
};

class BinaryUNode : public uast
{
public:
    void print(std::wostream& out) const;
};

class UnaryUNode : public uast
{
public:
    void print(std::wostream& out) const;
};

class PushUNode : public uast
{
public:
    PushUNode(uast* v);
    void print(std::wostream& out) const;

    uast* v;
};

class ReturnUNode : public uast
{
public:
    ReturnUNode(ExpressionTermUNode* val);
    void print(std::wostream& out) const;

    ExpressionTermUNode* val;
};

class TypeUNode : public uast
{
public:
    TypeUNode(std::vector<IdentNode*>* parts);
    ~TypeUNode();
    void print(std::wostream& out) const;

    std::vector<IdentNode*>* parts;
};

class VariableUNode : public uast
{
public:
    VariableUNode(TypeUNode* type_name, IdentNode* var_name);
    ~VariableUNode();
    void print(std::wostream& out) const;

    TypeUNode* type_name;
    IdentNode* var_name;
};

class ArgUNode : public uast
{
public:
    ArgUNode(IdentNode* name, TypeUNode* type);
    ~ArgUNode();
    void print(std::wostream& out) const;

    IdentNode* name;
    TypeUNode* type;
};

class ListUNode : public uast               //(a, b)
{
public:
    ListUNode(uast_arr* items);
    ~ListUNode();
    void print(std::wostream& out) const;

   uast_arr* items;
};

class ListArgUNode : public uast               //(int a, int b)
{
public:
    ListArgUNode(std::vector<ArgUNode*>* items);
    ~ListArgUNode();
    void print(std::wostream& out) const;

    std::vector<ArgUNode*>* items;
};

class FunctionHeaderUNode : public uast
{
public:
    FunctionHeaderUNode(TypeUNode* type, IdentNode* name, ListArgUNode* args);
    ~FunctionHeaderUNode();
    void print(std::wostream& out) const;

    TypeUNode* type;
    IdentNode* name;
    ListArgUNode* args;
};

class FunctionUNode : public uast
{
public:
    FunctionUNode(FunctionHeaderUNode* head, BlockUNode* code);
    ~FunctionUNode();
    void print(std::wostream& out) const;

    FunctionHeaderUNode* head;
    BlockUNode* code;
};

class FunctionCallUNode : public uast
{
public:
    FunctionCallUNode(IdentNode* target, ListUNode* args);
    ~FunctionCallUNode();
    void print(std::wostream& out) const;

    IdentNode* target;
    ListUNode* args;
};

class IfUNode : public uast
{
public:
    IfUNode(ExpressionTermUNode* cond, BlockUNode* true_block, BlockUNode* false_block);
    ~IfUNode();
    void print(std::wostream& out) const;

    ExpressionTermUNode* cond;
    BlockUNode* true_block;
    BlockUNode* false_block;
};

class WhileUNode : public uast
{
public:
    WhileUNode(ExpressionTermUNode* cond, BlockUNode* block);
    ~WhileUNode();
    void print(std::wostream& out) const;

    ExpressionTermUNode* cond;
    BlockUNode* block;
};

class OperatorUNode : public uast, public tast
{
public:
    OperatorUNode(op oper);
    OperatorUNode(tok* code);
    OperatorUNode(OperatorUNode* n);
    void print(std::wostream& out) const;

    op oper;
};

//#endif // AST_H
