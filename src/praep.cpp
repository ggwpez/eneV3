#include "praep.h"
#include "io.h"
#include "errors/errors.hpp"

#define tassert(tt, to) {if (to->type != tt) ERR(err_t::PAR_WRONG_BUT, (int)tt, to); }

praep::praep(compiler* comp, std::vector<tok*>* toks, std::vector<std::string>* included_asm)
{
    this->input = *toks;
    this->output = new std::vector<tok*>();
    this->defines = std::unordered_map<schar*, tok*>();

    this->sc = sc;
    this->comp = comp;
    this->included_asm = included_asm;

    std::string tmp(toks->front()->pos_file);
    this->working_dir = io::get_dir(tmp);           //cpp wtf - instances work but constructor calls do not
};

praep::~praep()
{

};

std::vector<tok*>* praep::process()
{
    for (int i = 0; i < input.size(); i++)
    {
        if (input[i]->type == tok_type::PRAEP)
        {
            int l = 0;
            parse_statement(i, l);
            i += l -1;
        }
        else if (input[i]->type == tok_type::NEW_LINE)
            continue;
        else if (input[i]->type == tok_type::IDENT)
        {
            auto it = defines.find(input[i]->string);
            if (it != defines.end())                            //this ident is defined, aka #def ident lelle
                output->push_back(it->second);
            else
                output->push_back(input[i]);                    //not defined
        }
        else
            output->push_back(input[i]);                        //not even an ident
    }

    return output;
};

void praep::parse_statement(int s, int& l)
{
    tassert(tok_type::PRAEP, input[s]); l = 1;
    int t_l = 0;

    switch (input[s +l]->type)
    {
        case tok_type::PRAEP: case tok_type::OP:
            parse_comment(s +l, t_l);
            break;
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

void praep::parse_comment(int s, int& l)
{
    if (input[s]->type == tok_type::PRAEP)          //## comment until end of line
        while (input[++l +s]->type != tok_type::NEW_LINE);
    else if (input[s]->type == tok_type::OP)        //#+ comment until next +#
    {
        while (++l)
            if (input[s +l]->type == tok_type::OP && input[s +l +1]->type == tok_type::PRAEP)
            {
                l += 2; break;
            }
    }
    else
        ERR(err_t::GEN_PAR);                        //should never happen
}

//#def ene cool
void praep::parse_def(int s, int& l)
{
    tassert(tok_type::DEF,   input[s]); l = 1;
    tassert(tok_type::IDENT, input[s +l]);

    schar* name = input[s +l]->string;
    l++;

    if (input[s +l]->type == tok_type::IDENT ||
        input[s +l]->type == tok_type::NUM ||
        input[s +l]->type == tok_type::STRING)
    {
        defines[name] = input[s +l];
    }
    else
        ERR(err_t::PRAEP_INVALID_TOK, input[s +l]);

    l++;
}

//#use "assert.ene"
void praep::parse_use(int s, int& l)
{
    tassert(tok_type::USE,    input[s]); l = 1;
    tassert(tok_type::STRING, input[s +l]);

    char* raw = io::get_c(input[s +l]->string);
    std::string f_name;
    f_name.assign(raw);
    delete raw;
    f_name = this->working_dir +f_name;

    std::string fo_name = f_name +std::string(target->assembler == as::NASM ? EX_NASM : EX_GAS);
    l++;

    this->comp->compile_file(f_name, fo_name, included_asm);
    included_asm->push_back(f_name);
}

//#pragma { warnings=false, warning13=false}
void praep::parse_pragma(int s, int& l)
{
    tassert(tok_type::PRAGMA, input[s]); l = 1;

}
