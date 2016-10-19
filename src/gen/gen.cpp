#include "gen.h"

gen::gen(cmp_args* args, std::vector<opcode*>* ops, std::wostringstream& out)
    : out(out)
{
    stack = std::vector<opcode*>();

    this->ops = ops;
    this->args = args;

    this->actual_stream = nullptr;
    this->ss_code;
    this->ss_codeh;
    this->ss_data;
    this->ss_bss;
}

gen::~gen()
{

}
void gen::output_opcode(opcode* op)
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
        *stream << L"push " << op->stream.str() << std::endl;
    else if (op->type == op_t::POP )
        *stream << L"pop  " << op->stream.str() << std::endl;
    else if (op->type == op_t::PUSH_KILL)
        *stream << L"add esp, " << op->stream.str() << std::endl;
}

void gen::clear_stack()
{
    while (stack.size())
    {
        output_opcode(stack.front());
        stack.erase(stack.begin());
    }
}

void gen::tick_automata(opcode* op)
{
    if (!op)
        clear_stack();
    else if (op->type == op_t::EML)
    {
        clear_stack();
        output_opcode(op);
    }
    else if (op->type == op_t::EM)
    {
        clear_stack();
        output_opcode(op);
    }
    else if (op->type == op_t::PUSH)
        stack.push_back(op);
    else if (op->type == op_t::POP )
    {
        if (stack.size() && stack.back()->type == op_t::PUSH)
        {
            opcode* push = stack.back();
            stack.pop_back();

            clear_stack();
            if (op->stream.str() != push->stream.str())
                ss_code << L"mov " << op->stream.str() << L", " << push->stream.str() << std::endl;
        }
        else
            output_opcode(op);
    }
    else if (op->type == op_t::PUSH_KILL)
    {
        if (stack.size() && stack.back()->type == op_t::PUSH)
            stack.pop_back();
        else
            output_opcode(op);
    }
}
void gen::generate()
{
    for (opcode* op : *ops)
    {
        if (this->args->optimisation)
            tick_automata(op);
        else
            output_opcode(op);
    }

    if (this->args->optimisation)
    {
        actual_stream = &ss_code;
        tick_automata(nullptr);
    }

    this->out << ss_codeh.str().c_str() << std::endl <<
                 ss_code .str().c_str() << std::endl <<
                 ss_data .str().c_str() << std::endl <<
                 ss_bss  .str().c_str();

}
