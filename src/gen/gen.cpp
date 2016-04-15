#include "gen.h"

gen::gen(std::vector<opcode*>* ops, std::wostringstream& out)
    : out(out)
{
    this->ops = ops;

    this->ss_code;
    this->ss_codeh;
    this->ss_data;
    this->ss_bss;
}

gen::~gen()
{

}

void gen::generate()
{
    for (opcode* op : *ops)
    {
        std::wostringstream* stream;

        if (op->stream_type == em_stream_t::CODE)
            stream = &ss_code;
        else if (op->stream_type == em_stream_t::CODEH)
            stream = &ss_codeh;
        else if (op->stream_type == em_stream_t::BSS)
            stream = &ss_bss;
        else if (op->stream_type == em_stream_t::DATA)
            stream = &ss_data;

        if (op->type == op_t::EML)
            *stream << op->stream.str() << std::endl;
        else if (op->type == op_t::EM)
            *stream << op->stream.str();
        else if (op->type == op_t::PUSH)
        {
            *stream << L"push " << op->stream.str() << std::endl;
        }
        else if (op->type == op_t::POP )
        {
            *stream << L"pop " << op->stream.str() << std::endl;
        }
    }

    this->out << ss_codeh.str().c_str() << std::endl <<
                 ss_code .str().c_str() << std::endl <<
                 ss_data .str().c_str() << std::endl <<
                 ss_bss  .str().c_str() << std::endl;

}
