#ifndef IL_NASM_H
#define IL_NASM_H

#include "il.h"

class il_nasm : public il
{
public:
    il_nasm(ProgramNode* code, std::wostringstream *ss);
    ~il_nasm();

    void generate() override;

private:
    void generate_ProgramNode_term(tast* code) override;
    void generate(tast* code) override;
    void generate(ProgramNode* code) override;
    void generate(BlockNode* code) override;
    void generate(ExpressionNode* code) override;
    void generate(ExpressionTermNode* code) override;
    void generate(AssignNode* code) override;
    void generate(NumNode* code) override;
    void generate(IdentNode* code) override;
    void generate(BoolNode* code) override;
    void generate(PushNode* code) override;
    void generate(PopNode* code) override;
    void generate(ASMNode* code) override;
    void generate(StringNode* code) override;
    void generate_op_add(OperatorNode* code) override;
    void generate_op_sub(OperatorNode* code) override;
    void generate_op_mul(OperatorNode* code) override;
    void generate_op_div(OperatorNode* code) override;
    void generate_op_drf(OperatorNode* code) override;
    void generate_op_equ(OperatorNode* code) override;
    void generate_op_sml(OperatorNode* code) override;
    void generate_op_grt(OperatorNode* code) override;
    void generate_op_neq(OperatorNode* code) override;
    void generate_op_not(OperatorNode* code) override;
    void generate_op_pop(OperatorNode* code) override;
    void generate_op_cpy(OperatorNode* code) override;
    void generate(OperatorNode* code) override;
    void generate(ReturnNode* code) override;
    void generate(BreakNode* code) override;
    void generate(GoOnNode* code) override;
    void generate(TypeNode* code) override;
    void generate_global(VariableNode* code);
    void generate_local(VariableNode* code) override;
    void generate(ArgNode* code) override;
    void generate(ListNode* code) override;
    void generate(ListArgNode* code) override;
    void generate(FunctionHeaderNode* code) override;
    void generate(FunctionExternNode* code) override;
    void generate(FunctionNode* code) override;
    void generate(FunctionCallNode* code) override;
    void generate(IfNode* code) override;
    void generate(WhileNode* code) override;

    void generate_sf_enter(int size);
    void generate_sf_leave(int size);
    void generate_ssp_init() override;
    void generate_ssp_check() override;
};

#endif // IL_H
