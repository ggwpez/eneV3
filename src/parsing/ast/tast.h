//#ifndef TAST_H
//#define TAST_H
#pragma once
#include "ast.h"
#include "../fmod.h"

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
    AssignNode(ExpressionTermNode* term, itype* dest, itype* src);
    ~AssignNode();
    void print(std::wostream& out) const;

    ExpressionTermNode* term;

    itype* dest;
    itype* src;
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
    void print(std::wostream &out) const;

    itype* t;
};

/*class AtomTypeNode : public tast
{
public:
    AtomTypeNode(itype *t);
    ~AtomTypeNode();
    void print(std::wostream& out) const;

    itype* t;
};

class ListTypeNode;
class FptrTypeNode : public tast
{
public:
    FptrTypeNode(ListTypeNode* args, TypeNode* ret);
    ~FptrTypeNode();
    void print(std::wostream& out) const;

    ListTypeNode* args;
    TypeNode* ret;
};*/

class VariableNode : public tast
{
public:
    VariableNode(TypeNode* type, IdentNode* var_name);
    VariableNode(TypeNode* type, IdentNode* var_name, int ebp_off);
    VariableNode(VariableNode* v);
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
    ArgNode(ArgNode* v);
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
    ListArgNode(ListArgNode* v);
    ~ListArgNode();
    void print(std::wostream& out) const;

    std::vector<ArgNode*>* items;
};

class ListTypeNode : public tast               //int int
{
public:
    ListTypeNode(std::vector<TypeNode*>* items);
    ~ListTypeNode();
    void print(std::wostream& out) const;

    std::vector<TypeNode*>* items;
};

class FunctionHeaderNode : public tast
{
public:
    FunctionHeaderNode(TypeNode* type, IdentNode* name, ListArgNode* args, int args_size, FMod mods);
    FunctionHeaderNode(FunctionHeaderNode* code);
    ~FunctionHeaderNode();
    void print(std::wostream& out) const;

    TypeNode* type;
    IdentNode* name;
    ListArgNode* args;
    int args_size;
    FMod mods;
};

class FunctionExternNode : public tast
{
public:
    FunctionExternNode(IdentNode* fname);
    ~FunctionExternNode();

    IdentNode* fname;
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
    FunctionCallNode(IdentNode* target, TypeNode* return_type, ListNode* args, int args_size);
    ~FunctionCallNode();
    void print(std::wostream& out) const;

    IdentNode* target;
    TypeNode* return_type;
    ListNode* args;
    int args_size;
};

class AnomymousCallNode : public tast
{
public:
    AnomymousCallNode(ListNode* args);
    ~AnomymousCallNode();
    void print(std::wostream& out) const;

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

class OperatorNode : public uast, public tast
{
public:
    OperatorNode(op oper);
    OperatorNode(op oper, itype* operand_type);
    OperatorNode(OperatorNode* n);
    void print(std::wostream& out) const;

    itype* operand_type;
    op oper;
};
