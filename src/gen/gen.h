#ifndef GEN_H
#define GEN_H

#include <vector>

#include "op.h"

class gen
{
public:
    gen(std::vector<op*>* ops, std::wostringstream& out);
    ~gen();

    void generate();

private:
    std::wostringstream* out;
    std::wostringstream* ss_code,* ss_codeh,* ss_data,* ss_bss;

    std::vector<op*>* ops;
};

#endif // GEN_H
