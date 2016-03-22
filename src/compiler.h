#ifndef COMPILER_H
#define COMPILER_H

#include "lexing/lexer.hpp"
#include "parsing/parser.h"
#include "il/il_nasm.h"
#include "il/il_gas.h"
#include "scope/scoper.h"
#include "target.h"

#include <string.h>

struct cmp_args
{
    cmp_args();
    cmp_args(size_t bits, std::vector<std::string> inputs, std::string output, std::string template_path,  as assembler, bool no_warn, bool pedantic_err, bool only_compile);

    std::vector<std::string> inputs;
    std::string output;
    std::string template_path;
    size_t bits;
    as assembler;
    bool no_warn;
    bool only_compile;
    bool pedantic_err;
};

class compiler
{
public:
    compiler(cmp_args& args);

    int compile();

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
