#include "il.h"
#include "warnings.h"

il::il(ProgramNode *code, std::wostringstream *ss)
{
    input = code;
    this->ss = ss;

    ss_code  = new std::wostringstream();
    ss_codeh = new std::wostringstream();
    ss_data  = new std::wostringstream();
    ss_bss   = new std::wostringstream();
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
    eml(L"mov esp, ebp" << std::endl <<
          L"pop ebp" << std::endl <<
          L"ret " << size);
}

void il::generate_global(VariableNode *code)
{
    if (! code->type->t->size)
        WAR(war_t::INSTANCE_OF_VOID, code);
}
