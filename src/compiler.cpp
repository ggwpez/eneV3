#include "compiler.h"
#include "io.h"

#include <fstream>
#include <iostream>
#include <string>

compiler::compiler()
{
    this->output = new std::wostringstream();
}

void compiler::compile(char* file_name)
{
    std::wcout << "Lexing...";
    lexer lex = lexer(file_name);
    std::vector<tok*>* toks = lex.lex();

    std::wcout << "Done." << std::endl << "Parsing...";
    parser par = parser(*toks);
    uast* un_ast = par.parse();

    std::wcout << "Done." << std::endl << "Type generation...";
    scope* sc = new scope();
    scoper* scr = new scoper(un_ast, sc);
    ProgramNode* ast = scr->convert();

    std::wcout << "Done." << std::endl << "Code generation...";
    this->load_template();
    il* gen = new il(ast, this->output);
    gen->generate();

    std::wcout << "Done." << std::endl << "Cleanup...";
    delete gen;
    delete ast;
    delete scr;
    delete sc;
    delete toks;
    std::wcout << "Done." << std::endl;
}

void compiler::write_wstr(char* file_name)
{
    std::wofstream f;
    f.open(file_name);
    f << this->output->str();

    f.close();
}

void compiler::write_str(char* file_name)
{
    std::string str = std::string(this->output->str().begin(), this->output->str().end());
    std::ofstream f;
    f.open(file_name);
    f << str;

    f.close();
}

std::wstring compiler::get_wstr()
{
    return this->output->str();
}

std::string compiler::get_str()
{
    return std::string(this->output->str().begin(), this->output->str().end());
}

#define TEMPLATE_PATH "../src/template.nasm"
void compiler::load_template()
{
    io helper;

    *this->output << L"; generated by the ene compiler version 3.0 " << std::endl <<
                     L"; build on " << __DATE__ << L' ' << __TIME__ << std::endl << std::endl <<
                     L"; ### TEMPLATE BEGIN ###" << std::endl;
    helper.read_file(TEMPLATE_PATH, this->output);
    *this->output << L"; ### TEMPLATE   END ###" << std::endl << std::endl << std::endl;
}
