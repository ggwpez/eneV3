#ifndef GEN_H
#define GEN_H

#include <vector>
#include <stack>
#include <sstream>

#include "opcode.h"
#include "compiler/cmp_args.h"

class gen
{
public:
    gen(cmp_args* args, std::vector<opcode*>* ops, std::wostringstream& out);
    ~gen();

    void generate();
    void tick_automata(opcode* op);
    void clear_stack();
    void output_opcode(opcode *op);

    cmp_args* args;
    std::vector<opcode*>stack;
    std::wostringstream& out;
    std::wostringstream* actual_stream;
    std::wostringstream ss_code, ss_codeh, ss_data, ss_bss;

    std::vector<opcode*>* ops;
};

#endif // GEN_H
