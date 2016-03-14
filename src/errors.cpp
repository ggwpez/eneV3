#include "errors.hpp"
#include "token.hpp"
#include "scope/itype.h"
#include "ast.h"

#include <iostream>
void post_processing_failed(va_list ap)
{
    e_out << L"Assemling (nasm) or linking (GNU ld) failed." << std::endl;
}

void io_file_not_found(va_list ap)
{
    char* fn = va_arg(ap, char*);

    e_out << L"Could not find file " << fn;
}

void par_wrong(va_list ap)
{
    tok* token = va_arg(ap, tok*);

    e_out << L"Invalid token: " << token->to_str();
    token->print_pos(e_out);
}

void par_wrong_but(va_list ap)
{
    tok* token = va_arg(ap, tok*);
    tok_type wanted = va_arg(ap, tok_type);

    e_out << L"Invalid token: " << token->to_str()
               << L" awaited " << tok_strings[(int)wanted];
    token->print_pos(e_out);
}

void lex_wrong(va_list ap)
{
    schar c = ((schar)va_arg(ap, int));

    e_out << L"Invalid char " << c;
}

void il_type_unknown(va_list ap)
{
    TypeNode* t = va_arg(ap, TypeNode*);

    e_out << L"Type ";
    t->print(e_out);
    e_out << L" unknown";
    t->print_pos(e_out);
}

void sc_type_name_unkown(va_list ap)
{
    IdentNode* i = va_arg(ap, IdentNode*);

    e_out << L"Type name ";
    i->print(e_out);
    e_out << L" unknown";
    i->print_pos(e_out);
}

void sc_fun_name_unkown(va_list ap)
{
    IdentNode* i = va_arg(ap, IdentNode*);

    e_out << L"Function name ";
    i->print(e_out);
    e_out << L" unknown";
    i->print_pos(e_out);
}

void sc_var_name_unkown(va_list ap)
{
    IdentNode* i = va_arg(ap, IdentNode*);

    e_out << L"Variable ";
    i->print(e_out);
    e_out << L" unknown";
    i->print_pos(e_out);
}

void sc_type_exists_already(va_list ap)
{
    itype* t = va_arg(ap, itype*);

    e_out << L"Type ";
    t->print(e_out);
    e_out << L" exists already";
}

void sc_fun_head_exists_already(va_list ap)
{
    FunctionHeaderNode* i = va_arg(ap, FunctionHeaderNode*);

    e_out << L"Header for function " << i->name->str << L" exists already";
    i->print_pos(e_out);
}

void sc_fun_exists_already(va_list ap)
{
    FunctionNode* i = va_arg(ap, FunctionNode*);

    e_out << L"Function " << i->head->name->str << L" exists already";
    i->head->name->print_pos(e_out);
}

void sc_var_exists_already(va_list ap)
{
    VariableNode* i = va_arg(ap, VariableNode*);

    e_out << L"Variable ";
    i->var_name->print(e_out);
    e_out << L" exists already";
}

int ERR(err_t type, ...)
{
    e_out << L"Error, ";

    va_list ap;
    va_start(ap, type);

    switch (type)
    {
        case err_t::GEN:
            e_out << L"Generic error";
            break;
        case err_t::GEN_IL:
            e_out << L"Generic code generator error";
            break;
        case err_t::GEN_SC:
            e_out << L"Generic scope error";
            break;
        case err_t::GEN_SCR:
            e_out << L"Generic scoper error";
            break;
        case err_t::GEN_PAR:
            e_out << L"Generic parsing error";
            break;
        case err_t::GEN_LEX:
            e_out << L"Generic lexing error";
            break;
        case err_t::GEN_WAR:
            e_out << L"Last warning treated as error.";
            break;
        case err_t::POST_PROCESSING_FAILED:
            post_processing_failed(ap);
            break;
        case err_t::IO_FILE_NOT_FOUND:
            io_file_not_found(ap);
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
        case err_t::SC_FUN_NAME_UNKOWN:
            sc_fun_name_unkown(ap);
            break;
        case err_t::SC_VAR_NAME_UNKOWN:
            sc_var_name_unkown(ap);
            break;
        case err_t::SC_TYPE_EXISTS_ALREADY:
            sc_type_exists_already(ap);
            break;
        case err_t::SC_FUN_HEAD_EXISTS_ALREADY:
            sc_fun_exists_already(ap);
            break;
        case err_t::SC_FUN_EXISTS_ALREADY:
            sc_fun_exists_already(ap);
            break;
        case err_t::SC_VAR_EXISTS_ALREADY:
            sc_var_exists_already(ap);
            break;
        default:
            e_out << L"Unhandled error.";
    }

    e_out << std::endl;
    va_end(ap);

    exit(-1);
}
