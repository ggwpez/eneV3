#include "cmp_alloc.h"

cmp_alloc::cmp_alloc()
{

}

cmp_alloc::~cmp_alloc()
{
    delete asm_gen;

    for (opcode* op : *il_ops)
        delete op;
    delete il_ops;

    delete il_gen;
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
