#ifndef ARRAY_T_H
#define ARRAY_T_H

#include "itype.h"

class array_t
{
public:
    array_t();

    void print(std::wostream& out);

    itype* of;
};

#endif // ARRAY_T_H
