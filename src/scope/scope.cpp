#include "scope.hpp"
#include "errors.hpp"

#include "value_t.h"
#include "void_t.h"
#include "target.h"

sc_type::sc_type(IdentNode* name, itype *type)
{
    this->name = name;
    this->type = type;
}

sc_type::~sc_type()
{
    delete this->name;
    delete this->type;
}

sc_var::sc_var(IdentNode* name, VariableNode* var)
{
    this->name = name;
    this->var = var;
}

sc_var::~sc_var()
{
    delete this->name;
}

sc_fun::sc_fun(IdentNode* name, FunctionNode* fun)
{
    this->name = name;
    this->fun = fun;
}

sc_fun::~sc_fun()
{
    delete this->name;
}

sc_head::sc_head(IdentNode* name, FunctionHeaderNode* head)
{
    this->name = name;
    this->head = head;
}

sc_head::~sc_head()
{
    delete this->name;
}

scope::scope()
{
    this->scopes = new std::vector<sc_local_alloc*>();
    this->gl_types = new std::vector<sc_type*>();
    this->gl_heads = new std::vector<sc_head*>();
    this->gl_funs = new std::vector<sc_fun*>();

    this->enter();

    //register inbuild types like i8 i16...
    for (value_type t : target->types)
        if (t != value_type::size)
            this->gl_types->push_back(new sc_type(new IdentNode(value_type_strings[(int)t]),
                                              new value_t(t)));


    this->gl_types->push_back(new sc_type(new IdentNode(L"void"), new void_t()));
}

scope::~scope()
{
    this->leave();

    if (this->scopes->size())
        ERR(err_t::GEN_SC);

    for (sc_type* gt : *this->gl_types)
        delete gt;
    for (sc_head* gh : *this->gl_heads)   //gets freed in ~ProgramNode
        delete gh;
    for (sc_fun* gf : *this->gl_funs)
        delete gf;

    delete this->gl_types;
    delete this->gl_heads;
    delete this->gl_funs;
}

void scope::enter()
{
    this->scopes->push_back(new sc_local_alloc());
}

void scope::leave()
{
    delete this->scopes->back();

    this->scopes->pop_back();
}

void scope::add_type(IdentNode* name, TypeNode* t)
{
    if (is_type_reg(name))
        ERR(err_t::SC_TYPE_EXISTS_ALREADY, t);

    this->gl_types->push_back(new sc_type(new IdentNode(name), t->t));
}

void scope::add_var(VariableNode* var)
{
    if (this->is_var_reg(var->var_name))
        ERR(err_t::SC_VAR_EXISTS_ALREADY, var);

    this->scopes->back()->vars->push_back(new sc_var(new IdentNode(var->var_name), var));
}

void scope::add_fun_head(FunctionHeaderNode* head)
{
    if (this->is_fun_head_reg(head->name))
        ERR(err_t::SC_FUN_HEAD_EXISTS_ALREADY, head);

    this->gl_heads->push_back(new sc_head(new IdentNode(head->name), head));
}

void scope::rm_head(FunctionHeaderNode* head)
{
    if (!this->is_fun_head_reg(head->name))
        ERR(err_t::SC_FUN_NAME_UNKOWN, head->name);     //same as above

    for (size_t i = 0; i < gl_heads->size(); i++)
        if (*gl_heads->at(i)->name == *head->name)
        {
            this->gl_heads->erase(gl_heads->begin() +i);
            break;
        }
}

void scope::add_fun(FunctionNode* fun)
{
    if (this->is_fun_reg(fun->head->name))
        ERR(err_t::SC_FUN_EXISTS_ALREADY, fun);

    if (this->is_fun_head_reg(fun->head->name))


    this->gl_funs->push_back(new sc_fun(new IdentNode(fun->head->name), fun));
}

itype* scope::get_var_type(IdentNode* name)
{
    for (sc_local_alloc* sc : *this->scopes)
        for (sc_var* var : *sc->vars)
            if (*var->name == *name)
                return var->var->type->t;

    ERR(err_t::SC_VAR_NAME_UNKOWN, name);
}

itype* scope::get_header_type(IdentNode* name)
{
    for (sc_head* t : *this->gl_heads)
        if (*t->name == *name)
            return t->head->type->t;

    ERR(err_t::SC_VAR_NAME_UNKOWN, name);
}

itype* scope::get_fun_type(IdentNode* name)
{
    for (sc_fun* t : *this->gl_funs)
        if (*t->name == *name)
            return t->fun->head->type->t;

    ERR(err_t::SC_VAR_NAME_UNKOWN, name);
}

FunctionNode* scope::get_fun(IdentNode* name)
{
    for (sc_fun* t : *this->gl_funs)
        if (*t->name == *name)
            return t->fun;

    ERR(err_t::SC_VAR_NAME_UNKOWN, name);
}

FunctionHeaderNode* scope::get_head(IdentNode* name)
{
    for (sc_head* t : *this->gl_heads)
        if (*t->name == *name)
            return t->head;

    ERR(err_t::SC_VAR_NAME_UNKOWN, name);
}

itype* scope::get_type(IdentNode* name)
{
    for (sc_type* t : *this->gl_types)
        if (*t->name == *name)
            return t->type;

    ERR(err_t::SC_TYPE_NAME_UNKOWN, name);
}

bool scope::is_type_reg(IdentNode* searched)
{
    for (sc_type* t : *this->gl_types)
        if (*t->name == *searched)
            return true;

    return false;
}

bool scope::is_var_reg(IdentNode* name)
{
    for (sc_local_alloc* sc : *this->scopes)
        for (sc_var* var : *sc->vars)
            if (*var->name == *name)
                return true;

    return false;
}

bool scope::is_fun_head_reg(IdentNode* name)
{
    for (sc_head* t : *this->gl_heads)
        if (*t->name == *name)
            return true;

    return false;
}

bool scope::is_fun_reg(IdentNode* name)
{
    for (sc_fun* t : *this->gl_funs)
        if (*t->name == *name)
            return true;

    return false;
}
