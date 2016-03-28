#include <algorithm>

#include "name_mng.h"

name_mng::name_mng()
{
    names = std::vector<schar*>();
}

name_mng::~name_mng()
{
    for (schar* n : names)
        delete[] n;
}

schar* name_mng::get_mem(schar* n, size_t l)
{
    for (schar* s : names)
    {
        if ((wcslen(s) == l) && !wcsncmp(s, n, l))
            return s;
    }

    schar* nn = new schar[l +1];

    wcsncpy(nn, n, l);
    nn[l] = 0;
    names.push_back(nn);

    return nn;
}
