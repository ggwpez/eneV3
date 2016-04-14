#include "op.h"
#include "../name_mng.h"

op::op(op_t type, schar* content)
{
    this->type = type;
    this->content = name_mng::get_mem(content);
    this->stream = em_stream_t::NONE;
}

op::op(op_t type, em_stream_t stream, schar* content)
{
    this->type = type;
    this->content = name_mng::get_mem(content);
    this->stream = stream;
}
