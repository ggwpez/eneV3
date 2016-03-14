#include "utast.h"

ast::ast()
{
    this->pos_en_line = this->pos_en_line_char = this->pos_st_line = this->pos_st_line_char = -1;
}

ast::ast(ast* code)
{
    this->set_pos_start(code);
    this->set_pos_end(code);
}

ast::ast(tok* code)
{
    this->set_pos_start(code);
    this->set_pos_end(code);
}

ast::ast(ast* start, ast* end)
{
    this->set_pos_start(start);
    this->set_pos_end(end);
}

ast::ast(tok* start, tok* end)
{
    this->pos_st_line = start->pos_st_line;
    this->pos_st_line_char = start->pos_st_line_char;

    this->pos_en_line = end->pos_en_line;
    this->pos_en_line_char = end->pos_en_line_char;
}

ast::set_pos(ast* code)
{
    this->set_pos_end(code);
    this->set_pos_start(code);
}

ast::set_pos(tok* code)
{
    this->set_pos_end(code);
    this->set_pos_start(code);
}

ast::set_pos_end(ast* end)
{
    if (!end)
        return;
    this->pos_en_line = end->pos_en_line;
    this->pos_en_line_char = end->pos_en_line_char;
}

ast::set_pos_end(tok* end)
{
    if (!end)
        return;
    this->pos_en_line = end->pos_en_line;
    this->pos_en_line_char = end->pos_en_line_char;
}

ast::set_pos_start(ast* start)
{
    if (!start)
        return;
    this->pos_st_line = start->pos_st_line;
    this->pos_st_line_char = start->pos_st_line_char;
}

ast::set_pos_start(tok* start)
{
    if (!start)
        return;
    this->pos_st_line = start->pos_st_line;
    this->pos_st_line_char = start->pos_st_line_char;
}

ast::~ast()
{

}

uast::uast() : ast()
{

}

uast::~uast()
{

}

void uast::print(std::wostream& out) const
{
    out << L"<ast>";
}

tast::tast() : ast()
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

IdentNode::IdentNode(tok* code) : ast(code)
{
    this->str = code->string;
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
    schar* str1 = this->str,* str2 = other.str;

    while (*str1 && *str1 == *str2)
            str1++, str2++;

    return !((unsigned short)*str1 - (unsigned short)*str2);
}

NumNode::NumNode(int num)
{
    this->num = num;
}

NumNode::NumNode(NumNode* n) : ast(n)
{
    this->num = n->num;
}

NumNode::NumNode(tok* t) : ast(t)
{
    this->num = t->number;
}

void NumNode::print(std::wostream& out) const
{
    out << num;
}

StringNode::StringNode(schar* str)
{
    this->str = str;
}

StringNode::StringNode(tok* str) : ast(str, str)
{
    this->str = str->string;
}

StringNode::StringNode(StringNode* n) : ast(n)
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

OperatorNode::OperatorNode(tok* code) : ast(code)
{
    this->oper = code->oper;
}

OperatorNode::OperatorNode(OperatorNode* n) : ast(n)
{
    this->oper = n->oper;
}

void OperatorNode::print(std::wostream& out) const
{
    out << op_strings[(int)this->oper];
}

BoolNode::BoolNode(BoolNode* n) : ast(n)
{

}

void BoolNode::print(std::wostream& out) const
{

}

ASMNode::ASMNode(StringNode* str) : ast(str)
{
    this->str = str;
}

ASMNode::ASMNode(ASMNode* n) : ast(n)
{
    this->str = n->str;
}

void ASMNode::print(std::wostream& out) const
{

}

BreakNode::BreakNode(BreakNode* n) : ast(n)
{

}

void BreakNode::print(std::wostream& out) const
{

}

GoOnNode::GoOnNode(GoOnNode* n) : ast(n)
{

}

GoOnNode::GoOnNode(tok* n) : ast(n, n)
{

}

void GoOnNode::print(std::wostream& out) const
{

}

PopNode::PopNode()
{
    this->target = nullptr;
}

PopNode::PopNode(IdentNode* target) : ast(target)
{
    this->target = target;
}

PopNode::PopNode(PopNode* n) : ast(n)
{
    this->target = n->target;
}

void PopNode::print(std::wostream &out) const
{

}
