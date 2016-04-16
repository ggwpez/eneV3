#include "void_t.h"

void_t::void_t() : itype(0)
{

}

void_t::~void_t()
{

}

void void_t::print(std::wostream& out)
{
    out << L"<void_t>";
}

bool void_t::operator==(const itype& other) const
{
    return dynamic_cast<void_t const*>(&other);
}
