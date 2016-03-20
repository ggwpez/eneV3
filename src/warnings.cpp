#include "warnings.h"
#include "errors.hpp"

bool war_as_error = false;
#define w_out (*w_ss)

std::vector<std::wostringstream*>* warnings;
std::wostringstream* w_ss;

void instance_of_void(va_list ap)
{
    VariableNode* v = va_arg(ap, VariableNode*);

    w_out << L"Variable ";
    v->print(w_out);
    w_out << L" is instaiating void.";
    v->print_pos(w_out);
}

void calling_unimpl_func(va_list ap)
{
    FunctionCallUNode* c = va_arg(ap, FunctionCallUNode*);

    w_out << L"Function call ";
    c->print(w_out);
    w_out << L" is calling an unimplemented function";
    c->print_pos(w_out);
}

void reading_uninit_mem(va_list ap)
{
    OperatorNode* a = va_arg(ap, OperatorNode*);

    w_out << L"Term ";
    a->print(w_out);
    w_out << L" is using uninitialized memory.";
    a->print_pos(w_out);
}

void reading_non_ptr_type(va_list ap)
{
    OperatorNode* r = va_arg(ap, OperatorNode*);

    w_out << L"Term ";
    r->print(w_out);
    w_out << L" is dereferencing non pointer type";
    r->print_pos(w_out);
}

void op_insuff_ops(va_list ap)
{
    OperatorNode* o = va_arg(ap, OperatorNode*);

    w_out << L"Operator ";
    o->print(w_out);
    w_out << L" maybe has insufficient argument count";
    o->print_pos(w_out);
}

void arg_count_wrong(va_list ap)
{
    FunctionCallNode* c = va_arg(ap, FunctionCallNode*);

    w_out << L"You have to clear the stack yourself after call ";
    c->print(w_out);
    c->print_pos(w_out);
    w_out << L" since your call arguments dont match the header.";
}

int WAR(war_t type, ...)
{
    war_next();

    va_list ap;
    va_start(ap, type);

    switch (type)
    {
        case war_t::INSTANCE_OF_VOID:
            instance_of_void(ap);
            break;
        case war_t::CALLING_UMIMPL_FUNC:
            calling_unimpl_func(ap);
            break;
        case war_t::READING_UNINIT_MEM:
            reading_uninit_mem(ap);
            break;
        case war_t::READING_NON_PTR_TYPE:
            reading_non_ptr_type(ap);
            break;
        case war_t::OP_INSUFF_OPS:
            op_insuff_ops(ap);
            break;
        case war_t::ARG_COUNT_WRONG:
            arg_count_wrong(ap);
            break;
        default:
            ERR(err_t::GEN_WAR);
            break;
    }

    w_out << std::endl;
    va_end(ap);

    if (war_as_error)
        ERR(err_t::GEN_WAR);
    return 0;
}

void war_init()
{
    warnings = new std::vector<std::wostringstream*>();
}

void war_next()
{
    warnings->push_back(new std::wostringstream());
    w_ss = warnings->back();
}

void war_dump(std::wostream& out)
{
    if (warnings->size())
    {
        std::wostringstream b;

        out << L"Warnings (" << warnings->size() << L")" << std::endl;

        for (size_t i = 0; i < warnings->size(); i++)
        {
            out << L'(' << i << L"): "  << warnings->at(i)->str().c_str();
            delete warnings->at(i);
        }
    }

    delete warnings;
}
