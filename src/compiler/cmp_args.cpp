#include "cmp_args.h"
#include "errors/errors.hpp"

cmp_args::cmp_args()
{

}

cmp_args::~cmp_args()
{
    delete this->inputs;
    delete this->output;
    delete this->template_path;
}

cmp_args::cmp_args(size_t bits, std::vector<std::string>* inputs, std::string* output, std::string* template_path, as assembler, bool no_warn, bool pedantic_err, bool only_compile, int optimisation)
    : bits(bits), inputs(inputs), output(output), template_path(template_path), assembler(assembler), no_warn(no_warn), pedantic_err(pedantic_err), only_compile(only_compile), optimisation(optimisation)
{
    if (bits == -1)
        ERR(err_t::IO_CMD_ARG_NO_BITS);
    if (!inputs || !inputs->size())
        ERR(err_t::IO_CMD_ARG_NO_INPUT);
    if (!output || !output->length())
        ERR(err_t::IO_CMD_ARG_NO_OUTPUT);
    if (assembler == as::size)
        ERR(err_t::IO_CMD_ARG_NO_AS);
}
