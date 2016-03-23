#ifndef IL_H
#define IL_H

#include <sstream>

#include "target.h"
#include "parsing/ast/uast.h"
#include "string_def.h"
#include "scope/scope.hpp"

#define pop(t)  (eml(L"pop  " << t))
#define push(t) (eml(L"push " << t))
#define em(s) emCODE(s)
#define eml(s) emlCODE(s)
#define emCODE(s) (*ss_code << s)
#define emlCODE(s) (*ss_code << s << std::endl)
#define emCODEH(s) (*ss_codeh << s)
#define emlCODEH(s) (*ss_codeh << s << std::endl)
#define emBSS(s) (*ss_bss << s)
#define emlBSS(s) (*ss_bss << s << std::endl)
#define emDATA(s) (*ss_data << s)
#define emlDATA(s) (*ss_data << s << std::endl)

class il
{
public:
    il(ProgramNode* code, std::wostringstream *ss);
    virtual ~il();

    virtual void generate() = 0;

protected:
    ProgramNode* input;
    std::wostringstream* ss;       //code header, all what should stand at the beginning of code
    std::wostringstream* ss_code,* ss_codeh,* ss_data,* ss_bss;

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
    void generate(OperatorNode* code);
    virtual void generate(ReturnNode* code) = 0;
    virtual void generate(BreakNode* code) = 0;
    virtual void generate(GoOnNode* code) = 0;
    virtual void generate(TypeNode* code) = 0;
    void generate_global(VariableNode* code);
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

    void generate_sf_enter(int size);
    void generate_sf_leave(int size);
    virtual void generate_ssp_init() = 0;
    virtual void generate_ssp_check() = 0;

    int str_c, sml_c, grt_c, if_c, while_c, ssp_c;
};

#endif // IL_H
