#pragma once

#include <inttypes.h>
#include <iostream>

#include "string_def.h"

extern const schar* tok_strings[];
enum class tok_type : int
{
    IDENT,
    STRING,
    NUM,
    OP,
    DDOT,
    DDDOT,
    COMMA,
    CIRCLE,
    ASSIGN,
    LBRK,
    RBRK,
    LEBRK,
    REBRK,
    LCBRK,
    RCBRK,
    DOT,
    SEMI,
    WHILE,
    FOR,
    IF,
    ASM,
    NSPACE,
    ELSE,
    BREAK,
    RETURN,
    STRUCT,
    EOS,
    size
};

extern const schar* op_strings[];
enum class op : int
{
    ADD = 0,    // +
    SUB = 1,    // -
    MUL = 2,    // *
    DIV = 3,    // /
    DRF = 4,    // ~ pointer deref like * in c
    EQU = 5,    // ?
    SML = 6,    // <
    GRT = 7,    // >
    NEQ = 8,    // !
    POP = 9,    // ^
    CPY = 10,   // $
    size
};

class tok
{
public:

    tok();
    tok(const tok& obj);
    tok(tok_type t);
    tok(tok_type t, schar* string);
    tok(tok_type t, op oper);
    tok(tok_type t, uint32_t number);

    ~tok();

    schar* to_str();
    void set_pos_st(char* pos_file, int pos_line, int pos_line_char);
    void set_pos_en(char* pos_file, int pos_line, int pos_line_char);
    tok_type type;

    char* pos_st_file, pos_st_line, pos_st_line_char,
        * pos_en_file, pos_en_line, pos_en_line_char;

    union
    {
        schar* string;
        op oper;
        int number;
    };
};
