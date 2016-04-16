#ifndef CMP_ALLOC_H
#define CMP_ALLOC_H

#include "lexing/lexer.hpp"
#include "parsing/parser.h"
#include "il/il_nasm.h"
#include "il/il_gas.h"
#include "scope/scoper.h"

class praep;
struct cmp_alloc
{
    cmp_alloc();
    ~cmp_alloc();

    lexer* lex;
    praep* prae;
    std::vector<tok*>* lexer_toks;
    std::vector<tok*>* praep_toks;
    parser* par;
    uast* un_ast;
    scoper* scr;
    ProgramNode* t_ast;
    il* il_gen;
    std::vector<opcode*>* il_ops;
    gen* asm_gen;
};

#endif // CMP_ALLOC_H
