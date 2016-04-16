#ifndef FPTR_T_H
#define FPTR_T_H

#include "itype.h"
#include <vector>

class fptr_t : public itype
{
public:
    fptr_t();
    fptr_t(std::vector<itype*>* args, itype* ret);
    ~fptr_t();

    void print(std::wostream& out) override;
    bool operator==(const itype& other) const override;

    std::vector<itype*>* args;
    itype* ret;
};

#endif // FPTR_T_H
