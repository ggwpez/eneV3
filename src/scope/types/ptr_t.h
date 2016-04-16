#ifndef PTR_T_H
#define PTR_T_H

#include "itype.h"

class ptr_t : public itype
{
public:
    ptr_t();
    ptr_t(itype* to);
    ~ptr_t();

    void print(std::wostream& out) override;
    bool operator==(const itype& other) const override;

    itype* to;
};

#endif // PTR_T_H
