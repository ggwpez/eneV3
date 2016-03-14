#include "token.hpp"

const schar* tok_strings[] = { L"<ident>", L"<string>", L"<number>", L"<operator>", L":", L"::", L",", L"°", L"=", L"(", L")", L"[", L"]", L"{", L"}", L".", L";", L"t_while", L"t_for", L"t_if", L"t_asm", L"t_namespace", L"t_else", L"t_break", L"t_return", L"t_struct", L"t_EOS" };
const schar* op_strings[] = {L"+", L"-", L"*", L"/", L"~", L"?", L"<", L">", L"!", L"^", L"$"};

schar* tok::to_str()
{
    if (this->type == tok_type::IDENT || this->type == tok_type::STRING)
        return this->string;
    else if (this->type == tok_type::OP)
        return op_strings[(int)this->oper];
    else
        return tok_strings[(int)this->type];
}

tok::tok()
{
    this->type = tok_type::size;
    this->string = NULL;

    this->pos_en_file = this->pos_en_line = this->pos_en_line_char = this->pos_st_file = this->pos_st_line = this->pos_st_line_char = NULL;
}

void tok::set_pos_st(char* pos_file, int pos_line, int pos_line_char)
{
    this->pos_st_file = pos_file;
    this->pos_st_line = pos_line;
    this->pos_st_line_char = pos_line_char;
}

void tok::set_pos_en(char* pos_file, int pos_line, int pos_line_char)
{
    this->pos_en_file = pos_file;
    this->pos_en_line = pos_line;
    this->pos_en_line_char = pos_line_char;
}


tok::tok(tok_type t)
{
    this->type = t;
};

tok::tok(tok_type t, op oper)
{
    this->type = t;
    this->oper = oper;
};

tok::tok(tok_type t, schar *string)
{
    this->type = t;
    this->string = string;
};

tok::tok(tok_type t, uint32_t number)
{
    this->type = t;
    this->number = number;
};

tok::~tok()
{
    if ((this->type == tok_type::IDENT || this->type == tok_type::STRING) && this->string)
    {
        free(this->string);

        this->string = NULL;
    }
}
