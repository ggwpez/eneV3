#ifndef ITYPE_H
#define ITYPE_H

#include <iostream>

class itype
{
public:
    itype(int size);

    virtual ~itype();
    virtual void print(std::wostream& out);
    friend std::wostream& operator<< (std::wostream& out, const itype& obj);

    int size;
};

#endif // ITYPE_H
