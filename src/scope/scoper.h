#ifndef SCOPER_H
#define SCOPER_H

#include "scope.hpp"

/**
 * @brief The scoper class
 * Gets an untyped AST and converts it to a
 * types AST, so the IL wont have to use the
 * scope anymore.
 */
class scoper
{
public:
    scoper(uast* input, scope* mng);
    ~scoper();
    ProgramNode* convert();

private:
    uast* input;
    scope* mng;
    itype* int_type;
    std::stack<itype*>* last_types;
    std::vector<itype*>* own_types;

    tast* scoper::convert(ProgramUNode* code);
    tast* scoper::convert(BlockUNode* code);
    tast* scoper::convert(ExpressionUNode* code);
    tast* scoper::convert(ExpressionTermUNode* code);
    tast* scoper::convert(AssignUNode* code);
    tast* scoper::convert(BinaryUNode* code);
    tast* scoper::convert(UnaryUNode* code);
    tast* scoper::convert(PushUNode* code);
    tast* scoper::convert(ReturnUNode* code);
    tast* scoper::convert(TypeUNode* code);
    tast* scoper::convert(AtomTypeUNode* code);
    tast* scoper::convert(FptrTypeUNode* code);
    tast* scoper::convert(VariableUNode* code);
    ArgNode *scoper::convert(ArgUNode *code);
    tast* scoper::convert(ListUNode* code);
    tast* scoper::convert(ListArgUNode* code);
    tast* scoper::convert(FunctionHeaderUNode* code);
    tast* scoper::convert(FunctionUNode* code);
    tast* scoper::convert(FunctionCallUNode* code);
    AnomymousCallNode* scoper::convert(AnomymousCallUNode *code);
    tast* scoper::convert(IfUNode* code);
    tast* scoper::convert(WhileUNode* code);
    tast* scoper::convert(IdentNode* code);
    tast* scoper::convert(NumNode* code);
    tast* scoper::convert(StringNode* code);
    tast* scoper::convert(ASMNode* code);
    tast* scoper::convert(BoolNode* code);
    tast* scoper::convert(OperatorUNode* code);
    tast* scoper::convert(BreakNode* code);
    tast* scoper::convert(GoOnNode* code);
    tast* scoper::convert(PopNode* code);
    tast* scoper::convert(uast* code);
};

#endif // SCOPER_H
