#include "il_nasm.h"
#include "errors/errors.hpp"
#include "errors/warnings.h"
#include "target.h"

using namespace std;

il_nasm::il_nasm(ProgramNode* code, std::wostringstream* ss)
    : il(code, ss)
{

}

il_nasm::~il_nasm()
{

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
    pop(rax);
    pop(rcx);

    switch (code->to_write->size)
    {
        case 1:
            eml(L"mov byte  [" << rcx << "], " << al);
            break;
        case 2:
            eml(L"mov word  [" << rcx << "], " << ax);
            break;
        case 4:
            eml(L"mov dword [" << rcx << "], " << eax);
            break;
        case 8:
            eml(L"mov qword [" << rcx << "], " << rax);
            break;
        default:
            ERR(err_t::IL_CANT_ASSIGN_TO_TYPE, code);
            break;
    }
};

void il_nasm::generate(NumNode* code)
{
    push(code->num);
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
        push(i->str);
    else if (NumNode*   n = dynamic_cast<NumNode*>(code->v))
        push(n->num);
    else if (VariableNode* v = dynamic_cast<VariableNode*>(code->v))     //its a variable in the stack frame
    {
        std::wstring adder = v->ebp_off >= 0 ? std::wstring(L"+") + std::to_wstring(v->ebp_off) : std::to_wstring(v->ebp_off);
        eml(L"lea " << rdx << L", [ebp " << adder << L']');
        push(rdx);
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
    std::wstring name = std::wstring(L"__str_") + std::to_wstring(++str_c);

    emlDATA(name << L": db \"" << code->str << L"\", 0");
    push(name.c_str());
};

void il_nasm::generate_op_add(OperatorNode* code)
{
    pop(rax);
    pop(rcx);
    eml(L"add " << rax << L", " << rcx);
    push(rax);
}

void il_nasm::generate_op_sub(OperatorNode* code)
{
    pop(rcx);
    pop(rax);
    eml(L"sub " << rax << L", " << rcx);
    push(rax);
}

void il_nasm::generate_op_mul(OperatorNode* code)
{
    pop(eax);
    pop(ecx);
    eml(L"mul " << rcx);
    push(eax);
}

void il_nasm::generate_op_div(OperatorNode* code)
{
    pop(rax);
    pop(rcx);
    eml(L"xor " << rdx << L", " << rdx);
    eml(L"div " << rcx);
    push(rax);
}

void il_nasm::generate_op_drf(OperatorNode* code)
{
    pop(rax);

    switch (code->operand_type->size)
    {
        case 1:
            eml(L"mov  " <<  al << ", byte [" << rax << "]");
            break;
        case 2:
            eml(L"mov  " <<  ax << ", word [" << rax << "]");
            break;
        case 4:
            eml(L"mov  " << eax << ", dword [" << rax << "]");
            break;
        case 8:
            eml(L"mov  " << rax << ", qword [" << rax << "]");
            break;
        default:
            ERR(err_t::GEN_IL);
            break;
    }

    push(rax);
}

void il_nasm::generate_op_equ(OperatorNode* code)
{
    generate_op_neq(code);
    generate_op_not(code);
}

void il_nasm::generate_op_sml(OperatorNode* code)
{
    std::wstring name = std::wstring(L"__sml_") + std::to_wstring(++sml_c);

    pop(rax);
    pop(rcx);

    eml(L"cmp " << rcx << ", " << rax);
    eml(L"jl " << name << L".ok");
    eml(L"jmp " << name << L".fail");

    eml(name << L".ok:");
    //eml(L"mov " << rax << ", ~0");
    eml(L"xor " << rax << ", " << rax);
    eml(L"not " << rax);
    eml(L"jmp " << name << L".end");

    eml(name << L".fail:");
    eml("xor " << rax << ", " << rax);
    eml(L"jmp " << name << L".end");

    eml(name << L".end:");
    push(rax);
}

void il_nasm::generate_op_grt(OperatorNode* code)
{
    std::wstring name = std::wstring(L"__grt_") + std::to_wstring(++grt_c);

    pop(rax);
    pop(rcx);

    eml(L"cmp " << rcx << ", " << rax);
    eml("jng " << name << L".fail");
    eml("jmp " << name << L".ok");

    eml(name << L".ok:");
    //eml("mov " << rax << ", ~0");
    eml(L"xor " << rax << ", " << rax);
    eml(L"not " << rax);
    eml("jmp " << name << L".end");

    eml(name << L".fail:");
    eml("xor " << rax << ", " << rax);
    eml("jmp " << name << L".end");

    eml(name << L".end:");
    push(rax);
}

void il_nasm::generate_op_neq(OperatorNode* code)
{
    pop(rax);
    pop(rcx);
    eml(L"xor " << rax << L", " << rax);
    eml(L"call boolNormalize");
    push(rax);
}

void il_nasm::generate_op_not(OperatorNode* code)
{
    pop(rax);
    eml(L"call boolNot");
    push(rax);
}

void il_nasm::generate_op_pop(OperatorNode* code)
{
    eml(L"add esp, " << __BYTES__);
}

void il_nasm::generate_op_cpy(OperatorNode* code)
{
    pop(rax);
    push(rax);
    push(rax);
}

void il_nasm::generate(OperatorNode* code)
{
    switch (code->oper)
    {
        case op::ADD:
            generate_op_add(code);
            break;
        case op::SUB:
            generate_op_sub(code);
            break;
        case op::MUL:
            generate_op_mul(code);
            break;
        case op::DIV:
            generate_op_div(code);
            break;
        case op::DRF:
            generate_op_drf(code);
            break;
        case op::EQU:
            generate_op_equ(code);
            break;
        case op::SML:
            generate_op_sml(code);
            break;
        case op::GRT:
            generate_op_grt(code);
            break;
        case op::NEQ:
            generate_op_neq(code);
            break;
        case op::POP:
            generate_op_pop(code);
            break;
        case op::CPY:
            generate_op_cpy(code);
            break;
        default:
            ERR(err_t::GEN_IL);
            break;
    }
};

void il_nasm::generate(ReturnNode* code)
{
    generate(code->val);
    pop(rax);

    eml("jmp .end");
};

void il_nasm::generate(BreakNode* code)
{

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

void il_nasm::generate_ssp_init()
{

}

void il_nasm::generate_ssp_check()
{

}

void il_nasm::generate(FunctionNode* code)
{
    eml(code->head->name->str << L':');
    generate_sf_enter(code->code->stack_s);

    generate(code->code);

    eml(code->head->name->str << L".end:");
    generate_sf_leave(0);
};

void il_nasm::generate(FunctionCallNode* code)
{
    tast_arr* t = code->args->items;
    for (size_t i = t->size(); i --> 0;)    //push args in reversed order
        generate(t->at(i));

    eml(L"call " << code->target->str);
    eml(L"add esp, " << code->args_size);   //### TODO allign to stack size

    if (code->return_type->t->size)
        push(rax);
};

void il_nasm::generate(IfNode* code)
{
    std::wstring name = std::wstring(L"__if_") + std::to_wstring(++if_c);

    eml(name << ':' << endl);
    generate(code->cond);
    pop(rax);

    eml(L"test " << rax << ", -1");
    eml(L"jz " << name << L".else");
    generate(code->true_block);
    eml(L"jmp " << name << L".end");

    eml(name << L".else");

    if (code->false_block)
        generate(code->false_block);

    eml(name << L".end");
};

void il_nasm::generate(WhileNode* code)
{
    std::wstring name = std::wstring(L"__while_") + std::to_wstring(++while_c);

    eml(name << L".start:");
    generate(code->cond);
    pop(L"eax");
    eml(L"test " << rax << ", -1");
    eml(L"jnz " << name << L".code");
    eml(L"jmp " << name << L".end");

    eml(name << L".code:");
    generate(code->block);

    eml(L"jmp " << name << L".start");
    eml(name << L".end:");
};

void il_nasm::generate()
{
    generate(input);

    *ss << "section .bss"  << endl << ss_bss ->str()  <<
           "section .text" << endl << ss_codeh->str() << ss_code->str() <<
           "section .data" << endl << ss_data->str();
}

void il_nasm::generate_ProgramNode_term(tast* code)
{
    if (dynamic_cast<VariableNode*>(code))
        generate_global(dynamic_cast<VariableNode*>(code));
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
    else if (dynamic_cast<IfNode*>(code))
        generate(dynamic_cast<IfNode*>(code));
    else if (dynamic_cast<WhileNode*>(code))
        generate(dynamic_cast<WhileNode*>(code));
    else
        ERR(err_t::GEN, L"Type error");
}
