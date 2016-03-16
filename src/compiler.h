#ifndef COMPILER_H
#define COMPILER_H

#include "lexer.hpp"
#include "parser.h"
#include "il.h"
#include "scope/scoper.h"

#include <string.h>

class compiler
{
public:
    compiler(size_t bits);

    void compile(char* file_name, char* output_file_name);
private:
    std::wostringstream* output;

    void post_process(char* output_file_name);
    void write_wstr(char* file_name);
    void write_str (char* file_name);
    void load_template();
    void write_w_template(std::wofstream f);
    void write_template(std::ofstream f);
};

#endif // COMPILER_H
