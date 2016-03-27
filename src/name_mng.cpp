#include <algorithm>

#include "name_mng.h"

name_mng::name_mng()
{
    names = std::vector<schar*>();
}

name_mng::~name_mng()
{
    for (schar* n : names)
        delete n;
}

schar* name_mng::get_name(schar* n, size_t l)
{
    for (schar* s : names)
    {
        if (!wcsncmp(s, n, l))
            return s;
    }

    size_t len = wcslen(n) +1;
    schar* nn = new schar[len];

    wcsncpy(nn, n, l);
    nn[l] = 0;
    names.push_back(nn);

    return nn;
}
