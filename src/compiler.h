#ifndef COMPILER_H
#define COMPILER_H

#include "lexer.hpp"
#include "parser.h"
#include "il.h"
#include "scope/scoper.h"
#include "target.h"

#include <string.h>

struct cmp_args
{
    cmp_args(size_t bits, std::vector<std::string> inputs, std::string output, as assembler);

    std::vector<std::string> inputs;
    std::string output;
    size_t bits;
    as assembler;
};

class compiler
{
public:
    compiler(cmp_args& args);

    void compile();

private:
    cmp_args* args;

    void compile_file(std::string const& file_name, std::string& output_file_name);
    void post_as(std::string& i_file, std::string& o_file);
    void post_ld(std::vector<std::string>& i_file, std::string& o_file);
    void write_wstr(std::wostringstream &ss, std::string& file_name);
    void write_str (std::wostringstream& ss, std::string& file_name);
    void load_template(std::wostringstream& ss);
    void write_w_template(std::wostringstream& str, std::wofstream f);
    void write_template(std::wostringstream& str, std::ofstream f);
};

#endif // COMPILER_H
