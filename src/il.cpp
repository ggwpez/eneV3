#include "il.h"
#include "errors.hpp"
#include "warnings.h"
#include "target.h"

#define pop(t) (eml(L"pop " << t))
#define em(s) emCODE(s)
#define eml(s) emlCODE(s)
#define emCODE(s) (*ss_code << s)
#define emlCODE(s) (*ss_code << s << std::endl)
#define emBSS(s) (*ss_bss << s)
#define emlBSS(s) (*ss_bss << s << std::endl)
#define emDATA(s) (*ss_data << s)
#define emlDATA(s) (*ss_data << s << std::endl)

int stack = 0;

using namespace std;

il::il(ProgramNode* code, std::wostringstream* ss)
{
    input = code;
    this->ss = ss;

    ss_code = new std::wostringstream();
    ss_data = new std::wostringstream();
    ss_bss  = new std::wostringstream();
}

il::~il()
{
    delete ss_code;
    delete ss_data;
    delete ss_bss;
}

void il::push(schar* t)
{
    PushNode n = PushNode(new IdentNode(t));    //identnode gets freed by ~PushNode()
    generate(&n);
}

void il::generate(ProgramNode* code)
{
    for (tast* block : *code->code)
        generate_ProgramNode_term(block);
};

void il::generate(BlockNode* code)
{
    for (tast* block : *code->code)
        generate(block);
};

void il::generate(ExpressionNode* code)
{

};

void il::generate(ExpressionTermNode* code)
{
    for (tast* e : *code->exps)
        generate(e);
};

void il::generate(AssignNode* code)
{

};

void il::generate(NumNode* code)
{

};

void il::generate(IdentNode* code)
{
    push(code->str);
};

void il::generate(BoolNode* code)
{

};

void il::generate(PushNode* code)
{
    IdentNode* i = dynamic_cast<IdentNode*>(code->v);
    NumNode*   n = dynamic_cast<NumNode*  >(code->v);

    em(L"push ");
    if (i)
        eml(i->str);
    else if (n)
        eml(n->num);
    else
        ERR(err_t::GEN_IL);
};

void il::generate(PopNode* code)
{
    if (code->target)
        *ss << L"pop " << code->target->str;
    else
        *ss << L"add esp, " << __BITS__;
};

void il::generate(ASMNode* code)
{

};

void il::generate(StringNode* code)
{
    size_t l = wcslen(code->str);
    schar c[] = {L'\'', 0, L'\'', 0};

    push(L"0");
    for (size_t i = l; i --> 0;)
        c[1] = code->str[i], push(c);
};

void il::generate(OperatorNode* code)
{

};

void il::generate(ReturnNode* code)
{

};

void il::generate(BreakNode* code)
{

};

void il::generate(GoOnNode* code)
{

};

void il::generate(TypeNode* code)
{

};

void il::generate_global(VariableNode* code)
{
    if (! code->type->t->size)
        WAR(war_t::INSTANCE_OF_VOID, code);

    emlBSS(code->var_name->str << L": resb " << code->type->t->size);
};

void il::generate_local(VariableNode* code)
{

};

void il::generate(ArgNode* code)
{

};

void il::generate(ListNode* code)
{

};

void il::generate(ListArgNode* code)
{

};

void il::generate(FunctionHeaderNode* code)
{

};

void il::generate(FunctionNode* code)
{
    em(code->head->name->str << L':' << endl <<
          L"push ebp    ;stack frame begin" << endl <<
          L"mov ebp, esp" << endl <<
          L"sub esp, " << (code->code->stack_s +code->head->args_size) << endl);

    generate(code->code);

    em(L".end:       ;jmp mark for return" << endl <<
          L"mov esp, ebp" << endl <<
          L"pop ebp" << endl <<
          L"ret " << code->head->args_size << endl);
};

void il::generate(FunctionCallNode* code)
{

};

int if_c = -1;
void il::generate(IfNode* code)
{
    std::wstring name = std::wstring(L"__if_") + std::to_wstring(++if_c);

    eml(name << ':' << endl);
    generate(code->cond);
    pop(L"eax");

    eml(L"test eax, " << __ALL_HIGH);
    eml(L"jz " << name << L".else");
    generate(code->true_block);
    eml(L"jmp " << name << L".end");

    eml(name << L".else");

    if (code->false_block)
        generate(code->false_block);

    eml(name << L".end");
};

int while_c = -1;
void il::generate(WhileNode* code)
{
    std::wstring name = std::wstring(L"__while_") + std::to_wstring(++while_c);

    eml(name << L".start:");
    generate(code->cond);
    pop(L"eax");
    eml(L"test eax, " << __ALL_HIGH);
    eml(L"jnz " << name << L".code");
    eml(L"jmp " << name << L".end");

    eml(name << L".code:");
    generate(code->block);

    eml(L"jmp " << name << L".start");
    eml(name << L".end:");
};

void il::generate()
{
    generate(input);

    *ss << L"section .bss"  << endl << ss_bss ->str() <<
           L"section .text" << endl << ss_code->str() <<
           L"section .data" << endl << ss_data->str();
}

void il::generate_ProgramNode_term(tast* code)
{
    if (dynamic_cast<VariableNode*>(code))
        generate_global(dynamic_cast<VariableNode*>(code));
    else
        generate(code);
}

void il::generate(tast* code)
{
    if (dynamic_cast<ProgramNode*>(code))
        generate(dynamic_cast<ProgramNode*>(code));
    else if (dynamic_cast<BlockNode*>(code))
        generate(dynamic_cast<BlockNode*>(code));
    else if (dynamic_cast<ExpressionNode*>(code))
        generate(dynamic_cast<ExpressionNode*>(code));
    else if (dynamic_cast<ExpressionTermNode*>(code))
        generate(dynamic_cast<ExpressionTermNode*>(code));
    else if (dynamic_cast<AssignNode*>(code))
        generate(dynamic_cast<AssignNode*>(code));
    else if (dynamic_cast<NumNode*>(code))
        generate(dynamic_cast<NumNode*>(code));
    else if (dynamic_cast<IdentNode*>(code))
        generate(dynamic_cast<IdentNode*>(code));
    else if (dynamic_cast<BoolNode*>(code))
        generate(dynamic_cast<BoolNode*>(code));
    else if (dynamic_cast<PushNode*>(code))
        generate(dynamic_cast<PushNode*>(code));
    else if (dynamic_cast<PopNode*>(code))
        generate(dynamic_cast<PopNode*>(code));
    else if (dynamic_cast<ASMNode*>(code))
        generate(dynamic_cast<ASMNode*>(code));
    else if (dynamic_cast<StringNode*>(code))
        generate(dynamic_cast<StringNode*>(code));
    else if (dynamic_cast<OperatorNode*>(code))
        generate(dynamic_cast<OperatorNode*>(code));
    else if (dynamic_cast<ReturnNode*>(code))
        generate(dynamic_cast<ReturnNode*>(code));
    else if (dynamic_cast<BreakNode*>(code))
        generate(dynamic_cast<BreakNode*>(code));
    else if (dynamic_cast<GoOnNode*>(code))
        generate(dynamic_cast<GoOnNode*>(code));
    else if (dynamic_cast<TypeNode*>(code))
        generate(dynamic_cast<TypeNode*>(code));
    else if (dynamic_cast<VariableNode*>(code))
        generate(dynamic_cast<VariableNode*>(code));
    else if (dynamic_cast<ArgNode*>(code))
        generate(dynamic_cast<ArgNode*>(code));
    else if (dynamic_cast<ListNode*>(code))
        generate(dynamic_cast<ListNode*>(code));
    else if (dynamic_cast<ListArgNode*>(code))
        generate(dynamic_cast<ListArgNode*>(code));
    else if (dynamic_cast<FunctionHeaderNode*>(code))
        generate(dynamic_cast<FunctionHeaderNode*>(code));
    else if (dynamic_cast<FunctionNode*>(code))
        generate(dynamic_cast<FunctionNode*>(code));
    else if (dynamic_cast<FunctionCallNode*>(code))
        generate(dynamic_cast<FunctionCallNode*>(code));
    else if (dynamic_cast<IfNode*>(code))
        generate(dynamic_cast<IfNode*>(code));
    else if (dynamic_cast<WhileNode*>(code))
        generate(dynamic_cast<WhileNode*>(code));
    else
        ERR(err_t::GEN, L"Type error");
}
