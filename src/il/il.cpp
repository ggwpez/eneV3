#include "il.h"
#include "errors/warnings.h"
#include "errors/errors.hpp"

il::il(ProgramNode *code, std::wostringstream *ss)
{
    input = code;
    this->ss = ss;

    ss_code  = new std::wostringstream();
    ss_codeh = new std::wostringstream();
    ss_data  = new std::wostringstream();
    ss_bss   = new std::wostringstream();

    funtion_returns = std::stack<schar*>();
    registered_strings = std::unordered_map<schar*, std::wstring*>();
    str_c = blk_c = sml_c = grt_c = ssp_c = anym_c = 0;
}

il::~il()
{
    for (std::pair<schar*, std::wstring*> i : this->registered_strings)
        delete i.second;

    delete ss_code;
    delete ss_codeh;
    delete ss_data;
    delete ss_bss;
}

void il::generate_sf_enter(int size)
{
    eml(L"push ebp" << std::endl <<
        L"mov ebp, esp" << std::endl <<
        L"sub esp, " << size);
}

void il::generate_sf_leave(int size)
{
    eml(L"add esp, " << size << std::endl <<
        L"mov esp, ebp" << std::endl <<
        L"pop ebp" << std::endl <<
        L"ret ");
}

std::wstring* il::generate_string_name(schar* content)
{
    auto it = this->registered_strings.find(content);

    if (it == registered_strings.end())
    {
        std::wstring tmp = std::wstring(L"__str_") +std::to_wstring(++str_c);
        std::wstring* to_add = new std::wstring(tmp);
        registered_strings[content] = to_add;
        return to_add;
    }
    else
        return (*it).second;
}

void il::generate_global(VariableNode *code)
{
    if (! code->type->t->size)
        WAR(war_t::INSTANCE_OF_VOID, code);
}

void il::generate(OperatorNode* code)
{
    switch (code->oper)
    {
        case op::ADD:
            generate_op_add(code);
            break;
        case op::SUB:
            generate_op_sub(code);
            break;
        case op::MUL:
            generate_op_mul(code);
            break;
        case op::DIV:
            generate_op_div(code);
            break;
        case op::DRF:
            generate_op_drf(code);
            break;
        case op::EQU:
            generate_op_equ(code);
            break;
        case op::SML:
            generate_op_sml(code);
            break;
        case op::GRT:
            generate_op_grt(code);
            break;
        case op::NOT:
            generate_op_not(code);
            break;
        case op::POP:
            generate_op_pop(code);
            break;
        case op::CPY:
            generate_op_cpy(code);
            break;
        case op::OR:
            generate_op_or(code);
            break;
        case op::AND:
            generate_op_and(code);
            break;
        default:
            ERR(err_t::GEN_IL);
            break;
    }
};
