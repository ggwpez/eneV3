#ifndef OP_H
#define OP_H

#include <sstream>
#include <ostream>

#include "../string_def.h"

enum class op_t : unsigned char
{
    PUSH,
    PUSH_KILL,
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

struct opcode
{
    opcode(op_t type);
    opcode(op_t type, em_stream_t stream_type);

    op_t type;
    em_stream_t stream_type;     // not needed when when its a PUSH or POP
    std::wostringstream stream;
};

#endif // OP_H
