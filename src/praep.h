#ifndef PRAEP_H
#define PRAEP_H

#include <vector>
#include <unordered_map>

#include "lexing/token.hpp"
#include "string_def.h"
#include "target.h"
#include "scope/scoper.h"
#include "compiler.h"

class praep
{
public:
    praep(scope* sc, compiler* comp, std::vector<tok *> *toks, std::vector<std::string> *included_asm);

    std::vector<tok*>* process();

private:
    void parse_statement(int s, int& l);
    void parse_def(int s, int& l);
    void parse_use(int s, int& l);
    void parse_pragma(int s, int& l);

    std::vector<tok*> input;
    std::vector<tok*>* output;

    scope* sc;
    compiler* comp;
    std::vector<std::string>* included_asm;

    std::unordered_map<schar*, tok*>* defines;
};

#endif // PRAEP_H
