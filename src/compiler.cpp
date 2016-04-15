#include <fstream>
#include <iostream>
#include <string>

#include "praep.h"

#include "compiler.h"

std::string ex_asm;
std::string ex_obj;

compiler::compiler(cmp_args& args)
{
    target = targets[(args.bits >> 5) + ((int)args.assembler *3)];
    this->args = &args;
    this->allocs = new std::vector<comp_alloc*>();
    this->names = new name_mng();
    this->sc = new scope(names);
    this->build_dir = io::get_dir(*args.output);

    ex_asm = EX_ASM(target->assembler);
    ex_obj = std::string(_EX_OBJ);
}

compiler::~compiler()
{
    for (std::vector<comp_alloc*>::const_iterator it = allocs->begin(); it != allocs->end(); it++)
        delete *it;

    delete allocs;
    delete sc;
    delete names;
}

int compiler::compile()
{
    std::vector<std::string>& i_files = *this->args->inputs;
    std::vector<std::string>  obj_files;

    //Compile ene
    war_init();
    std::vector<std::string> included_asm = std::vector<std::string>();     //included files, already compiled, now in asm form
    for (std::string const& ifile : i_files)
    {
        std::string file = io::get_file_name(ifile);
        std::string output = this->build_dir +file +ex_asm;

        compile_file(ifile, output, &included_asm);
    }
    war_close();

    //Assemble nasm
    for (std::string f : included_asm)
        i_files.insert(i_files.begin(), f);

    for (std::string const& ifile : i_files)
    {
        std::string file = io::get_file_name(ifile);
        std::string input  = this->build_dir +file +ex_asm;
        std::string output = this->build_dir +file +ex_obj;

        post_as(input, output);
        obj_files.push_back(output);
    }

    //Link objects
    post_ld(obj_files, *this->args->output);
    return 0;
}

void compiler::compile_file(std::string const& file_name, std::string& output_file_name, std::vector<std::string>* included_asm)
{
    std::wostringstream out;
    if (!args->no_warn)
    {
        war_enter(file_name);
        war_as_error = args->pedantic_err;
    }

    comp_alloc* alloc = new comp_alloc();

    alloc->lex = new lexer(this->names, file_name.c_str());
    alloc->lexer_toks = alloc->lex->lex();

    alloc->prae = new praep(this, alloc->lexer_toks, included_asm);
    alloc->praep_toks = alloc->prae->process();

    alloc->par = new parser(alloc->praep_toks);
    alloc->un_ast = alloc->par->parse();

    alloc->scr = new scoper(alloc->un_ast, sc);
    alloc->t_ast = alloc->scr->convert();

    this->load_template(out);
    if (target->assembler == as::NASM)
        alloc->il_gen = new il_nasm(alloc->t_ast);
    else
        alloc->il_gen = new il_gas(alloc->t_ast);

    alloc->il_ops = alloc->il_gen->generate();

    if (!args->no_warn)
        war_dump(std::wcout);

    write_wstr(out, output_file_name);
    this->allocs->push_back(alloc);
}

void compiler::post_as(std::string& i_file, std::string& o_file)
{
    std::stringstream assembler;
    if (target->assembler == as::NASM)
        assembler << "nasm -f elf";
    else
        assembler << "as -c --";

    assembler << __BITS__ << " " << i_file << " -o " << o_file;

    if (system(assembler.str().c_str()))
        ERR(err_t::POST_PROCESSING_AS_FAILED);
}

void compiler::post_ld(std::vector<std::string>& i_files, std::string& o_file)
{
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
    std::wofstream f;
    f.open(file_name);
    f << ss.str();

    f.close();
}

void compiler::write_str(std::wostringstream& ss, std::string& file_name)
{
    std::string str = std::string(ss.str().begin(), ss.str().end());
    std::ofstream f;
    f.open(file_name);
    f << str;

    f.close();
}

void compiler::load_template(std::wostringstream& ss)
{
    std::string ex = EX_ASM(target->assembler);
    std::string tem_path = *this->args->template_path +std::string("/template_") +std::to_string(target->bits) +ex;

    io::read_file(tem_path.c_str(), ss);
}

cmp_args::cmp_args()
{

}

cmp_args::~cmp_args()
{
    delete this->inputs;
    delete this->output;
    delete this->template_path;
}

cmp_args::cmp_args(size_t bits, std::vector<std::string>* inputs, std::string* output, std::string* template_path, as assembler, bool no_warn, bool pedantic_err, bool only_compile)
    : bits(bits), inputs(inputs), output(output), template_path(template_path), assembler(assembler), no_warn(no_warn), pedantic_err(pedantic_err), only_compile(only_compile)
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

comp_alloc::comp_alloc()
{

}

comp_alloc::~comp_alloc()
{
    delete asm_gen;
    delete il_gen;
    delete il_ops;
    delete prae;
    delete scr;
    delete par;

    delete t_ast;
    delete un_ast;

    delete praep_toks;

    for (tok* t : *lexer_toks)
        delete t;

    delete lexer_toks;
    delete lex;
}
