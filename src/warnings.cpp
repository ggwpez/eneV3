#include "warnings.h"
#include "errors.hpp"

bool war_as_error = false;

void instance_of_void(va_list ap)
{
    VariableNode* v = va_arg(ap, VariableNode*);
}

int WAR(war_t type, ...)
{
    std::wcout << L"Warning, ";

    va_list ap;
    va_start(ap, type);

    switch (type)
    {
        case war_t::INSTANCE_OF_VOID:
            instance_of_void(ap);
            break;
        default:

            break;
    }

    std::wcout << std::endl;
    va_end(ap);

    if (war_as_error)
        ERR(err_t::GEN_WAR);
}
