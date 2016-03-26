#include "praep.h"
#include "errors/errors.hpp"

#define tassert(tt, to) {if (to->type != tt) ERR(err_t::PAR_WRONG_BUT, (int)tt, to); }

praep::praep(std::vector<tok*>* toks)
{
    this->input = *toks;
    this->output = new std::vector<tok*>();
    this->defines = new std::unordered_map<schar*, tok*>();
};

std::vector<tok*>* praep::process()
{
    //output->reserve((size_t)(this->input.size() *0.95f);        //probably about 5% praep code
    int l;

    for (int i = 0; i < input.size(); i++)
    {
        if (input[i]->type == tok_type::PRAEP)
        {
            l = 0;
            parse_statement(i, l);
            i += l -1;
        }
        else
            output->push_back(input[i]);
    }

    return output;
};

void praep::parse_statement(int s, int& l)
{
    tassert(tok_type::PRAEP, input[s]); l = 1;
    int t_l = 0;

    switch (input[s +l]->type)
    {
        case tok_type::DEF:
            parse_def(s +l, t_l);
            break;
        case tok_type::USE:
            parse_use(s +l, t_l);
            break;
        case tok_type::PRAGMA:
            parse_pragma(s +l, t_l);
            break;
        default:
            ERR(err_t::PRAEP_INVALID_TOK, input[s +l]);
            break;
    }

    l += t_l;
}

//#def ene cool
void praep::parse_def(int s, int& l)
{
    tassert(tok_type::DEF, input[s]); l = 1;
    tassert(tok_type::IDENT, input[s +l]);

    schar* name = input[s +l]->string;
    l++;

    if (input[s +l]->type == tok_type::IDENT ||
        input[s +l]->type == tok_type::NUM ||
        input[s +l]->type == tok_type::STRING)
        defines->operator [](name) = input[s +l];
    else
        ERR(err_t::PRAEP_INVALID_TOK, input[s +l]);

    l++;
}

void praep::parse_use(int s, int& l)
{

}

void praep::parse_pragma(int s, int& l)
{

}
