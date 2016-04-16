#ifndef STRUCT_T_H
#define STRUCT_T_H

#include <vector>

#include "itype.h"
#include "string_def.h"

class struct_t : itype
{
public:
    struct_t();

    void print(std::wostream& out) override;

    schar* name;
    std::vector<itype*>* member;
};

#endif // STRUCT_T_H
