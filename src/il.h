#ifndef IL_H
#define IL_H

#include <sstream>

#include "target.h"
#include "uast.h"
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
    ~il();

    void generate();

private:
    ProgramNode* input;
    std::wostringstream* ss;       //code header, all what should stand at the beginning of code
    std::wostringstream* ss_code,* ss_codeh,* ss_data,* ss_bss;

    void generate_ProgramNode_term(tast* code);
    void generate(tast* code);
    void generate(ProgramNode* code);
    void generate(BlockNode* code);
    void generate(ExpressionNode* code);
    void generate(ExpressionTermNode* code);
    void generate(AssignNode* code);
    void generate(NumNode* code);
    void generate(IdentNode* code);
    void generate(BoolNode* code);
    void generate(PushNode* code);
    void generate(PopNode* code);
    void generate(ASMNode* code);
    void generate(StringNode* code);
    void generate_op_add(OperatorNode* code);
    void generate_op_sub(OperatorNode* code);
    void generate_op_mul(OperatorNode* code);
    void generate_op_div(OperatorNode* code);
    void generate_op_drf(OperatorNode* code);
    void generate_op_equ(OperatorNode* code);
    void generate_op_sml(OperatorNode* code);
    void generate_op_grt(OperatorNode* code);
    void generate_op_neq(OperatorNode* code);
    void generate_op_not(OperatorNode* code);
    void generate_op_pop(OperatorNode* code);
    void generate_op_cpy(OperatorNode* code);
    void generate(OperatorNode* code);
    void generate(ReturnNode* code);
    void generate(BreakNode* code);
    void generate(GoOnNode* code);
    void generate(TypeNode* code);
    void generate_global(VariableNode* code);
    void generate_local(VariableNode* code);
    void generate(ArgNode* code);
    void generate(ListNode* code);
    void generate(ListArgNode* code);
    void generate(FunctionHeaderNode* code);
    void generate(FunctionExternNode* code);
    void generate(FunctionNode* code);
    void generate(FunctionCallNode* code);
    void generate(IfNode* code);
    void generate(WhileNode* code);

};

#endif // IL_H
