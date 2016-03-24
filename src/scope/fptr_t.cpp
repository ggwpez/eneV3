#include "fptr_t.h"
#include "target.h"

fptr_t::fptr_t() : itype(__BYTES__)
{

}

fptr_t::fptr_t(std::vector<itype*>* args, itype* ret) : itype(__BYTES__)
{
    this->args = args;
    this->ret = ret;
}

fptr_t::~fptr_t()
{
    delete this->args;
    delete this->ret;
}
