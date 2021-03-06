#include "ast/uast.h"
#include "errors/warnings.h"
#include "target.h"

ProgramNode::ProgramNode(tast_arr* code) : ast(code->empty() ? nullptr : code->front(), code->empty() ? nullptr : code->back())
{
    this->code = code;
}

ProgramNode::~ProgramNode()
{
    for (tast* system32 : *this->code)
        delete system32;                //lel

    delete this->code;
}

void ProgramNode::print(std::wostream& out) const
{
    for (tast* s : *code)
        s->print(out);
}

BlockNode::BlockNode(tast_arr* code, int stack_s) : ast(code->empty() ? nullptr : code->front(), code->empty() ? nullptr : code->back())
{
    this->code = code;
    this->stack_s = stack_s;
}

BlockNode::~BlockNode()
{
    for (tast* it : *this->code)
        delete it;

    delete this->code;
}

void BlockNode::print(std::wostream& out) const
{
    for (tast* s : *code)
        s->print(out), out << std::endl;
}

ExpressionNode::ExpressionNode(tast* exp) : ast(exp)
{
    this->exp = exp;
}

ExpressionNode::~ExpressionNode()
{
    delete this->exp;
}

void ExpressionNode::print(std::wostream& out) const
{
    //out << L"<ExpressionNode <exp ";
    this->exp->print(out);
    //out << L">>";
}

ExpressionTermNode::ExpressionTermNode(tast_arr* exps) : ast(exps->empty() ? nullptr : exps->front(), exps->empty() ? nullptr : exps->back())
{
    this->exps = exps;
}

void ExpressionTermNode::print(std::wostream& out) const
{
    //out << L"<FunctionNode <exps ";
    for (tast_arr::const_iterator it = exps->begin(); it != exps->end(); it++)
        (*it)->print(out), out << (it +1 == exps->end() ? L"" : L" ");
    out << L';';
    //out << L">>";
}

ExpressionTermNode::~ExpressionTermNode()
{
    for (tast* it : *this->exps)
        delete it;

    delete this->exps;
}

PushNode::PushNode(tast* v) : ast(v)
{
    this->v = v;
}

PushNode::~PushNode()
{
    delete this->v;
}

void PushNode::print(std::wostream& out) const
{
    //out << "<push ";
    v->print(out);
    //out << L'>';
}

ArgNode::ArgNode(IdentNode* name, TypeNode* type) : ast(type, name)
{
    this->name = name;
    this->type = type;
}

ArgNode::ArgNode(ArgNode* v)
{
    this->name = new IdentNode(v->name);
    this->type = new TypeNode(v->type->t);
}

ArgNode::~ArgNode()
{
    delete this->name;
    delete this->type;
}

void ArgNode::print(std::wostream& out) const
{

}

TypeNode::TypeNode(itype* t)
{
    this->t = t;
}

void TypeNode::print(std::wostream &out) const
{
    t->print(out);
}

/*AtomTypeNode::AtomTypeNode(itype* t)
{
    this->t = t;
}

AtomTypeNode::~AtomTypeNode()
{

}

void AtomTypeNode::print(std::wostream& out) const
{
    out << L"<AtomTypeNode <t ";
    t->print(out);
    out << L">>";
}

FptrTypeNode::FptrTypeNode(ListTypeNode* args, TypeNode* ret)
{
    this->args = args;
    this->ret = ret;
}

FptrTypeNode::~FptrTypeNode()
{
    delete this->args;
    delete this->ret;
}

void FptrTypeNode::print(std::wostream& out) const
{
    out << L"<FptrTypeNode <args ";
    args->print(out);
    out << L"><ret ";
    ret->print(out);
    out << L">>";
}*/

VariableNode::VariableNode(TypeNode* type, IdentNode* var_name) : ast(type, var_name)
{
    this->type = type;
    this->var_name = var_name;
    this->ebp_off = 0;
}

VariableNode::VariableNode(TypeNode* type, IdentNode* var_name, int ebp_off) : ast(type, var_name)
{
    this->type = type;
    this->var_name = var_name;
    this->ebp_off = ebp_off;
}

VariableNode::VariableNode(VariableNode* v)
{
    this->type = new TypeNode(v->type->t);
    this->var_name = new IdentNode(v->var_name);
    this->ebp_off = v->ebp_off;
}

VariableNode::~VariableNode()
{
    delete this->type;
    delete this->var_name;
}

void VariableNode::print(std::wostream& out) const
{
    out << this->var_name->str;
}

ListNode::ListNode(tast_arr* items) : ast(items->empty() ? nullptr : items->front(), items->empty() ? nullptr : items->back())
{
    this->items = items;
}

ListNode::~ListNode()
{
    for (tast* item : *items)
        delete item;

    delete this->items;
}

void ListNode::print(std::wostream& out) const
{
    for (tast_arr::const_iterator it = items->begin(); it != items->end(); it++)
        (*it)->print(out), out << (it +1 == items->end() ? L"" : L", ");
}

AssignNode::AssignNode(ExpressionTermNode* term, itype* dest, itype *src) : ast(term)
{
    this->term = term;
    this->dest = dest;
    this->src = src;
}

AssignNode::~AssignNode()
{
    delete this->term;
}

void AssignNode::print(std::wostream& out) const
{
    out << L" = ";
    this->term->print(out);
}

FunctionHeaderNode::FunctionHeaderNode(TypeNode* type, IdentNode* name, ListArgNode* args, int args_size, FMod mods) : ast(type, args)
{
    this->type = type;
    this->name = name;
    this->args = args;
    this->args_size = args_size;
    this->mods = mods;
}

FunctionHeaderNode::FunctionHeaderNode(FunctionHeaderNode* code)
{
    this->type = new TypeNode(code->type->t);
    this->name = new IdentNode(code->name);
    this->args = new ListArgNode(code->args);
    this->args_size = code->args_size;
    this->mods = code->mods;
}

FunctionHeaderNode::~FunctionHeaderNode()
{
    delete this->type;
    delete this->name;
    delete this->args;
}

void FunctionHeaderNode::print(std::wostream& out) const
{
    this->type->print(out);
    out << L' ';
    this->name->print(out);
    out << L" (";
    this->args->print(out);
    out << L')';
}

FunctionExternNode::FunctionExternNode(IdentNode* fname)
{
    this->fname = fname;
}

FunctionExternNode::~FunctionExternNode()
{
    delete this->fname;
}

FunctionNode::FunctionNode(FunctionHeaderNode* head, BlockNode* code) : ast(head, code)
{
    this->head = head;
    this->code = code;
}

FunctionNode::~FunctionNode()
{
    delete this->head;
    delete this->code;
}

void FunctionNode::print(std::wostream& out) const
{
    this->head->print(out);
    out << L' ';
    this->code->print(out);
}

ListArgNode::ListArgNode(std::vector<ArgNode*>* items) : ast(items->empty() ? nullptr : items->front(), items->empty() ? nullptr : items->back())
{
    this->items = items;
}

ListArgNode::ListArgNode(ListArgNode* v)
{
    this->items = new std::vector<ArgNode*>();

    for (ArgNode* n :* v->items)
        items->push_back(new ArgNode(n));
}

ListArgNode::~ListArgNode()
{
    for (tast* item : *items)
        delete item;

    delete this->items;
}

void ListArgNode::print(std::wostream &out) const
{
    out << L'(';
    for (std::vector<ArgNode*>::const_iterator it = items->begin(); it != items->end(); it++)
        (*it)->print(out), out << (it +1 == items->end() ? L"" : L", ");
    out << L')';
}

ListTypeNode::ListTypeNode(std::vector<TypeNode*>* items) : ast(items->empty() ? nullptr : items->front(), items->empty() ? nullptr : items->back())
{
    this->items = items;
}

ListTypeNode::~ListTypeNode()
{
    for (TypeNode* item : *items)
        delete item;

    delete this->items;
}

void ListTypeNode::print(std::wostream &out) const
{
    for (TypeNode* item : *items)
        item->print(out);
}

FunctionCallNode::FunctionCallNode(IdentNode* target, TypeNode *return_type, ListNode* args, int args_size) : ast(target, args)
{
    this->target = target;
    this->return_type = return_type;
    this->args = args;
    this->args_size = args_size;
}

void FunctionCallNode::print(std::wostream& out) const
{
    this->target->print(out);
    out << L'(';
    this->args->print(out);
    out << L')';
}

FunctionCallNode::~FunctionCallNode()
{
    delete this->target;
    delete this->return_type;
    delete this->args;
}

AnomymousCallNode::AnomymousCallNode(ListNode* args)
{
    this->args = args;
}

void AnomymousCallNode::print(std::wostream &out) const
{
    out << L"<AnomymousCallNode <args ";
    this->args->print(out);
    out << L">";
}

AnomymousCallNode::~AnomymousCallNode()
{
    delete this->args;
}

ReturnNode::ReturnNode(ExpressionTermNode* val) : ast(val)
{
    this->val = val;
}

ReturnNode::~ReturnNode()
{
    delete this->val;
}

void ReturnNode::print(std::wostream& out) const
{
    out << L"return ";
    this->val->print(out);
}

IfNode::IfNode(ExpressionTermNode* cond, BlockNode* true_block, BlockNode* false_block) : ast(cond, false_block ? false_block : true_block)
{
    this->cond = cond;
    this->true_block = true_block;
    this->false_block = false_block;
}

IfNode::~IfNode()
{
    delete this->cond;
    delete this->true_block;
    delete this->false_block;
}

void IfNode::print(std::wostream& out) const
{

}

WhileNode::WhileNode(ExpressionTermNode* cond, BlockNode* block) : ast(cond, block)
{
    this->cond = cond;
    this->block = block;
}

WhileNode::~WhileNode()
{
    delete this->cond;
    delete this->block;
}

void WhileNode::print(std::wostream& out) const
{

}

ForNode::ForNode(ExpressionTermNode* init, ExpressionTermNode* cond, ExpressionTermNode* inc, BlockNode* block)
{
    this->init = init;
    this->cond = cond;
    this->inc = inc;
    this->block = block;
}

ForNode::~ForNode()
{
    delete this->init;
    delete this->cond;
    delete this->inc;
    delete this->block;
}

void ForNode::print(std::wostream& out) const
{
    out << L"for (<init "; this->init->print(out); out << L">;<cond ";
    this->cond->print(out); out << L">;<inc ";
    this->inc->print(out); out << L">;)" << std::endl;

    out << L"{\n";
    this->block->print(out);
    out << L"}\n";
}

OperatorNode::OperatorNode(op oper)
{
    this->oper = oper;
}

OperatorNode::OperatorNode(op oper, itype* operand_type)
{
    this->oper = oper;
    this->operand_type = operand_type;
}

OperatorNode::OperatorNode(OperatorNode* n) : ast(n)
{
    this->oper = n->oper;
}

void OperatorNode::print(std::wostream& out) const
{
    out << op_strings[(int)this->oper];
}
