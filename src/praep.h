#ifndef PRAEP_H
#define PRAEP_H

#include <vector>
#include <unordered_map>

#include "lexing/token.hpp"
#include "string_def.h"
#include "target.h"

class praep
{
public:
    praep(std::vector<tok*>* toks);

    std::vector<tok*>* process();

private:
    void parse_statement(int s, int& l);
    void parse_def(int s, int& l);
    void parse_use(int s, int& l);
    void parse_pragma(int s, int& l);

    std::vector<tok*> input;
    std::vector<tok*>* output;

    std::unordered_map<schar*, tok*>* defines;
};

#endif // PRAEP_H
