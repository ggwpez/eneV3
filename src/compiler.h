#ifndef COMPILER_H
#define COMPILER_H

#include "lexing/lexer.hpp"
#include "parsing/parser.h"
#include "il/il_nasm.h"
#include "il/il_gas.h"
#include "scope/scoper.h"
#include "io.h"
#include "errors/warnings.h"
#include "errors/errors.hpp"
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

class praep;
struct comp_alloc
{
    comp_alloc();
    ~comp_alloc();

    lexer* lex;
    praep* prae;
    std::vector<tok*>* lexer_toks;
    std::vector<tok*>* praep_toks;
    parser* par;
    uast* un_ast;
    scoper* scr;
    ProgramNode* t_ast;
    il* gen;
};

class compiler
{
public:
    compiler(cmp_args& args);
    ~compiler();

    int compile();
    void compile_file(scope *sc, std::string const& file_name, std::string& output_file_name, std::vector<std::string> *included_asm);
    std::string working_dir;

private:
    cmp_args* args;
    std::vector<comp_alloc*>* allocs;

    void post_as(std::string& i_file, std::string& o_file);
    void post_ld(std::vector<std::string>& i_file, std::string& o_file);
    void write_wstr(std::wostringstream &ss, std::string& file_name);
    void write_str (std::wostringstream& ss, std::string& file_name);
    void load_template(std::wostringstream& ss);
    void write_w_template(std::wostringstream& str, std::wofstream f);
    void write_template(std::wostringstream& str, std::ofstream f);
};

#endif // COMPILER_H
