#include "il_nasm.h"
#include "errors/errors.hpp"
#include "errors/warnings.h"
#include "target.h"
#include "parsing/fmod.h"

using namespace std;

il_nasm::il_nasm(ProgramNode* code)
    : il(code)
{

}

il_nasm::~il_nasm()
{

}

schar const* il_nasm::get_cc()
{
    return L"; ";
}

void il_nasm::generate(ProgramNode* code)
{
    for (tast* block : *code->code)
        generate_ProgramNode_term(block);
};

void il_nasm::generate(BlockNode* code)
{
    for (tast* block : *code->code)
        generate(block);
};

void il_nasm::generate_sf_enter(int size)
{
    il::generate_sf_enter(size);
};

void il_nasm::generate_sf_leave(int size)
{
    il::generate_sf_leave(size);
};

void il_nasm::generate(ExpressionNode* code)
{

};

void il_nasm::generate(ExpressionTermNode* code)
{
    for (tast* e : *code->exps)
        generate(e);
};

void il_nasm::generate(AssignNode* code)
{
    generate(code->term);

    //write
    POP(rax);
    POP(rcx);

    eml(L"xor " << rdx << L", " << rdx);
    switch (code->src->size)
    {
        case 1:
            eml(L"mov " << dl << L", " << al);
            break;
        case 2:
            eml(L"mov " << dx << L", " << ax);
            break;
        case 4:
            eml(L"mov " << edx << L", " << eax);
            break;
        case 8:
            eml(L"mov " << rdx << L", " << rax);
            break;
        default:
            ERR(err_t::IL_CANT_ASSIGN_TO_TYPE, code);
            break;
    }

    if (!code->dest)
        ERR(err_t::IL_CANT_ASSIGN_TO_TYPE, code);
    switch (code->dest->size)
    {
        case 1:
            eml(L"mov byte  [" << rcx << L"], " << dl);
            break;
        case 2:
            eml(L"mov word  [" << rcx << L"], " << dx);
            break;
        case 4:
            eml(L"mov dword [" << rcx << L"], " << edx);
            break;
        case 8:
            eml(L"mov qword [" << rcx << L"], " << rdx);
            break;
        default:
            ERR(err_t::IL_CANT_ASSIGN_TO_TYPE, code);
            break;
    }
};

void il_nasm::generate(NumNode* code)
{
    PUSH(code->num);
};

void il_nasm::generate(IdentNode* code)
{

};

void il_nasm::generate(BoolNode* code)
{

};

void il_nasm::generate(PushNode* code)
{
    if (IdentNode* i = dynamic_cast<IdentNode*>(code->v))
        PUSH(i->str);
    else if (NumNode*  n = dynamic_cast<NumNode*>(code->v))
        PUSH(n->num);
    else if (VariableNode* v = dynamic_cast<VariableNode*>(code->v))     //its a variable in the stack frame
    {
        if (v->ebp_off)
        {
            std::wstring adder = v->ebp_off >= 0 ? std::wstring(L"+") + std::to_wstring(v->ebp_off) : std::to_wstring(v->ebp_off);
            eml(L"lea " << rdx << L", [ebp " << adder << L']');
            PUSH(rdx);
        }
        else
        {
            eml(L"lea " << rdx << L", [" << v->var_name->str << L']');
            PUSH(rdx);
        }
    }
    else
        ERR(err_t::GEN_IL);
};

void il_nasm::generate(PopNode* code)
{
    if (code->target)
        *ss << L"pop " << code->target->str;
    else
        *ss << L"add esp, " << __BYTES__;
};

void il_nasm::generate(ASMNode* code)
{
    eml(L"; begin inline ASM from line " << code->pos_st_line << L" pos " << code->pos_st_line_char);
    eml(code->str->str);
    eml(L"; end   inline ASM from line " << code->pos_en_line << L" pos " << code->pos_en_line_char);
};

void il_nasm::generate(StringNode* code)
{
    bool already_registered;
    std::wstring* name = il::generate_string_name(code->str, already_registered);

    if (!already_registered)
        emlDATA(name->c_str() << L": db \"" << code->str << L"\", 0");
    PUSH(name->c_str());
};

void il_nasm::generate_op_add(OperatorNode* code)
{
    POP(rax);
    POP(rcx);
    eml(L"add " << rax << L", " << rcx);
    PUSH(rax);
}

void il_nasm::generate_op_sub(OperatorNode* code)
{
    POP(rcx);
    POP(rax);
    eml(L"sub " << rax << L", " << rcx);
    PUSH(rax);
}

void il_nasm::generate_op_mul(OperatorNode* code)
{
    POP(eax);
    POP(ecx);
    eml(L"mul " << rcx);
    PUSH(eax);
}

void il_nasm::generate_op_div(OperatorNode* code)
{
    POP(rcx);
    POP(rax);
    eml(L"xor " << rdx << L", " << rdx);
    eml(L"div " << rcx);
    PUSH(rax);
}

void il_nasm::generate_op_drf(OperatorNode* code)
{
    POP(rcx);
    eml(L"xor " << rax << L", " << rax);

    switch (code->operand_type->size)
    {
        case 1:
            eml(L"mov  " <<  al << ", byte [" << rcx << "]");
            break;
        case 2:
            eml(L"mov  " <<  ax << ", word [" << rcx << "]");
            break;
        case 4:
            eml(L"mov  " << eax << ", dword [" << rcx << "]");
            break;
        case 8:
            eml(L"mov  " << rax << ", qword [" << rcx << "]");
            break;
        default:
            ERR(err_t::GEN_IL);
            break;
    }

    PUSH(rax);
}

void il_nasm::generate_op_equ(OperatorNode* code)
{
    generate_op_neq(code);
    generate_op_not(code);
}

void il_nasm::generate_op_sml(OperatorNode* code)
{
    std::wstring name = std::wstring(L"__sml_") + std::to_wstring(++sml_c);

    POP(rax);
    POP(rcx);

    eml(L"cmp " << rcx << ", " << rax);
    eml(L"jl " << name << L".ok");
    eml(L"jmp " << name << L".fail");

    eml(name << L".ok:");
    eml(L"xor " << rax << ", " << rax);
    eml(L"not " << rax);
    eml(L"jmp " << name << L".end");

    eml(name << L".fail:");
    eml("xor " << rax << ", " << rax);
    eml(L"jmp " << name << L".end");

    eml(name << L".end:");
    PUSH(rax);
}

void il_nasm::generate_op_grt(OperatorNode* code)
{
    std::wstring name = std::wstring(L"__grt_") + std::to_wstring(++grt_c);

    POP(rax);
    POP(rcx);

    eml(L"cmp " << rcx << ", " << rax);
    eml("jng " << name << L".fail");
    eml("jmp " << name << L".ok");

    eml(name << L".ok:");
    eml(L"xor " << rax << ", " << rax);
    eml(L"not " << rax);
    eml("jmp " << name << L".end");

    eml(name << L".fail:");
    eml("xor " << rax << ", " << rax);
    eml("jmp " << name << L".end");

    eml(name << L".end:");
    PUSH(rax);
}

void il_nasm::generate_op_neq(OperatorNode* code)
{
    POP(rax);
    POP(rcx);
    eml(L"xor " << rax << L", " << rcx);
    eml(L"call boolNormalize");
    PUSH(rax);
}

void il_nasm::generate_op_not(OperatorNode* code)
{
    POP(rax);
    eml(L"call boolNormalize");
    eml(L"call boolNot");
    PUSH(rax);
}

void il_nasm::generate_op_pop(OperatorNode* code)
{
    eml(L"add esp, " << __BYTES__);
}

void il_nasm::generate_op_cpy(OperatorNode* code)
{
    POP(rax);
    PUSH(rax);
    PUSH(rax);
}

void il_nasm::generate_op_and(OperatorNode* code)
{
    eml(L"call boolNormalize");
    POP(rax);
    eml(L"call boolNormalize");
    POP(rcx);
    eml(L"and " << rax << L", " << rcx);
    PUSH(rax);
}

void il_nasm::generate_op_or (OperatorNode* code)
{
    eml(L"call boolNormalize");
    POP(rax);
    eml(L"call boolNormalize");
    POP(rcx);
    eml(L"or  " << rax << L", " << rcx);
    PUSH(rax);
}

void il_nasm::generate(OperatorNode* code)
{
    il::generate(code);         //call the switch in base class
};

void il_nasm::generate(ReturnNode* code)
{
    eml(L"xor " << rax << L", " << rax);    //return 0 if no return value given
    generate(code->val);
    POP(rax);

    eml("jmp end_" << this->funtion_returns.top());
};

void il_nasm::generate(BreakNode* code)
{
    eml("jmp " << il::__str_break << std::to_wstring(brk_c));
};

void il_nasm::generate(GoOnNode* code)
{

};

void il_nasm::generate(TypeNode* code)
{
    em(L"; casting to ");
    code->print(*ss_code);
    eml(L"");
};

void il_nasm::generate_global(VariableNode* code)
{
    il::generate_global(code);

    emlBSS(code->var_name->str << L": resb " << code->type->t->size);
};

void il_nasm::generate_local(VariableNode* code)
{

};

void il_nasm::generate(ArgNode* code)
{

};

void il_nasm::generate(ListNode* code)
{

};

void il_nasm::generate(ListArgNode* code)
{

};

void il_nasm::generate(FunctionHeaderNode* code)
{

};

void il_nasm::generate(FunctionExternNode* code)
{
    emlCODEH(L"extern " << code->fname->str);
};

void il_nasm::generate(FunctionNode* code)
{
    schar* name = code->head->name->str;

    if (!wcscmp(name, L"test_if"))
    {
        int a = 0;
    }

    if ((int)code->head->mods & (int)FMod::GLOBAL)
        emlCODEH(L"global " << name);

    eml(name << L":\t;start of function " << name);

    this->funtion_returns.push(name);
    generate_sf_enter(code->code->stack_s);
    generate(code->code);

    eml(L"end_" << name << L":\t;end of function " << name);
    generate_sf_leave(code->code->stack_s);
    this->funtion_returns.pop();
};

void il_nasm::generate(FunctionCallNode* code)
{
    tast_arr* t = code->args->items;
    for (size_t i = t->size(); i --> 0;)    //push args in reversed order
        generate(t->at(i));

    eml(L"call " << code->target->str);
    eml(L"add esp, " << code->args_size);   //### TODO allign to stack size

    if (code->return_type->t->size)
        PUSH(rax);
};

void il_nasm::generate(AnomymousCallNode* code)
{
    POP(rax);
    //PUSH(L"anonym_end_" << ++anym_c);
    eml(L"jmp " << rax);

    eml(L"anonym_end_" << anym_c << L':');
}

void il_nasm::generate(IfNode* code)
{
    std::wstring name = il::__str_if + std::to_wstring(++if_c),
                 else_name = name +il::__str_else,
                 end_name  = name +il::__str_end;

    eml(name << ':');
    generate(code->cond);
    POP(rax);

    eml(L"test " << rax << ", -1");
    eml(L"jz " << else_name);
    generate(code->true_block);
    eml(L"jmp " << end_name);

    eml(else_name << L':');
    generate(code->false_block);

    eml(end_name << L':');
};

void il_nasm::generate(WhileNode* code)
{
    int my_brk = ++brk_c;
    std::wstring name = il::__str_while +std::to_wstring(my_brk),
                 start_name= name +il::__str_start,
                 code_name = name +il::__str_code,
                 end_name  = name +il::__str_end;

    eml(start_name << L':');
    generate(code->cond);
    POP(rax);
    eml(L"test " << rax << ", -1");
    eml(L"jnz " << code_name);
    eml(L"jmp " << end_name);

    eml(code_name << L':');
    generate(code->block);

    eml(L"jmp " << start_name);
    eml(il::__str_break << std::to_wstring(my_brk) << L':');
    eml(end_name << L':');
};

void il_nasm::generate(ForNode* code)
{
    int my_brk = ++brk_c;
    std::wstring name = il::__str_for + std::to_wstring(my_brk),
                 start_name= name +il::__str_start,
                 check_name= name +il::__str_check,
                 code_name = name +il::__str_code,
                 end_name  = name +il::__str_end;

    eml(start_name << L':');                    //int i = 0;
    generate(code->init);

    eml(check_name << L':');                    //i < LENGTH
    generate(code->cond);
    POP(rax);
    eml(L"test " << rax << ", -1");
    eml(L"jnz " << code_name);
    eml(L"jmp " << end_name);

    eml(code_name << L':');
    generate(code->block);              //do stuff in the loop
    generate(code->inc);                //i++
    eml("jmp " << check_name);          //start over

    eml(il::__str_break << std::to_wstring(my_brk) << L':');
    eml(end_name << L':');
};

void il_nasm::generate_ProgramNode_term(tast* code)
{
    if (VariableNode* v = dynamic_cast<VariableNode*>(code))
        generate_global(v);
    else
        generate(code);
}

void il_nasm::generate(tast* code)
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
        generate_local(dynamic_cast<VariableNode*>(code));
    else if (dynamic_cast<ArgNode*>(code))
        generate(dynamic_cast<ArgNode*>(code));
    else if (dynamic_cast<ListNode*>(code))
        generate(dynamic_cast<ListNode*>(code));
    else if (dynamic_cast<ListArgNode*>(code))
        generate(dynamic_cast<ListArgNode*>(code));
    else if (dynamic_cast<FunctionHeaderNode*>(code))
        generate(dynamic_cast<FunctionHeaderNode*>(code));
    else if (dynamic_cast<FunctionExternNode*>(code))
        generate(dynamic_cast<FunctionExternNode*>(code));
    else if (dynamic_cast<FunctionNode*>(code))
        generate(dynamic_cast<FunctionNode*>(code));
    else if (dynamic_cast<FunctionCallNode*>(code))
        generate(dynamic_cast<FunctionCallNode*>(code));
    else if (dynamic_cast<AnomymousCallNode*>(code))
        generate(dynamic_cast<AnomymousCallNode*>(code));
    else if (dynamic_cast<IfNode*>(code))
        generate(dynamic_cast<IfNode*>(code));
    else if (dynamic_cast<WhileNode*>(code))
        generate(dynamic_cast<WhileNode*>(code));
    else if (dynamic_cast<ForNode*>(code))
        generate(dynamic_cast<ForNode*>(code));
    else
        ERR(err_t::GEN, L"Type error");
}
