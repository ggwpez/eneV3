#ifndef OP_H
#define OP_H

#include "../string_def.h"

enum class op_t : unsigned char
{
    PUSH,
    POP,
    EM,
    EML,
};

enum class em_stream_t : unsigned char
{
    NONE = 0,
    CODE,
    CODEH,
    BSS,
    DATA
};

struct op
{
    op(op_t type, schar* content);
    op(op_t type, em_stream_t stream, schar* content);

    op_t type;
    schar* content;
    em_stream_t stream;     // not needed when when its a PUSH or POP
};

#endif // OP_H
