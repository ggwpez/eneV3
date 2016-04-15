#ifndef GEN_H
#define GEN_H

#include <vector>
#include <sstream>

#include "opcode.h"

class gen
{
public:
    gen(std::vector<opcode*>* ops, std::wostringstream& out);
    ~gen();

    void generate();

    std::wostringstream& out;
    std::wostringstream ss_code, ss_codeh, ss_data, ss_bss;

    std::vector<opcode*>* ops;
};

#endif // GEN_H
