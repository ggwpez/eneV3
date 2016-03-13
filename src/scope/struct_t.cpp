#include "struct_t.h"

struct_t::struct_t()
{

}

void struct_t::print(std::wostream& out)
{
    out << L"<struct <name=" << this->name << L"><members=";

    for (itype* t : *this->member)
        t->print(out);

    out << L">>";
}
