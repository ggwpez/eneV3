#ifndef VOID_T_H
#define VOID_T_H

#include "itype.h"

class void_t : public itype
{
public:
    void_t();

    void print(std::wostream& out);
};

#endif // VOID_T_H
