#include "token.hpp"

const schar* tok_strings[] = { L"<ident>", L"<string>", L"<number>", L"<operator>", L":", L"::", L",", L"°", L"=", L"(", L")", L"[", L"]", L"{", L"}", L".", L";", L"\\", L"while", L"for", L"if", L"asm", L"namespace", L"else", L"break", L"goon", L"return", L"struct",
                    /*praep*/  L"#", L"def", L"use", L"pragma", L"<newline>",
                               L"EOS" };
const schar* op_strings[] = { L"+", L"-", L"*", L"/", L"~", L"?", L"<", L">", L"!", L"^", L"$", L"&", L"|" };

void tok::print(std::wostream& out)
{
    if (this->type == tok_type::IDENT || this->type == tok_type::STRING)
        out << this->string;
    else if (this->type == tok_type::OP)
        out << op_strings[(int)this->oper];
    else if (this->type == tok_type::NUM)
        out << this->number;
    else
        out << tok_strings[(int)this->type];
}

tok::tok()
{
    this->type = tok_type::size;
    this->string = NULL;


    this->pos_file = this->pos_en_line = this->pos_en_line_char = this->pos_st_line = this->pos_st_line_char = NULL;
}

void tok::print_pos(std::wostream& out)
{
    out << L" in file " << this->pos_file
        << L" line " << std::to_wstring(this->pos_st_line)
        << L" pos " << std::to_wstring(this->pos_st_line_char)
        << L" to line " << std::to_wstring(this->pos_en_line)
        << L" pos " << std::to_wstring(this->pos_en_line_char);
}

void tok::set_pos_st(int pos_line, int pos_line_char)
{
    this->pos_st_line = pos_line;
    this->pos_st_line_char = pos_line_char;
}

void tok::set_pos_en(int pos_line, int pos_line_char)
{
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

}
