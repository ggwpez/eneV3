//#ifndef TAST_H
//#define TAST_H
#pragma once
#include "ast.h"

typedef std::vector<tast*> tast_arr;

class ProgramNode : public tast
{
public:
    ProgramNode(tast_arr* code);
    ~ProgramNode();
    void print(std::wostream& out) const;

   tast_arr* code;
};

class BlockNode : public tast
{
public:
    BlockNode(tast_arr* code, int stack_s);
    ~BlockNode();
    void print(std::wostream& out) const;

   tast_arr* code;
   int stack_s;
};

class ExpressionNode : public tast
{
public:
    ExpressionNode(tast* exp);
    ~ExpressionNode();
    void print(std::wostream& out) const;

    tast* exp;
};

class ExpressionTermNode : public tast
{
public:
    ExpressionTermNode(tast_arr* exps);
    ~ExpressionTermNode();
    void print(std::wostream& out) const;

   tast_arr* exps;
};

class AssignNode : public tast
{
public:
    AssignNode(ExpressionTermNode* term);
    ~AssignNode();
    void print(std::wostream& out) const;

    ExpressionTermNode* term;
};

class PushNode : public tast
{
public:
    PushNode(tast* v);
    ~PushNode();
    void print(std::wostream& out) const;

    tast* v;
};

class ReturnNode : public tast
{
public:
    ReturnNode(ExpressionTermNode* val);
    ~ReturnNode();
    void print(std::wostream& out) const;

    ExpressionTermNode* val;
};

class TypeNode : public tast
{
public:
    TypeNode(itype* t);
    ~TypeNode();
    void print(std::wostream& out) const;

    itype* t;
};

class VariableNode : public tast
{
public:
    VariableNode(TypeNode* type, IdentNode* var_name);
    ~VariableNode();
    void print(std::wostream& out) const;

    TypeNode* type;
    IdentNode* var_name;
    int ebp_off;
};

class ArgNode : public tast
{
public:
    ArgNode(IdentNode* name, TypeNode* type);
    ~ArgNode();
    void print(std::wostream& out) const;

    IdentNode* name;
    TypeNode* type;
};

class ListNode : public tast               //(a, b)
{
public:
    ListNode(tast_arr* items);
    ~ListNode();
    void print(std::wostream& out) const;

   tast_arr* items;
};

class ListArgNode : public tast               //(int a, int b)
{
public:
    ListArgNode(std::vector<ArgNode*>* items);
    ~ListArgNode();
    void print(std::wostream& out) const;

    std::vector<ArgNode*>* items;
};

class FunctionHeaderNode : public tast
{
public:
    FunctionHeaderNode(TypeNode* type, IdentNode* name, ListArgNode* args, int args_size);
    ~FunctionHeaderNode();
    void print(std::wostream& out) const;

    TypeNode* type;
    IdentNode* name;
    ListArgNode* args;
    int args_size;
};

class FunctionNode : public tast
{
public:
    FunctionNode(FunctionHeaderNode* head, BlockNode* code);
    ~FunctionNode();
    void print(std::wostream& out) const;

    FunctionHeaderNode* head;
    BlockNode* code;
};

class FunctionCallNode : public tast
{
public:
    FunctionCallNode(IdentNode* target, ListNode* args);
    ~FunctionCallNode();
    void print(std::wostream& out) const;

    IdentNode* target;
    ListNode* args;
};

class IfNode : public tast
{
public:
    IfNode(ExpressionTermNode* cond, BlockNode* true_block, BlockNode* false_block);
    ~IfNode();
    void print(std::wostream& out) const;

    ExpressionTermNode* cond;
    BlockNode* true_block;
    BlockNode* false_block;
};

class WhileNode : public tast
{
public:
    WhileNode(ExpressionTermNode* cond, BlockNode* block);
    ~WhileNode();
    void print(std::wostream& out) const;

    ExpressionTermNode* cond;
    BlockNode* block;
};
