#include "itype.h"

itype::itype(int size) : size(size)
{

}

itype::~itype()
{
}

std::wostream& operator<< (std::wostream& out, itype& obj)
{
    obj.print(out);
    return out;
}
