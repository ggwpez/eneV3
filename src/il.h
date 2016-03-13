#ifndef IL_H
#define IL_H

#include <sstream>

#include "uast.h"
#include "string_def.h"
#include "scope/scope.hpp"

class il
{
public:
    il(ProgramNode* code, std::wostringstream *ss);
    ~il();

    void generate();

private:
    ProgramNode* input;
    std::wostringstream* ss;
    std::wostringstream* ss_code,* ss_data,* ss_bss;

    void push(schar* t);

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
    void generate(FunctionNode* code);
    void generate(FunctionCallNode* code);
    void generate(IfNode* code);
    void generate(WhileNode* code);

};

#endif // IL_H
