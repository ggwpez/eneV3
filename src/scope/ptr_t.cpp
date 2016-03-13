#include "ptr_t.h"

#include "value_t.h"
#include "struct_t.h"
#include "target.h"

ptr_t::ptr_t() : itype(__BITS__)
{
    to = nullptr;
}

ptr_t::ptr_t(itype* to) : itype(__BITS__)
{
    this->to = to;
}

ptr_t::~ptr_t()
{
    if (!dynamic_cast<value_t*>(to) && !dynamic_cast<struct_t*>(to))
        delete to;
}

void ptr_t::print(std::wostream& out)
{
    out << L"<ptr_t <to=";
    if (to)
        this->to->print(out);
    out << L">>";
}
