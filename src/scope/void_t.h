#ifndef VOID_T_H
#define VOID_T_H

#include "itype.h"

class void_t : public itype
{
public:
    void_t();
    ~void_t();

    void print(std::wostream& out) override;
    bool operator==(const itype& other) const override;
};

#endif // VOID_T_H
