#include "errors.hpp"
#include "token.hpp"
#include "scope/itype.h"
#include "ast.h"

#include <iostream>

void print_pos(tok* token)
{
    std::wcerr << L" in file "<< token->pos_file
               << L" line " << token->pos_line
               << L" pos " << token->pos_line_char;
}

void par_wrong(va_list ap)
{
    tok* token = va_arg(ap, tok*);

    std::wcerr << L"Invalid token: " << token->to_str();
    print_pos(token);
}

void par_wrong_but(va_list ap)
{
    tok* token = va_arg(ap, tok*);
    tok_type wanted = va_arg(ap, tok_type);

    std::wcerr << L"Invalid token: " << token->to_str()
               << L" awaited " << tok_strings[(int)wanted];
    print_pos(token);
}

void lex_wrong(va_list ap)
{
    schar c = ((schar)va_arg(ap, int));

    std::wcerr << L"Invalid char " << c;
}

void il_type_unknown(va_list ap)
{
    itype* t = va_arg(ap, itype*);

    std::wcerr << L"Type ";
    t->print(std::wcerr);
    std::wcerr << L" unknown";
}

void sc_type_name_unkown(va_list ap)
{
    IdentNode* i = va_arg(ap, IdentNode*);

    std::wcerr << L"Type name ";
    i->print(std::wcerr);
    std::wcerr << L" unknown";
}

void sc_var_name_unkown(va_list ap)
{
    IdentNode* i = va_arg(ap, IdentNode*);

    std::wcerr << L"Variable ";
    i->print(std::wcerr);
    std::wcerr << L" unknown";
}

void sc_type_exists_already(va_list ap)
{
    itype* t = va_arg(ap, itype*);

    std::wcerr << L"Type ";
    t->print(std::wcerr);
    std::wcerr << L" exists already";
}

void sc_fun_exists_already(va_list ap)
{
    FunctionNode* i = va_arg(ap, FunctionNode*);

    std::wcerr << L"Function ";
    i->head->print(std::wcerr);
    std::wcerr << L" exists already";
}

void sc_var_exists_already(va_list ap)
{
    VariableNode* i = va_arg(ap, VariableNode*);

    std::wcerr << L"Variable ";
    i->var_name->print(std::wcerr);
    std::wcerr << L" exists already";
}

int ERR(err_t type, ...)
{
    std::wcerr << L"Error, ";

    va_list ap;
    va_start(ap, type);

    switch (type)
    {
        case err_t::GEN:
            std::wcerr << L"Generic error";
            break;
        case err_t::GEN_IL:
            std::wcerr << L"Generic code generator error";
            break;
        case err_t::GEN_SC:
            std::wcerr << L"Generic scope error";
            break;
        case err_t::GEN_SCR:
            std::wcerr << L"Generic scoper error";
            break;
        case err_t::GEN_PAR:
            std::wcerr << L"Generic parsing error";
            break;
        case err_t::GEN_LEX:
            std::wcerr << L"Generic lexing error";
            break;
        case err_t::GEN_WAR:
            std::wcerr << L"Last warning treated as error.";
            break;
        case err_t::PAR_WRONG:
            par_wrong(ap);
            break;
        case err_t::PAR_WRONG_BUT:
            par_wrong_but(ap);
            break;
        case err_t::LEX_WRONG:
            lex_wrong(ap);
            break;
        case err_t::IL_TYPE_UNKNOWN:
            il_type_unknown(ap);
            break;
        case err_t::SC_TYPE_NAME_UNKOWN:
            sc_type_name_unkown(ap);
            break;
        case err_t::SC_VAR_NAME_UNKOWN:
            sc_var_name_unkown(ap);
            break;
        case err_t::SC_TYPE_EXISTS_ALREADY:
            sc_type_exists_already(ap);
            break;
        case err_t::SC_FUN_EXISTS_ALREADY:
            sc_fun_exists_already(ap);
            break;
        case err_t::SC_VAR_EXISTS_ALREADY:
            sc_var_exists_already(ap);
            break;
        default:
            std::wcerr << L"Unhandled error.";
    }

    std::wcerr << std::endl;
    va_end(ap);

    exit(-1);
}
