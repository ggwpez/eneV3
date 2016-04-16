#ifndef ITYPE_H
#define ITYPE_H

#include <iostream>

class itype
{
public:
    itype(int size);

    virtual ~itype();
    virtual void print(std::wostream& out) = 0;
    friend std::wostream& operator<< (std::wostream& out, const itype& obj);
    virtual bool operator==(const itype& other) const = 0;

    int size;
};

#endif // ITYPE_H
