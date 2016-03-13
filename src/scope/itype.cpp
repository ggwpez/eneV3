#include "itype.h"

itype::itype(int size)
{
    this->size = size;
}

itype::~itype()
{
}

void itype::print(std::wostream& out)
{
    out << L"<Itype>";
}

std::wostream& operator<< (std::wostream& out, itype& obj)
{
    obj.print(out);
    return out;
}
