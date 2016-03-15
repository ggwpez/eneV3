#include "lexer.hpp"
#include "errors.hpp"
#include "io.h"

#include <string.h>
#include <ctype.h>
#include <wchar.h>

lexer::lexer(char *file_name)
{
    io helper;
    this->input = helper.read_file(file_name);
    length = wcslen(input);
    this->file_name = file_name;
};

lexer::~lexer()
{
    free(input);
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
        else if (c == L':')
            push = ddot(pos, l);
        else if (c == '"')
            push = string(pos, l);
        else if (iswalpha(c) || c == L'_')
            push = ident(pos, l);
        else if (iswdigit(c))
            push = number(pos, l);
        else if (iswspace(c))
        {
            if (c == L'\n')
            {
                line++;
                char_c = 0;
            }
            l = 1; char_c++;
            continue;
        }
        char_c += l;

        if (!push || !l)
            ERR(err_t::LEX_WRONG, c);
        else
        {
            push->set_pos_st(file_name, line, char_c-l);
            push->set_pos_en(file_name, line, char_c);
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

    buf = malloc((l +1) * sizeof(schar));
    wcpncpy(buf, input +s, l);
    number = wcstol(buf, nullptr, 10);

    free(buf);

    return new tok(tok_type::NUM, number);
};

tok* parse_ident(schar* ident)
{
    if (!wcscmp(L"namespace", ident))
        return new tok(tok_type::NSPACE);
    else if (!wcscmp(L"asm", ident))
        return new tok(tok_type::ASM);
    else if (!wcscmp(L"if", ident))
        return new tok(tok_type::IF);
    else if (!wcscmp(L"else", ident))
        return new tok(tok_type::ELSE);
    else if (!wcscmp(L"while", ident))
        return new tok(tok_type::WHILE);
    else if (!wcscmp(L"for", ident))
        return new tok(tok_type::FOR);
    else if (!wcscmp(L"return", ident))
        return new tok(tok_type::RETURN);
    else if (!wcscmp(L"struct", ident))
        return new tok(tok_type::STRUCT);
    else if (!wcscmp(L"break", ident))
        return new tok(tok_type::BREAK);
    else
        return new tok(tok_type::IDENT, ident);
}

tok* lexer::ident(int s, int& l)
{
    schar* ident;
    while ((s +l) < length && (isalnum(input[s +l]) || input[s +l] == '_'))
        l++;

    ident = malloc((l +1) * sizeof(schar));
    wcpncpy(ident, input +s, l);
    ident[l] = L'\0';

    return parse_ident(ident);
};

tok* lexer::string(int s, int& l)
{
    schar* string;
    l++;                            //skip first "

    while ((s+l) < length && input[s +l] != L'"')
        l++;

    string = malloc(l * sizeof(schar) +2);
    wcpncpy(string, input +s +1, l);
    string[l++ -1] = L'\0';

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
