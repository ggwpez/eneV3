#include "errors.hpp"
#include "lexing/token.hpp"
#include "scope/itype.h"
#include "parsing/ast/ast.h"
#include "warnings.h"

#include <iostream>

void post_processing_ld_failed(va_list ap)
{
    e_out << L"Linking (GNU ld) failed." << std::endl;
}

void post_processing_as_failed(va_list ap)
{
    e_out << L"Assemling (nasm) failed." << std::endl;
}

void io_file_not_found(va_list ap)
{
    char* fn = va_arg(ap, char*);

    e_out << L"Could not find file " << fn;
}

void io_cmg_arg_unknown(va_list ap)
{
    char* arg = va_arg(ap, char*);

    e_out << L"Argument " << arg << L" unknown.";
}

void io_cmd_arg_as_unknown(va_list ap)
{
    char* i = va_arg(ap, char*);

    e_out << i << L" is not a valid assembler.";
}

void io_cmd_arg_no_bits()
{
    e_out << L"Bit count not set (16/32/64).";
}

void io_cmd_arg_no_input()
{
    e_out << L"No input files set.";
}

void io_cmd_arg_no_output()
{
    e_out << L"No output file set.";
}

void io_cmd_arg_no_as()
{
    e_out << L"No assembler set.";
}

void par_wrong(va_list ap)
{
    tok* token = va_arg(ap, tok*);

    e_out << L"Invalid token: " << token->to_str();
    token->print_pos(e_out);
}

void par_wrong_but(va_list ap)
{
    tok_type wanted = (tok_type)va_arg(ap, int);
    tok* token = va_arg(ap, tok*);

    e_out << L"Invalid token: " << token->to_str()
               << L" awaited " << tok_strings[(int)wanted];
    token->print_pos(e_out);
}

void lex_wrong(va_list ap)
{
    schar c = ((schar)va_arg(ap, int));

    e_out << L"Invalid char " << c;
}

void cmp_bits_unsupp(va_list ap)
{
    size_t b = va_arg(ap, size_t);

    e_out << b << L" is not supported as bit count.";
}

void il_type_unknown(va_list ap)
{
    TypeNode* t = va_arg(ap, TypeNode*);

    e_out << L"Type ";
    t->print(e_out);
    e_out << L" unknown";
    t->print_pos(e_out);
}

void il_cant_assign_to_type(va_list ap)
{
    AssignNode* t = va_arg(ap, AssignNode*);

    e_out << L"Cant write to type ";
    t->to_write->print(e_out);
    e_out << L" in ";
    t->print(e_out);
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
            war_dump(std::wcerr);
            e_out << L"Last warning treated as error, due to -p (pedantic error)";
            break;
        case err_t::POST_PROCESSING_LD_FAILED:
            post_processing_ld_failed(ap);
            break;
        case err_t::POST_PROCESSING_AS_FAILED:
            post_processing_as_failed(ap);
            break;
        case err_t::IO_FILE_NOT_FOUND:
            io_file_not_found(ap);
            break;
        case err_t::IO_CMD_ARG_UNKNOWN:
            io_cmg_arg_unknown(ap);
            break;
        case err_t::IO_CMD_ARG_AS_UNKNOWN:
            io_cmd_arg_as_unknown(ap);
            break;
        case err_t::IO_CMD_ARG_NO_BITS:
            io_cmd_arg_no_bits();
            break;
        case err_t::IO_CMD_ARG_NO_INPUT:
            io_cmd_arg_no_input();
            break;
        case err_t::IO_CMD_ARG_NO_OUTPUT:
            io_cmd_arg_no_output();
            break;
        case err_t::IO_CMD_ARG_NO_AS:
            io_cmd_arg_no_as();
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
        case err_t::CMP_BITS_UNSUPP:
            cmp_bits_unsupp(ap);
            break;
        case err_t::IL_TYPE_UNKNOWN:
            il_type_unknown(ap);
            break;
        case err_t::IL_CANT_ASSIGN_TO_TYPE:
            il_cant_assign_to_type(ap);
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

    std::wcout << std::flush;
    std::cout << std::flush;
    exit(-1);
}
