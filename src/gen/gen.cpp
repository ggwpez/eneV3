#include "gen.h"

gen::gen(std::vector<op>* ops, std::wostringstream& out)
{
    this->ops = ops;

    this->out = out;
    this->ss_code  = new std::wostringstream();
    this->ss_codeh = new std::wostringstream();
    this->ss_data  = new std::wostringstream();
    this->ss_bss   = new std::wostringstream();
}

gen::~gen()
{
    delete ss_code;
    delete ss_codeh;
    delete ss_data;
    delete ss_bss;
}

void gen::generate()
{
    *ss_code << L"; test";
}
