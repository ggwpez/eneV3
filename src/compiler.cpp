#include <fstream>
#include <iostream>
#include <string>

#define SKIP_AS 1
#define SKIP_LD 1
#define SKIP_IO 1

#include "compiler.h"
#include "io.h"
#include "warnings.h"
#include "errors.hpp"
#include "target.h"

compiler::compiler(cmp_args& args)
{
    target = targets[(args.bits >> 5) + ((int)args.assembler *3)];
    this->args = &args;
}

void compiler::compile()
{
    std::vector<std::string>* i_files = &this->args->inputs;
    std::vector<std::string>  obj_files;
    std::string const ex_asm(target->assembler == as::NASM ? ".nasm" : ".s");
    std::string const ex_obj(".obj");

    //Compile ene
    for (std::string const& file : *i_files)
    {
        std::string output = file +ex_asm;

        compile_file(file, output);
    }

    //Assemble nasm
    for (std::string const& file : *i_files)
    {
        std::string input  = file +ex_asm;
        std::string output = file +ex_obj;

        post_as(input, output);
        obj_files.push_back(output);
    }

    //Link objects
    post_ld(obj_files, this->args->output);
}

void compiler::compile_file(std::string const& file_name, std::string& output_file_name)
{
    std::wostringstream out;
    if (!args->no_warn)
        war_init();

    lexer lex = lexer(file_name.c_str());
    std::vector<tok*>* toks = lex.lex();

    parser par = parser(*toks);
    uast* un_ast = par.parse();

    scope* sc = new scope();
    scoper* scr = new scoper(un_ast, sc);
    ProgramNode* ast = scr->convert();

    this->load_template(out);
    il* gen;
    if (target->assembler == as::NASM)
        gen = new il_nasm(ast, &out);
    else
        gen = new il_gas(ast, &out);
    gen->generate();

    if (!args->no_warn)
        war_dump(std::wcout);

    delete gen;
    delete ast;
    delete scr;
    delete sc;
    for (tok* t : *toks)
        delete t;
    delete toks;

    write_wstr(out, output_file_name);
}

void compiler::post_as(std::string& i_file, std::string& o_file)
{
    if (SKIP_AS) return;
    std::stringstream assembler;
    if (target->assembler == as::NASM)
        assembler << "nasm -f elf";
    else
        assembler << "gcc -c -m";

    assembler << __BITS__ << " " << i_file << " -o " << o_file;

    if (system(assembler.str().c_str()))
        ERR(err_t::POST_PROCESSING_AS_FAILED);
}

void compiler::post_ld(std::vector<std::string>& i_files, std::string& o_file)
{
    if (SKIP_LD) return;
    std::stringstream gcc;
    gcc  << "gcc -m" << __BITS__ << " ";

    for (size_t i = 0; i < i_files.size(); i++)
        gcc << ' ' << i_files[i];

    gcc << " -o " << o_file;

    if (system(gcc.str().c_str()))
        ERR(err_t::POST_PROCESSING_LD_FAILED);
}

void compiler::write_wstr(std::wostringstream& ss, std::string& file_name)
{
    if (SKIP_IO) return;
    std::wofstream f;
    f.open(file_name);
    f << ss.str();

    f.close();
}

void compiler::write_str(std::wostringstream& ss, std::string& file_name)
{
    if (SKIP_IO) return;
    std::string str = std::string(ss.str().begin(), ss.str().end());
    std::ofstream f;
    f.open(file_name);
    f << str;

    f.close();
}

void compiler::load_template(std::wostringstream& ss)
{
    if (SKIP_IO) return;

    io helper;
    std::string ex = std::string(target->assembler == as::NASM ? ".nasm" : ".s");
    std::string tem_path = std::string("../src/templates/template_") + std::to_string(target->bits) +ex;

    /*ss << L"# generated by the ene compiler version 3.0 " << std::endl <<
                     << L" build on " << __DATE__ << L' ' << __TIME__ << std::endl << std::endl <<
                     << L" ### TEMPLATE BEGIN ###" << std::endl;*/

    helper.read_file(tem_path.c_str(), ss);
    //ss << L"# ### TEMPLATE   END ###" << std::endl << std::endl << std::endl;
}

cmp_args::cmp_args(size_t bits, std::vector<std::string> inputs, std::string output, as assembler, bool no_warn, bool only_compile)
    : bits(bits), inputs(inputs), output(output), assembler(assembler), no_warn(no_warn), only_compile(only_compile)
{
    if (bits == -1)
        ERR(err_t::IO_CMD_ARG_NO_BITS);
    if (!inputs.size())
        ERR(err_t::IO_CMD_ARG_NO_INPUT);
    if (!output.length())
        ERR(err_t::IO_CMD_ARG_NO_OUTPUT);
    if (assembler == as::size)
        ERR(err_t::IO_CMD_ARG_NO_AS);
}
