#ifndef SCOPE_HPP
#define SCOPE_HPP

#include "ast.h"
#include "string_def.h"

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
    std::vector<sc_var*>* vars;
    int frame_size;
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
    itype* get_type(IdentNode* name);
    bool is_type_reg(TypeNode* searched);
    bool is_var_reg(IdentNode* name);
    bool is_fun_head_reg(IdentNode* name);
    bool is_fun_reg(IdentNode* name);

    std::vector<sc_local_alloc*>* scopes;
    std::vector<sc_type*>*  gl_types;
    std::vector<sc_head*>*  gl_heads;       //all function header, implicitly marked as extern
    std::vector<sc_fun*>*   gl_funs;
};

#endif // SCOPE_HPP
