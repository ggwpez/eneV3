#include "ptr_t.h"

#include "value_t.h"
#include "struct_t.h"
#include "target.h"

ptr_t::ptr_t() : itype(__BYTES__), to(nullptr)
{

}

ptr_t::ptr_t(itype* to) : itype(__BYTES__), to(to)
{

}

ptr_t::~ptr_t()
{
    if (!dynamic_cast<value_t*>(to) && !dynamic_cast<struct_t*>(to))
        delete to;
}

void ptr_t::print(std::wostream& out)
{
    out << L"(ptr ";
    if (to)
        this->to->print(out);
    out << L")";
}

bool ptr_t::operator==(const itype& other) const
{
    if (ptr_t const* p = dynamic_cast<ptr_t const*>(&other))
        return *this->to == *p;
    else
        return false;
}
