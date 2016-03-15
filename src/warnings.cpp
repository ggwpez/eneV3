#include "warnings.h"
#include "errors.hpp"

bool war_as_error = false;
std::wostringstream* w_out;
__warning_collection* wc;

void instance_of_void(va_list ap)
{
    VariableNode* v = va_arg(ap, VariableNode*);

    *w_out << L"Variable ";
    v->print(*w_out);
    *w_out << L" is instaiating void.";
    v->print_pos(*w_out);
}

void calling_unimpl_func(va_list ap)
{
    FunctionCallUNode* c = va_arg(ap, FunctionCallUNode*);

    *w_out << L"Function call ";
    c->print(*w_out);
    *w_out << L" is calling an unimplemented function";
    c->print_pos(*w_out);
}

int WAR(war_t type, ...)
{
    war_next();
    if (!w_out)
        return -1;

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
        default:
            ERR(err_t::GEN_WAR);
            break;
    }

    *w_out << std::endl;
    va_end(ap);

    if (war_as_error)
        ERR(err_t::GEN_WAR);
}

void war_init()
{
    wc = new __warning_collection();

}

void war_next()
{
    wc->warnings->push_back(new std::wostringstream());
    w_out = wc->warnings->back();
}

void war_dump(std::wostream& out)
{
    out << L"Warnings (" << wc->warnings->size() << L")" << (wc->warnings->size() ? L":" : L"") << std::endl;

    for (std::wostringstream* s : *wc->warnings)
        out << s->str().c_str();

    delete wc;
    w_out = nullptr;
}

__warning_collection::__warning_collection()
{
    this->warnings = new std::vector<std::wostringstream*>();
}

__warning_collection::~__warning_collection()
{
    for (std::wostringstream* w : *this->warnings)
        delete w;

    delete this->warnings;
}
