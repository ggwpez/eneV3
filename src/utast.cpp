#include "utast.h"

uast::uast()
{

}

uast::~uast()
{

}

void uast::print(std::wostream& out) const
{
    out << L"<ast>";
}

tast::tast()
{

}

tast::~tast()
{

}

void tast::print(std::wostream& out) const
{

}

IdentNode::IdentNode(schar* str)
{
    this->str = str;
}

IdentNode::IdentNode(IdentNode* Node)
{
    this->str = Node->str;
}

void IdentNode::print(std::wostream& out) const
{
    out << str;
}

bool IdentNode::operator==(const IdentNode& other) const
{
    return !(wcscmp(this->str, other.str));     //### TODO: jsut compare pointers, and see if its the same
}

NumNode::NumNode(int num)
{
    this->num = num;
}

NumNode::NumNode(NumNode* n)
{
    this->num = n->num;
}

void NumNode::print(std::wostream& out) const
{
    out << num;
}

StringNode::StringNode(schar* str)
{
    this->str = str;
}

StringNode::StringNode(StringNode* n)
{
    this->str = n->str;
}

StringNode::~StringNode()
{

}

void StringNode::print(std::wostream& out) const
{

}

OperatorNode::OperatorNode(op oper)
{
    this->oper = oper;
}

OperatorNode::OperatorNode(OperatorNode* n)
{
    this->oper = n->oper;
}

void OperatorNode::print(std::wostream& out) const
{
    out << op_strings[(int)this->oper];
}

BoolNode::BoolNode(BoolNode* n)
{

}

void BoolNode::print(std::wostream& out) const
{

}

ASMNode::ASMNode(ASMNode* n)
{

}

void ASMNode::print(std::wostream& out) const
{

}

BreakNode::BreakNode(BreakNode* n)
{

}

void BreakNode::print(std::wostream& out) const
{

}

GoOnNode::GoOnNode(GoOnNode* n)
{

}

void GoOnNode::print(std::wostream& out) const
{

}

PopNode::PopNode()
{
    this->target = nullptr;
}

PopNode::PopNode(IdentNode* target)
{
    this->target = target;
}

PopNode::PopNode(PopNode* n)
{
    this->target = n->target;
}

void PopNode::print(std::wostream &out) const
{

}
