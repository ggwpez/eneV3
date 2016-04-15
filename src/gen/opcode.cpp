#include "opcode.h"
#include "../name_mng.h"

opcode::opcode(op_t type)
{
    this->type = type;
    this->stream_type = em_stream_t::CODE;
}

opcode::opcode(op_t type, em_stream_t stream_type)
{
    this->type = type;
    this->stream_type = stream_type;
}
