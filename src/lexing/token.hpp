#pragma once

#include <inttypes.h>
#include <iostream>

#include "string_def.h"

extern const schar* tok_strings[];
enum class tok_type
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
    BACKSLASH,
    WHILE,
    FOR,
    IF,
    ASM,
    NSPACE,
    ELSE,
    BREAK,
    RETURN,
    STRUCT,
    PRAEP,
    DEF,
    USE,
    PRAGMA,
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
    NOT = 8,    // !
    POP = 9,    // ^
    CPY = 10,   // $
    AND = 11,
    OR = 12,
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

    void print(std::wostream &out);
    void print_pos(std::wostream& out);
    void set_pos_st(int pos_line, int pos_line_char);
    void set_pos_en(int pos_line, int pos_line_char);
    tok_type type;

    char* pos_file, pos_st_line, pos_st_line_char,
          pos_en_line, pos_en_line_char;

    union
    {
        schar* string;
        op oper;
        int number;
    };
};
