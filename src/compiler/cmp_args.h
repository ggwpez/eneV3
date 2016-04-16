#ifndef CMP_ARGS_H
#define CMP_ARGS_H

#include <vector>

#include "string_def.h"
#include "target.h"

struct cmp_args
{
    cmp_args();
    ~cmp_args();
    cmp_args(size_t bits, std::vector<std::string>* inputs, std::string* output, std::string* template_path,  as assembler, bool no_warn, bool pedantic_err, bool only_compile, int optimisation);

    std::vector<std::string>* inputs;
    std::string* output;
    std::string* template_path;
    size_t bits;
    as assembler;
    bool no_warn;
    bool only_compile;
    bool pedantic_err;
    int optimisation;
};

#endif // CMP_ARGS_H
