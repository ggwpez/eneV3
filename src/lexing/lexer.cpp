#include "lexer.hpp"
#include "errors/errors.hpp"
#include "io.h"

#include <string.h>
#include <ctype.h>
#include <wchar.h>

#define TAB_WIDTH 4

lexer::lexer(name_mng* names, char* file_name)
{
    io helper;
    this->input = helper.read_file(file_name);
    length = wcslen(input);
    this->file_name = file_name;
    this->names = names;
};

lexer::~lexer()
{
    delete[] input;
    length = 0;
}

tok* find_token(schar c)
{
    for (int i = 0; i < (int)op::size; i++)
        if (op_strings[(int)i][0] == c)
            return new tok((tok_type)i);

    return NULL;
}

std::vector<tok*>* lexer::lex()
{
    std::vector<tok*>* toks = new std::vector<tok*>();
    toks->reserve(length ? length /4 : 0);
    int pos = 0, l = 0, line = 1, char_c = 1;

    while ((pos += l) < length)
    {
        l = 0;
        schar c = input[pos];

        op mb_op = oper(c);
        tok* push = NULL;

        if (mb_op != op::size)
            push = new tok(tok_type::OP, mb_op), l++;
        else if (push = find_token(c))
            l = 1;
        else if (c == L'(')
            push = new tok(tok_type::LBRK), l++;
        else if (c == L')')
            push = new tok(tok_type::RBRK), l++;
        else if (c == L'[')
            push = new tok(tok_type::LEBRK), l++;
        else if (c == L']')
            push = new tok(tok_type::REBRK), l++;
        else if (c == L'{')
            push = new tok(tok_type::LCBRK), l++;
        else if (c == L'}')
            push = new tok(tok_type::RCBRK), l++;
        else if (c == L',')
            push = new tok(tok_type::COMMA), l++;
        else if (c == L'=')
            push = new tok(tok_type::ASSIGN), l++;
        else if (c == L'°')
            push = new tok(tok_type::CIRCLE), l++;
        else if (c == L'.')
            push = new tok(tok_type::DOT), l++;
        else if (c == L';')
            push = new tok(tok_type::SEMI), l++;
        else if (c == L'#')
            push = new tok(tok_type::PRAEP), l++;
        else if (c == L':')
            push = ddot(pos, l);
        else if (c == '"')
            push = string(pos, l);
        else if (iswalpha(c) || c == L'_')
            push = ident(pos, l);
        else if (iswdigit(c))
            push = number(pos, l);
        else if (c == L'\n')
        {
            line++, char_c = 0;
            push = new tok(tok_type::NEW_LINE), l = 1;
        }
        else if (iswspace(c))
        {

            if (c == L'\t')
                char_c += TAB_WIDTH;

            l = 1; char_c++;
            continue;
        }
        char_c += l;

        if (!push || !l)
            ERR(err_t::LEX_WRONG, c);
        else
        {
            push->set_pos_st(line, char_c-l);
            push->set_pos_en(line, char_c);
            push->pos_file = this->file_name;
            toks->push_back(push);
        }
    }

    toks->push_back(new tok(tok_type::EOS));
    return toks;
};

op lexer::oper(schar c)
{
    for (int i = 0; i < (int)op::size; i++)
        if (op_strings[i][0] == c)
            return (op)i;

    return op::size;
}

tok* lexer::number(int s, int& l)
{
    schar* buf;
    int number;

    l = 1;
    while ((s + l) < length && isdigit(input[s +l]))
        l++;

    buf = malloc((l +1) *sizeof(schar));
    wcpncpy(buf, input +s, l);
    buf[l] = L'\0';
    number = wcstol(buf, nullptr, 10);

    free(buf);

    return new tok(tok_type::NUM, number);
};

tok* parse_ident(schar* ident)
{
    for (size_t i = (size_t)tok_type::WHILE; i < (int)tok_type::EOS; i++)
        if (!wcscmp(tok_strings[i], ident))
            return new tok((tok_type)i);

    return new tok(tok_type::IDENT, ident);
}

tok* lexer::ident(int s, int& l)
{
    while ((s +l) < length && (isalnum(input[s +l]) || input[s +l] == '_'))
        l++;

    schar* new_ident = this->names->get_mem(input +s, l);

    return parse_ident(new_ident);
};

tok* lexer::string(int s, int& l)
{
    schar* string;
    l++;                            //skip first "

    while ((s+l) < length && input[s +l] != L'"')
        l++;

    string = names->get_mem(input +s +1, l++ -1);

    return new tok(tok_type::STRING, string);
};

tok* lexer::ddot(int s, int& l)
{
    l = 1;
    if (s == length -1)
        return new tok(tok_type::DDOT);

    if (input[s +1] == ':')
    {
        l = 2;
        return new tok(tok_type::DDDOT);
    }

    return new tok(tok_type::DDOT);
};
