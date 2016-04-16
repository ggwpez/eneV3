#include "scoper.h"
#include "errors/errors.hpp"
#include "errors/warnings.h"
#include "target.h"

#include "types/ptr_t.h"
#include "types/fptr_t.h"
#include "types/void_t.h"

#define last_type (!last_types->size() ? nullptr : last_types->top())
#define clear_types while(last_types->size()) {last_types->pop();}

scoper::scoper(uast* input, scope* mng)
{
    this->input = input;
    this->mng = mng;

    this->int_type = mng->get_type(&IdentNode(target->int_t));
    this->string_type = mng->get_type(&IdentNode(target->string_t));

    this->last_types = new std::stack<itype*>();
    own_types = new std::vector<itype*>();
}

scoper::~scoper()
{
    for (itype* t : *this->own_types)
        delete t;

    delete this->own_types;
    delete this->last_types;
}

tast* scoper::convert(ProgramUNode* code)
{
    tast_arr* ncode = new tast_arr();
    for (sc_fun* f : *mng->gl_funs)
    {
        ncode->push_back(new FunctionExternNode(new IdentNode(f->fun->head->name)));
    }

    for (size_t i = 0; i < code->code->size(); i++)
    {
        ncode->push_back(convert(code->code->at(i)));

        if (VariableNode* v = dynamic_cast<VariableNode*>(ncode->back()))
            mng->add_var(v);
    }

    for (sc_head* h : *mng->gl_heads)
    {
        ncode->push_back(new FunctionExternNode(new IdentNode(h->head->name)));
    }

    ProgramNode* ret = new ProgramNode(ncode);
    ret->set_pos(code);
    return ret;
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
            frame_s -= __BYTES__;
            v->ebp_off = frame_s;
            mng->add_var(v);
            delete v;
        }
        else
            ncode->push_back(ne);
    }

    mng->leave();
    BlockNode* ret = new BlockNode(ncode, frame_s < 0 ? -frame_s : frame_s);
    ret->set_pos(code);
    return ret;
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

    itype* src = last_type;
    if (last_types->size())
        last_types->pop();
    else
        WAR(war_t::ASSIGN_NO_TYPE, code);
    itype* dest = last_type;
    if (last_types->size())
        last_types->pop();
    else
        WAR(war_t::ASSIGN_NO_TYPE, code);

    AssignNode* ret = new AssignNode(nterm, dest, src);



    return ret;
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
    tast* ret;
    if (dynamic_cast<AtomTypeUNode*>(code))
        ret = convert(dynamic_cast<AtomTypeUNode*>(code));
    else if (dynamic_cast<FptrTypeUNode*>(code))
        ret = convert(dynamic_cast<FptrTypeUNode*>(code));
    else
        ERR(err_t::GEN_SCR);

    if (last_types->size())     //perform the cast
        last_types->pop();
    last_types->push(dynamic_cast<TypeNode*>(ret)->t);
    return ret;
};

tast* scoper::convert(AtomTypeUNode* code)
{
    itype* ret = mng->get_type(code->parts->at(0));   //base type
    TypeNode* rret = new TypeNode(ret);
    rret->set_pos(code);

    for (size_t i = 1; i < code->parts->size(); i++)
    {
        if (!wcscmp(L"°", code->parts->at(i)->str))
            ret = new ptr_t(ret);
        else
            ERR(err_t::IL_TYPE_UNKNOWN, rret);
    }

    if (code->parts->size() > 1)
        this->own_types->push_back(ret);                    //TODO ### optimize this, it always creates new pointer types, instead of reusing existing ones

    rret->t = ret;
    return rret;
}

tast* scoper::convert(FptrTypeUNode* code)
{
    ListTypeNode* args = convert(code->args);
    TypeNode* ret_t = convert(code->ret);

    std::vector<itype*>* types = new std::vector<itype*>();
    for (TypeNode* t : *args->items)
        types->push_back(t->t);

    itype* t = new fptr_t(types, ret_t->t);
    TypeNode* ret = new TypeNode(t);
    delete args;
    delete ret_t;

    return ret;
}

tast* scoper::convert(VariableUNode* code)
{
    TypeNode* t = convert(code->type_name);

    if (!t)
        ERR(err_t::IL_TYPE_UNKNOWN, t);

    return new VariableNode(t, new IdentNode(code->var_name));
};

ArgNode* scoper::convert(ArgUNode* code)
{
    TypeNode* t = convert(code->type);

    return new ArgNode(new IdentNode(code->name), t);
}

tast* scoper::convert(ListUNode* code)
{
    tast_arr* nel = new tast_arr();

    for (uast* it : *code->items)
        nel->push_back(convert(it));

    ListNode* ret = new ListNode(nel);
    ret->set_pos(code);
    return ret;
};

tast* scoper::convert(ListArgUNode* code)
{
    std::vector<ArgNode*>* nan = new std::vector<ArgNode*>();

    for (ArgUNode* arg : *code->items)
        nan->push_back(convert(arg));

    ListArgNode* ret = new ListArgNode(nan);
    ret->set_pos(code);
    return ret;
};

tast* scoper::convert(FunctionHeaderUNode* code)
{
    ListArgNode* args = convert(code->args);
    TypeNode* t = convert(code->type);
    IdentNode* i = new IdentNode(code->name);

    int args_s = 0;
    for (ArgNode* a : *args->items)
        args_s += a->type->t->size;

    FunctionHeaderNode* h = new FunctionHeaderNode(t, i, args, args_s, code->mods);
    mng->add_fun_head(h);
    return h;
};

tast* scoper::convert(FunctionUNode* code)
{
    FunctionHeaderNode* h = convert(code->head);

    mng->enter();
    int ebp_off = __BYTES__;
    std::vector<VariableNode*> tmp_args;

    for (ArgNode* arg : *h->args->items)
    {
        ebp_off += __BYTES__;                   //only can push multiple of sizeof(void*)

        VariableNode* v = new VariableNode(new TypeNode(arg->type->t), new IdentNode(arg->name), ebp_off);
        tmp_args.push_back(v);
        mng->add_var(v);
    }

    while (last_types->size())
        last_types->pop();

    BlockNode* b = convert(code->code);
    for (VariableNode* v : tmp_args)                //mng->leave() dosent delete VariableNodes
        delete v;

    FunctionNode* f = new FunctionNode(h, b);

    while (last_types->size())                         //pop return values
        last_types->pop();

    mng->leave();
    mng->rm_head(h);                                //headers signal extern functions, since it has a body now, rm it from the extern list
    mng->add_fun(f);
    return f;
};

tast* scoper::convert(FunctionCallUNode* code)
{
    ListNode* args = convert(code->args);
    IdentNode* t = new IdentNode(code->target);
    int arg_c = -1, args_s = 0;
    itype* ret_type = nullptr;

    if (mng->is_fun_reg(t))
    {
        FunctionNode* f = mng->get_fun(t);
        ret_type = f->head->type->t;
        arg_c = f->head->args->items->size();
        for (ArgNode* arg : *f->head->args->items)
            args_s += arg->type->t->size;
    }
    else if (mng->is_fun_head_reg(t))
    {
        FunctionHeaderNode* n = mng->get_head(t);
        ret_type = n->type->t;
        arg_c = n->args->items->size();
        for (ArgNode* arg : *n->args->items)
            args_s += arg->type->t->size;
        //delete n;
        //WAR(war_t::CALLING_UMIMPL_FUNC, code);
    }
    else
    {
        ERR(err_t::SC_FUN_NAME_UNKOWN, t);
    }

    FunctionCallNode* ret;
    if (arg_c !=  args->items->size())
    {
        ret = new FunctionCallNode(t, new TypeNode(ret_type), args, 0);
        WAR(war_t::ARG_COUNT_WRONG, ret);
    }
    else
        ret = new FunctionCallNode(t, new TypeNode(ret_type), args, args_s);

    this->last_types->push(ret_type);
    return ret;
};

AnomymousCallNode* scoper::convert(AnomymousCallUNode* code)
{
    ListNode* args = convert(code->args);

    return new AnomymousCallNode(args);
}

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

tast* scoper::convert(ForUNode* code)
{
    ExpressionTermNode* init = convert(code->init),
                      * cond = convert(code->cond),
                      *  inc = convert(code-> inc);

    BlockNode* block = convert(code->block);

    return new ForNode(init, cond, inc, block);
};

tast* scoper::convert(IdentNode* code)
{
    tast* to_add = NULL;

    if (mng->is_type_reg(code))
    {
        this->last_types->push(mng->get_type(code));
        return new TypeNode(last_type);     //its a cast
    }
    else if (mng->is_var_reg(code))
    {   //its a variable, lets check if in stackframe or not
        VariableNode* v = mng->get_var(code);
        this->last_types->push(v->type->t);
        to_add = v;
    }
    /*else if (mng->is_fun_reg(code))
    {   //anonymous call
        to_add = new IdentNode(code);
        //TODO ### add return type for anonymous calls
    }
    //TODO ### add function heads, to allow extern anonymous calls*/
    else
        ERR(err_t::SC_VAR_NAME_UNKOWN, code);

    return new PushNode(to_add);
};

#include <assert.h>
tast* scoper::convert(NumNode* code)
{
    last_types->push(int_type);
    assert(this->last_types->size());
    return new NumNode(code);
}

tast* scoper::convert(StringNode* code)
{
    last_types->push(string_type);
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

tast* scoper::convert(OperatorUNode* code)
{
    itype* target_t = last_type;
    OperatorNode* ret = new OperatorNode(code->oper, target_t);
    ret->set_pos(code);

    switch (ret->oper)
    {
        case op::ADD: case op::SUB: case op::MUL: case op::DIV: case op::EQU: case op::AND: case op::OR: case op::SML: case op::GRT:
            if (last_types->size() < 2)
            {
                WAR(war_t::OP_INSUFF_OPS, ret);
                while (last_types->size())
                    last_types->pop();
            }
            else
                last_types->pop();
            break;
        case op::NOT:
            if (!last_types->size())
                WAR(war_t::OP_INSUFF_OPS, ret);
            else
                last_types->pop();
            break;
        case op::DRF:
            if (!last_types->size())
                WAR(war_t::READING_UNINIT_MEM, ret);
            else if (ptr_t* v = dynamic_cast<ptr_t*>(last_type))
            {
                last_types->pop();
                last_types->push(v->to);                            //this isnt a use after free, right?
                target_t = last_type;
            }
            else
                WAR(war_t::READING_NON_PTR_TYPE, ret, last_type);
            break;
        case op::POP:
            if (!last_types->size())
                WAR(war_t::OP_INSUFF_OPS, ret);
            else
                last_types->pop();
            break;
        case op::CPY:
            if (!last_types->size())
                WAR(war_t::OP_INSUFF_OPS, ret);
            else
                last_types->push(last_type);
            break;
        default:
            ERR(err_t::GEN_SCR);
            break;
    }

    ret->operand_type = target_t;
    return ret;
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
    else if (dynamic_cast<AnomymousCallUNode*>(code))
        convert(dynamic_cast<AnomymousCallUNode*>(code));
    else if(dynamic_cast<IfUNode*>(code))
        convert(dynamic_cast<IfUNode*>(code));
    else if(dynamic_cast<WhileUNode*>(code))
        convert(dynamic_cast<WhileUNode*>(code));
    else if (dynamic_cast<ForUNode*>(code))
        convert(dynamic_cast<ForUNode*>(code));
    else if(dynamic_cast<IdentNode*>(code))
        convert(dynamic_cast<IdentNode*>(code));
    else if(dynamic_cast<StringNode*>(code))
        convert(dynamic_cast<StringNode*>(code));
    else if(dynamic_cast<BoolNode*>(code))
        convert(dynamic_cast<BoolNode*>(code));
    else if(dynamic_cast<ASMNode*>(code))
        convert(dynamic_cast<ASMNode*>(code));
    else if(dynamic_cast<OperatorUNode*>(code))
        convert(dynamic_cast<OperatorUNode*>(code));
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
