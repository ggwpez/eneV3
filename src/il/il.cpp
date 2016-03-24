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
    str_c = if_c = while_c = sml_c = grt_c = ssp_c = anym_c = ret_c = 0;
}

il::~il()
{
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
