#include "scoper.h"
#include "errors.hpp"

#include "ptr_t.h"

scoper::scoper(uast* input, scope* mng)
{
    this->input = input;
    this->mng = mng;
}

tast* scoper::convert(ProgramUNode* code)
{
    tast_arr* ncode = new tast_arr();

    for (size_t i = 0; i < code->code->size(); i++)
        ncode->push_back(convert(code->code->at(i)));

    return new ProgramNode(ncode);
};

tast* scoper::convert(BlockUNode* code)
{
    mng->enter();
    tast_arr* ncode = new tast_arr();
    int frame_s = 0;

    for (size_t i = 0; i < code->code->size(); i++)
    {
        tast* ne = convert(code->code->at(i));
        VariableNode* v = dynamic_cast<VariableNode*>(ne);

        if (v)
        {
            v->ebp_off = frame_s +4;
            frame_s += v->type->t->size;
        }

        ncode->push_back(ne);
    }

    mng->leave();
    return new BlockNode(ncode, frame_s);
};

tast* scoper::convert(ExpressionUNode* code)
{
    tast* exp = convert(code->exp);

    return new ExpressionNode(exp);
};

tast* scoper::convert(ExpressionTermUNode* code)
{
    tast_arr* nexps = new tast_arr();

    for (size_t i = 0; i < code->exps->size(); i++)
        nexps->push_back(convert(code->exps->at(i)));

    return new ExpressionTermNode(nexps);
};

tast* scoper::convert(AssignUNode* code)
{
    ExpressionTermNode* nterm = convert(code->term);

    return new AssignNode(nterm);
};

tast* scoper::convert(PushUNode* code)
{
    tast* v = convert(code->v);

    return new PushNode(v);
};

tast* scoper::convert(ReturnUNode* code)
{
    ExpressionTermNode* e = convert(code->val);

    return new ReturnNode(e);
};

tast* scoper::convert(TypeUNode* code)
{
    itype* ret = mng->get_type(code->parts->at(0));   //base type

    for (size_t i = 1; i < code->parts->size(); i++)
    {
        if (!wcscmp(L"°", code->parts->at(i)->str))
            ret = new ptr_t(ret);
        else
            ERR(err_t::IL_TYPE_UNKNOWN, ret);
    }

    return new TypeNode(ret);
};

tast* scoper::convert(VariableUNode* code)
{
    TypeNode* t = convert(code->type_name);

    if (!t)
        ERR(err_t::IL_TYPE_UNKNOWN, t);

    return new VariableNode(t, new IdentNode(code->var_name));
};

tast* scoper::convert(ArgUNode* code)
{
    TypeNode* t = convert(code->type);

    return new ArgNode(new IdentNode(code->name), t);
}

tast* scoper::convert(ListUNode* code)
{
    tast_arr* nel = new tast_arr();

    for (uast* it : *code->items)
        nel->push_back(convert(it));

    return new ListNode(nel);
};

tast* scoper::convert(ListArgUNode* code)
{
    std::vector<ArgNode*>* nan = new std::vector<ArgNode*>();

    for (ArgUNode* arg : *code->items)
        nan->push_back(convert(arg));

    return new ListArgNode(nan);
};

tast* scoper::convert(FunctionHeaderUNode* code)
{
    ListArgNode* args = convert(code->args);
    TypeNode* t = convert(code->type);
    IdentNode* i = new IdentNode(code->name);

    int args_s = 0;
    for (ArgNode* a : *args->items)
        args_s += a->type->t->size;

    return new FunctionHeaderNode(t, i, args, args_s);
};

tast* scoper::convert(FunctionUNode* code)
{
    FunctionHeaderNode* h = convert(code->head);
    BlockNode* b = convert(code->code);

    return new FunctionNode(h, b);
};

tast* scoper::convert(FunctionCallUNode* code)
{
    ListNode* args = convert(code->args);
    IdentNode* t = new IdentNode(code->target);

    return new FunctionCallNode(t, args);
};

tast* scoper::convert(IfUNode* code)
{
    ExpressionTermNode* cond = convert(code->cond);
    BlockNode* bt = convert(code->true_block),
             * bf = convert(code->false_block);

    return new IfNode(cond, bt, bf);
};

tast* scoper::convert(WhileUNode* code)
{
    ExpressionTermNode* cond = convert(code->cond);
    BlockNode* b = convert(code->block);

    return new WhileNode(cond, b);
};

tast* scoper::convert(IdentNode* code)
{
    return new IdentNode(code);
};

tast* scoper::convert(NumNode* code)
{
    return new NumNode(code);
}

tast* scoper::convert(StringNode* code)
{
    return new StringNode(code);
};

tast* scoper::convert(ASMNode* code)
{
    return new ASMNode(code);
};

tast* scoper::convert(BoolNode* code)
{
    return new BoolNode(code);
};

tast* scoper::convert(OperatorNode* code)
{
    return new OperatorNode(code);
};

tast* scoper::convert(BreakNode* code)
{
    return new BreakNode(code);
};

tast* scoper::convert(GoOnNode* code)
{
    return new GoOnNode(code);
};

tast* scoper::convert(PopNode* code)
{
    return new PopNode(code);
};

ProgramNode* scoper::convert()
{
    tast* ret = convert(input);

    delete input;
    return ret;
}

tast* scoper::convert(uast* code)
{
    if (dynamic_cast<ProgramUNode*>(code))
        convert(dynamic_cast<ProgramUNode*>(code));
    else if(dynamic_cast<BlockUNode*>(code))
        convert(dynamic_cast<BlockUNode*>(code));
    else if(dynamic_cast<ExpressionUNode*>(code))
        convert(dynamic_cast<ExpressionUNode*>(code));
    else if(dynamic_cast<ExpressionTermUNode*>(code))
        convert(dynamic_cast<ExpressionTermUNode*>(code));
    else if(dynamic_cast<AssignUNode*>(code))
        convert(dynamic_cast<AssignUNode*>(code));
    else if(dynamic_cast<PushUNode*>(code))
        convert(dynamic_cast<PushUNode*>(code));
    else if(dynamic_cast<ReturnUNode*>(code))
        convert(dynamic_cast<ReturnUNode*>(code));
    else if(dynamic_cast<TypeUNode*>(code))
        convert(dynamic_cast<TypeUNode*>(code));
    else if(dynamic_cast<VariableUNode*>(code))
        convert(dynamic_cast<VariableUNode*>(code));
    else if(dynamic_cast<ArgUNode*>(code))
        convert(dynamic_cast<ArgUNode*>(code));
    else if(dynamic_cast<ListUNode*>(code))
        convert(dynamic_cast<ListUNode*>(code));
    else if(dynamic_cast<ListArgUNode*>(code))
        convert(dynamic_cast<ListArgUNode*>(code));
    else if(dynamic_cast<FunctionHeaderUNode*>(code))
        convert(dynamic_cast<FunctionHeaderUNode*>(code));
    else if(dynamic_cast<FunctionUNode*>(code))
        convert(dynamic_cast<FunctionUNode*>(code));
    else if(dynamic_cast<FunctionCallUNode*>(code))
        convert(dynamic_cast<FunctionCallUNode*>(code));
    else if(dynamic_cast<IfUNode*>(code))
        convert(dynamic_cast<IfUNode*>(code));
    else if(dynamic_cast<WhileUNode*>(code))
        convert(dynamic_cast<WhileUNode*>(code));
    else if(dynamic_cast<IdentNode*>(code))
        convert(dynamic_cast<IdentNode*>(code));
    else if(dynamic_cast<StringNode*>(code))
        convert(dynamic_cast<StringNode*>(code));
    else if(dynamic_cast<BoolNode*>(code))
        convert(dynamic_cast<BoolNode*>(code));
    else if(dynamic_cast<ASMNode*>(code))
        convert(dynamic_cast<ASMNode*>(code));
    else if(dynamic_cast<OperatorNode*>(code))
        convert(dynamic_cast<OperatorNode*>(code));
    else if(dynamic_cast<NumNode*>(code))
        convert(dynamic_cast<NumNode*>(code));
    else if(dynamic_cast<PopNode*>(code))
        convert(dynamic_cast<PopNode*>(code));
    else if(dynamic_cast<BreakNode*>(code))
        convert(dynamic_cast<BreakNode*>(code));
    else if(dynamic_cast<GoOnNode*>(code))
        convert(dynamic_cast<GoOnNode*>(code));
    else if (dynamic_cast<tast*>(code))                  //is there no need for typing, like in NumNode?
        return dynamic_cast<tast*>(code);
    else
        ERR(err_t::GEN_SCR);
}
