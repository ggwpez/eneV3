#ifndef IL_H
#define IL_H

#include <sstream>
#include <stack>
#include <unordered_map>

#include "target.h"
#include "ast/uast.h"
#include "string_def.h"
#include "scope/scope.hpp"
#include "gen/gen.h"

#define POP(s)          __create_op(op_t::POP , em_stream_t::CODE, s)
#define PUSH(s)         __create_op(op_t::PUSH, em_stream_t::CODE, s)
#define em(s)           emCODE(s)
#define eml(s)          emlCODE(s)
#define emCODE(s)       __create_op(op_t::EM,  em_stream_t::CODE , s)
#define emlCODE(s)      __create_op(op_t::EML, em_stream_t::CODE , s)
#define emCODEH(s)      __create_op(op_t::EM , em_stream_t::CODEH, s)
#define emlCODEH(s)     __create_op(op_t::EML, em_stream_t::CODEH, s)
#define emBSS(s)        __create_op(op_t::EM , em_stream_t::BSS  , s)
#define emlBSS(s)       __create_op(op_t::EML, em_stream_t::BSS  , s)
#define emDATA(s)       __create_op(op_t::EM , em_stream_t::DATA , s)
#define emlDATA(s)      __create_op(op_t::EML, em_stream_t::DATA , s)

#define __create_op(t, st, msg) do { opcode* tmp = new opcode(t, st); tmp->stream << msg; this->out->push_back(tmp); } while (0)

class il
{
public:
    il(ProgramNode* code);
    virtual ~il();

    std::vector<opcode*>* generate();

protected:
    ProgramNode* input;
    std::vector<opcode*>* out;

    virtual void generate_ProgramNode_term(tast* code) = 0;
    virtual void generate(tast* code) = 0;
    virtual void generate(ProgramNode* code) = 0;
    virtual void generate(BlockNode* code) = 0;
    virtual void generate(ExpressionNode* code) = 0;
    virtual void generate(ExpressionTermNode* code) = 0;
    virtual void generate(AssignNode* code) = 0;
    virtual void generate(NumNode* code) = 0;
    virtual void generate(IdentNode* code) = 0;
    virtual void generate(BoolNode* code) = 0;
    virtual void generate(PushNode* code) = 0;
    virtual void generate(PopNode* code) = 0;
    virtual void generate(ASMNode* code) = 0;
    virtual void generate(StringNode* code) = 0;
    virtual void generate_op_add(OperatorNode* code) = 0;
    virtual void generate_op_sub(OperatorNode* code) = 0;
    virtual void generate_op_mul(OperatorNode* code) = 0;
    virtual void generate_op_div(OperatorNode* code) = 0;
    virtual void generate_op_drf(OperatorNode* code) = 0;
    virtual void generate_op_equ(OperatorNode* code) = 0;
    virtual void generate_op_sml(OperatorNode* code) = 0;
    virtual void generate_op_grt(OperatorNode* code) = 0;
    virtual void generate_op_neq(OperatorNode* code) = 0;
    virtual void generate_op_not(OperatorNode* code) = 0;
    virtual void generate_op_pop(OperatorNode* code) = 0;
    virtual void generate_op_cpy(OperatorNode* code) = 0;
    virtual void generate_op_and(OperatorNode* code) = 0;
    virtual void generate_op_or (OperatorNode* code) = 0;
    virtual void generate(ReturnNode* code) = 0;
    virtual void generate(BreakNode* code) = 0;
    virtual void generate(GoOnNode* code) = 0;
    virtual void generate(TypeNode* code) = 0;
    virtual void generate_local(VariableNode* code) = 0;
    virtual void generate(ArgNode* code) = 0;
    virtual void generate(ListNode* code) = 0;
    virtual void generate(ListArgNode* code) = 0;
    virtual void generate(FunctionHeaderNode* code) = 0;
    virtual void generate(FunctionExternNode* code) = 0;
    virtual void generate(FunctionNode* code) = 0;
    virtual void generate(FunctionCallNode* code) = 0;
    virtual void generate(AnomymousCallNode* code) = 0;
    virtual void generate(IfNode* code) = 0;
    virtual void generate(WhileNode* code) = 0;
    virtual void generate(ForNode* code) = 0;

    void generate(OperatorNode* code);
    void generate_global(VariableNode* code);

    void generate_sf_enter(int size);
    void generate_sf_leave(int size);

    virtual void initalize_streams() = 0;
    void generate_output_init();
    void generate_output_end();
    std::wstring* generate_string_name(schar* content, bool& already_registered);
    virtual schar const* get_cc() = 0;                         //return the string needed for commenting out a line, aka // ; #

    int str_c, sml_c, grt_c, brk_c, ssp_c, anym_c, if_c, while_c;
    std::stack<schar*> funtion_returns;
    std::unordered_map<schar*, std::wstring*> registered_strings;

    // Stack logik
    void acum_push(schar const* str);
    void acum_push(int a);
    void acum_pop (schar const* str);
    std::stack<schar*> acum_stack;

    // constants
    schar const* const __str_start  = L"__start_";
    schar const* const __str_code   = L"__code_";
    schar const* const __str_check  = L"__ckeck_";
    schar const* const __str_end    = L"__end_";

    schar const* const __str_if     = L"__if_";
    schar const* const __str_else   = L"__else_";
    schar const* const __str_while  = L"__while_";
    schar const* const __str_for    = L"__for_";
    schar const* const __str_break  = L"__break_";
};

#endif // IL_H
