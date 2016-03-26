#ifndef SCOPE_HPP
#define SCOPE_HPP

#include "parsing/ast/ast.h"
#include "string_def.h"

#include <stack>
#include <vector>

struct sc_type
{
    sc_type(IdentNode* name, itype* type);
    ~sc_type();

    IdentNode* name;
    itype* type;
};

struct sc_var
{
    sc_var(IdentNode* name, VariableNode* var);
    ~sc_var();

    IdentNode* name;
    VariableNode* var;
};

struct sc_fun
{
    sc_fun(IdentNode* name, FunctionNode* fun);
    ~sc_fun();

    IdentNode* name;
    FunctionNode* fun;
};

struct sc_head
{
    sc_head(IdentNode* name, FunctionHeaderNode* head);
    ~sc_head();

    IdentNode* name;
    FunctionHeaderNode* head;
};

struct sc_local_alloc
{
    sc_local_alloc();
    ~sc_local_alloc();

    std::vector<sc_var*>* vars;
};

class scope
{
public:
    scope();
    ~scope();

    void enter();
    void leave();

    void add_type(IdentNode* name, TypeNode* t);
    void add_var(VariableNode* var);
    void add_fun(FunctionNode* fun);
    void add_fun_head(FunctionHeaderNode* head);
    void rm_head(FunctionHeaderNode *head);
    tast* get_ast(IdentNode* name);
    itype* get_var_type(IdentNode *name);
    itype* get_header_type(IdentNode* name);
    itype* get_fun_type(IdentNode *name);
    FunctionNode* get_fun(IdentNode* name);
    VariableNode* get_var(IdentNode* name);
    FunctionHeaderNode* get_head(IdentNode* name);
    itype* get_type(IdentNode* name);
    bool is_type_reg(IdentNode *searched);
    bool is_var_reg(IdentNode* name);
    bool is_fun_head_reg(IdentNode* name);
    bool is_fun_reg(IdentNode* name);

    void dump();

    std::vector<sc_local_alloc*>* scopes;
    std::vector<sc_type*>*  gl_types;
    std::vector<sc_head*>*  gl_heads;       //all function header, implicitly marked as extern
    std::vector<sc_fun*>*   gl_funs;
};

#endif // SCOPE_HPP
