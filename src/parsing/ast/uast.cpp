#include "uast.h"

ProgramUNode::ProgramUNode(uast_arr* code) : ast(code->empty() ? nullptr : code->front(), code->empty() ? nullptr : code->back())
{
    this->code = code;
}

ProgramUNode::~ProgramUNode()
{
    for (uast* system32 : *this->code)
        delete system32;                //lel

    delete this->code;
}

void ProgramUNode::print(std::wostream& out) const
{
    for (uast* s : *code)
        s->print(out);
}

BlockUNode::BlockUNode(uast_arr* code) : ast(code->empty() ? nullptr : code->front(), code->empty() ? nullptr : code->back())
{
    this->code = code;
}

BlockUNode::~BlockUNode()
{
    for (uast* it : *this->code)
        delete it;

    delete this->code;
}

void BlockUNode::print(std::wostream& out) const
{
    for (uast* s : *code)
        s->print(out), out << std::endl;
}

ExpressionUNode::ExpressionUNode(uast *exp) : ast(exp)
{
    this->exp = exp;
}

void ExpressionUNode::print(std::wostream& out) const
{
    out << L"<ExpressionUNode <exp ";
    this->exp->print(out);
    out << L">>";
}

ExpressionTermUNode::ExpressionTermUNode(uast_arr* exps) : ast(exps->empty() ? nullptr : exps->front(), exps->empty() ? nullptr : exps->back())
{
    this->exps = exps;
}

void ExpressionTermUNode::print(std::wostream& out) const
{
    out << L"<ExpressionTermUNode <exps ";
    for (uast* it : *this->exps)
        it->print(out), out << (it == this->exps->back() ? L"" : L" ");     //formation ftw
    out << L">>";
}

ExpressionTermUNode::~ExpressionTermUNode()
{
    for (uast* it : *this->exps)
        delete it;

    delete this->exps;
}

PushUNode::PushUNode(uast* v) : ast(v, v)
{
    this->v = v;
}

void PushUNode::print(std::wostream& out) const
{
    out << "<push ";
    v->print(out);
    out << L'>';
}

ArgUNode::ArgUNode(IdentNode* name, TypeUNode* type) : ast(type, name)
{
    this->name = name;
    this->type = type;
}

ArgUNode::~ArgUNode()
{
    delete this->name;
    delete this->type;
}

void ArgUNode::print(std::wostream& out) const
{

}

TypeUNode::TypeUNode(std::vector<IdentNode*>* parts) : ast(parts->empty() ? nullptr : parts->front(), parts->empty() ? nullptr : parts->back())
{
    this->parts = parts;
}

TypeUNode::~TypeUNode()
{
    for (IdentNode* i : *this->parts)
        delete i;

    delete this->parts;
}

void TypeUNode::print(std::wostream& out) const
{
    for (IdentNode* str : *parts)
        out << str->str;
}

VariableUNode::VariableUNode(TypeUNode* type_name, IdentNode* var_name) : ast(type_name, var_name)
{
    this->type_name = type_name;
    this->var_name = var_name;
}

VariableUNode::~VariableUNode()
{
    delete this->type_name;
    delete this->var_name;
}

void VariableUNode::print(std::wostream& out) const
{
    out << L"<VariableUNode <type_name ";
    this->type_name->print(out);
    out << L"><var_name " << this->var_name->str << L">>>";
}

ListUNode::ListUNode(uast_arr* items) : ast(items->size() ? items->front() : nullptr, items->size() ? items->back() : nullptr)
{
    this->items = items;
}

ListUNode::~ListUNode()
{
    for (uast* item : *items)
        delete item;

    delete this->items;
}

void ListUNode::print(std::wostream& out) const
{
    out << L"<ListUNode <items ";
    for (uast* item : *items)
        item->print(out);

    out << L">>";
}

AssignUNode::AssignUNode(ExpressionTermUNode* term) : ast(term)
{
    this->term = term;
}

void AssignUNode::print(std::wostream& out) const
{
    out << L"<AssignUNode <term ";
    this->term->print(out);
    out << L">>";
}

FunctionHeaderUNode::FunctionHeaderUNode(TypeUNode* type, IdentNode* name, ListArgUNode* args) : ast(type, args->items->empty() ? dynamic_cast<ast*>(name) : dynamic_cast<ast*>(args))
{
    this->type = type;
    this->name = name;
    this->args = args;
}

FunctionHeaderUNode::~FunctionHeaderUNode()
{
    delete this->type;
    delete this->name;
    delete this->args;
}

void FunctionHeaderUNode::print(std::wostream& out) const
{
    out << L"<FunctionHeaderUNode <type ";
    this->type->print(out);
    out << L"><name ";
    this->name->print(out);
    out << L"><args ";
    this->args->print(out);
    out << L">>";
}

FunctionUNode::FunctionUNode(FunctionHeaderUNode* head, BlockUNode* code) : ast(head, code->code->empty() ? dynamic_cast<ast*>(head) : dynamic_cast<ast*>(code))
{
    this->head = head;
    this->code = code;
}

FunctionUNode::~FunctionUNode()
{
    delete this->head;
    delete this->code;
}

void FunctionUNode::print(std::wostream& out) const
{
    out << L"<FunctionUNode <head ";
    this->head->print(out);
    out << L"><code ";
    this->code->print(out);
    out << L">>";
}

ListArgUNode::ListArgUNode(std::vector<ArgUNode*>* items) : ast(items->empty() ? nullptr : items->front(), items->empty() ? nullptr : items->back())
{
    this->items = items;
}

ListArgUNode::~ListArgUNode()
{
    for (uast* item : *items)
        delete item;

    delete this->items;
}

void ListArgUNode::print(std::wostream &out) const
{
    out << L"<ListArgUNode <items ";
    for (uast* item : *items)
        item->print(out);
    out << L">>";
}

FunctionCallUNode::FunctionCallUNode(IdentNode* target, ListUNode* args) : ast(target, args)
{
    this->target = target;
    this->args = args;
}

void FunctionCallUNode::print(std::wostream& out) const
{
    out << L"<FunctionCallUNode <target ";
    this->target->print(out);
    out << L"><args ";
    this->args->print(out);
    out << L">>";
}

FunctionCallUNode::~FunctionCallUNode()
{
    delete this->target;
    delete this->args;
}

AnomymousCallUNode::AnomymousCallUNode(ListUNode *args)
{
    this->args = args;
}

void AnomymousCallUNode::print(std::wostream& out) const
{
    out << L"<AnomymousCallUNode <args ";
    this->args->print(out);
    out << L">";
}

AnomymousCallUNode::~AnomymousCallUNode()
{
    delete this->args;
}

ReturnUNode::ReturnUNode(ExpressionTermUNode* val) : ast(val)
{
    this->val = val;
}

void ReturnUNode::print(std::wostream& out) const
{
    out << L"<ReturnUNode <val ";
    this->val->print(out);
    out << L">>";
}

IfUNode::IfUNode(ExpressionTermUNode* cond, BlockUNode* true_block, BlockUNode* false_block) : ast(cond, false_block ? false_block : true_block)
{
    this->cond = cond;
    this->true_block = true_block;
    this->false_block = false_block;
}

IfUNode::~IfUNode()
{
    delete this->cond;
    delete this->true_block;
    delete this->false_block;
}

void IfUNode::print(std::wostream& out) const
{

}

WhileUNode::WhileUNode(ExpressionTermUNode* cond, BlockUNode* block) : ast(cond, block)
{
    this->cond = cond;
    this->block = block;
}

WhileUNode::~WhileUNode()
{
    delete this->cond;
    delete this->block;
}

void WhileUNode::print(std::wostream& out) const
{

}

void BinaryUNode::print(std::wostream& out) const
{

}

void UnaryUNode::print(std::wostream& out) const
{

}

OperatorUNode::OperatorUNode(op oper)
{
    this->oper = oper;
}

OperatorUNode::OperatorUNode(tok* code) : ast(code)
{
    this->oper = code->oper;
}

OperatorUNode::OperatorUNode(OperatorUNode *n) : ast(n)
{
    this->oper = n->oper;
}

void OperatorUNode::print(std::wostream& out) const
{
    out << op_strings[(int)this->oper];
}
