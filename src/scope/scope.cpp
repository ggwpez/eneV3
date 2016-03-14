#include "scope.hpp"
#include "errors.hpp"

#include "value_t.h"
#include "void_t.h"

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

scope::scope()
{
    this->scopes = new std::vector<sc_local_alloc*>();
    this->gl_types = new std::vector<sc_type*>();
    this->gl_funs = new std::vector<sc_fun*>();

    this->enter();

    //register inbuild types like i8 i16...
    for (int i = (int)value_type::I8; i < (int)value_type::size; i++)
        this->gl_types->push_back(new sc_type(new IdentNode(value_type_strings[i]),
                                              new value_t((value_type)i)));

    this->gl_types->push_back(new sc_type(new IdentNode(L"void"), new void_t()));
}

scope::~scope()
{
    this->leave();

    if (this->scopes->size())
        ERR(err_t::GEN_SC);

    for (sc_type* gt : *this->gl_types)
        delete gt;
    for (sc_fun* gf : *this->gl_funs)
        delete gf;

    delete this->gl_types;
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
    if (is_type_reg(t))
        ERR(err_t::SC_TYPE_EXISTS_ALREADY, t);

    this->gl_types->push_back(new sc_type(new IdentNode(name), t->t));
}

void scope::add_var(VariableNode* var)
{
    if (this->is_var_reg(var->var_name))
        ERR(err_t::SC_VAR_EXISTS_ALREADY, var);

    this->scopes->back()->vars->push_back(new sc_var(new IdentNode(var->var_name), var));
}

void scope::add_fun(FunctionNode* fun)
{
    if (this->is_fun_reg(fun->head->name))
        ERR(err_t::SC_FUN_EXISTS_ALREADY, fun);

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

itype* scope::get_type(IdentNode* name)
{
    for (sc_type* t : *this->gl_types)
        if (*t->name == *name)
            return t->type;

    ERR(err_t::SC_TYPE_NAME_UNKOWN, name);
}

bool scope::is_type_reg(TypeNode* searched)
{
    for (sc_type* t : *this->gl_types)
        if (t->type == searched->t)
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

bool scope::is_fun_reg(IdentNode* name)
{
    for (sc_fun* t : *this->gl_funs)
        if (*t->name == *name)
            return true;

    return false;
}
