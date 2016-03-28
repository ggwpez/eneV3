#ifndef NAME_MNG_H
#define NAME_MNG_H

#include "target.h"
#include "string_def.h"
#include "lexing/token.hpp"

#include <vector>

class name_mng
{
public:
    name_mng();
    ~name_mng();

    schar* get_mem(schar* n, size_t l);

private:
    std::vector<schar*> names;
};

#endif // NAME_MNG_H
