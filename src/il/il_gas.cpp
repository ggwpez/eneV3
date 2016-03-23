#include "il_gas.h"
#include "errors/errors.hpp"
#include "errors/warnings.h"
#include "target.h"

using namespace std;

il_gas::il_gas(ProgramNode* code, std::wostringstream* ss)
    : il(code, ss)
{

}

il_gas::~il_gas()
{

}

void il_gas::generate(ProgramNode* code)
{
    for (tast* block : *code->code)
        generate_ProgramNode_term(block);
};

void il_gas::generate(BlockNode* code)
{
    for (tast* block : *code->code)
        generate(block);
};

void il_gas::generate_sf_enter(int size)
{
    il::generate_sf_enter(size);
};

void il_gas::generate_sf_leave(int size)
{
    il::generate_sf_leave(size);
};

void il_gas::generate(ExpressionNode* code)
{

};

void il_gas::generate(ExpressionTermNode* code)
{
    for (tast* e : *code->exps)
        generate(e);
};

void il_gas::generate(AssignNode* code)
{
    generate(code->term);

    //write
    pop(rax);
    pop(rcx);

    switch (code->to_write->size)
    {
        case 1:
            eml(L"mov [" << rcx << "], " << al);
            break;
        case 2:
            eml(L"mov [" << rcx << "], " << ax);
            break;
        case 4:
            eml(L"mov [" << rcx << "], " << eax);
            break;
        case 8:
            eml(L"mov [" << rcx << "], " << rax);
            break;
        default:
            ERR(err_t::IL_CANT_ASSIGN_TO_TYPE, code);
            break;
    }
};

void il_gas::generate(NumNode* code)
{
    push(code->num);
};

void il_gas::generate(IdentNode* code)
{

};

void il_gas::generate(BoolNode* code)
{

};

void il_gas::generate(PushNode* code)
{
    if (IdentNode* i = dynamic_cast<IdentNode*>(code->v))
    {
        eml(L"lea " << rdx << L", " << i->str);
        push(rdx);
    }
    else if (NumNode*   n = dynamic_cast<NumNode*  >(code->v))
        push(n->num);
    else if (VariableNode* v = dynamic_cast<VariableNode*>(code->v))     //its a variable in the stack frame
    {
        if (v->ebp_off)
        {
            std::wstring adder = v->ebp_off >= 0 ? std::wstring(L"+") + std::to_wstring(v->ebp_off) : std::to_wstring(v->ebp_off);
            eml(L"lea " << rdx << L", [ebp " << adder << L']');
            push(rdx);
        }
        else
        {
            eml(L"lea " << rdx << L", [" << v->var_name->str << L']');
            push(rdx);
        }
    }
    else
        ERR(err_t::GEN_IL);
};

void il_gas::generate(PopNode* code)
{
    if (code->target)
        *ss << L"pop " << code->target->str;
    else
        *ss << L"add esp, " << __BYTES__;
};

void il_gas::generate(ASMNode* code)
{
    eml(L"# begin inline ASM from line " << code->pos_st_line << L" pos " << code->pos_st_line_char);
    eml(code->str->str);
    eml(L"# end   inline ASM from line " << code->pos_en_line << L" pos " << code->pos_en_line_char);
};

void il_gas::generate(StringNode* code)
{
    std::wstring name = std::wstring(L"__str_") + std::to_wstring(++str_c);

    emlDATA(name << L": .asciz \"" << code->str << L'"');
    generate(&PushNode(new IdentNode(name.c_str())));
};

void il_gas::generate_op_add(OperatorNode* code)
{
    pop(rax);
    pop(rcx);
    eml(L"add " << rax << L", " << rcx);
    push(rax);
}

void il_gas::generate_op_sub(OperatorNode* code)
{
    pop(rcx);
    pop(rax);
    eml(L"sub " << rax << L", " << rcx);
    push(rax);
}

void il_gas::generate_op_mul(OperatorNode* code)
{
    pop(eax);
    pop(ecx);
    eml(L"mul " << rcx);
    push(eax);
}

void il_gas::generate_op_div(OperatorNode* code)
{
    pop(rcx);
    pop(rax);
    eml(L"xor " << rdx << L", " << rdx);
    eml(L"div " << rcx);
    push(rax);
}

void il_gas::generate_op_drf(OperatorNode* code)
{
    pop(rax);

    switch (code->operand_type->size)
    {
        case 1:
            eml(L"mov " <<  al << ", [" << rax << "]");
            break;
        case 2:
            eml(L"mov " <<  ax << ", [" << rax << "]");
            break;
        case 4:
            eml(L"mov " << eax << ", [" << rax << "]");
            break;
        case 8:
            eml(L"mov " << rax << ", [" << rax << "]");
            break;
        default:
            ERR(err_t::GEN_IL);
            break;
    }

    push(rax);
}

void il_gas::generate_op_equ(OperatorNode* code)
{
    pop(rax);
    pop(rcx);
    eml(L"xor " << rax << L", " << rcx);
    eml(L"not " << rax);
    push(rax);

    //generate_op_neq(code);
    //generate_op_not(code);
}

void il_gas::generate_op_sml(OperatorNode* code)
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

void il_gas::generate_op_grt(OperatorNode* code)
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

void il_gas::generate_op_neq(OperatorNode* code)
{
    pop(rax);
    pop(rcx);
    eml(L"xor " << rax << L", " << rax);
    eml(L"call boolNormalize");
    push(rax);
}

void il_gas::generate_op_not(OperatorNode* code)
{
    pop(rax);
    eml(L"call boolNot");
    push(rax);
}

void il_gas::generate_op_pop(OperatorNode* code)
{
    eml(L"add esp, " << __BYTES__);
}

void il_gas::generate_op_cpy(OperatorNode* code)
{
    pop(rax);
    push(rax);
    push(rax);
}

void il_gas::generate_op_and(OperatorNode* code)
{
    eml(L"call boolNormalize");
    pop(rax);
    eml(L"call boolNormalize");
    pop(rcx);
    eml(L"and " << rax << L", " << rcx);
    push(rax);
}

void il_gas::generate_op_or (OperatorNode* code)
{
    eml(L"call boolNormalize");
    pop(rax);
    eml(L"call boolNormalize");
    pop(rcx);
    eml(L"or  " << rax << L", " << rcx);
    push(rax);
}

void il_gas::generate(OperatorNode* code)
{
    il::generate(code);
};

int ret_c = 0;
void il_gas::generate(ReturnNode* code)
{
    generate(code->val);
    pop(rax);

    eml("jmp .end_" << ret_c);
};

void il_gas::generate(BreakNode* code)
{

};

void il_gas::generate(GoOnNode* code)
{

};

void il_gas::generate(TypeNode* code)
{
    em(L"# casting to ");
    code->print(*ss_code);
    eml(L"");
};

void il_gas::generate_global(VariableNode* code)
{
    il::generate_global(code);

    emlBSS(L".lcomm " << code->var_name->str << L", " << code->type->t->size);
};

void il_gas::generate_local(VariableNode* code)
{

};

void il_gas::generate(ArgNode* code)
{

};

void il_gas::generate(ListNode* code)
{

};

void il_gas::generate(ListArgNode* code)
{

};

void il_gas::generate(FunctionHeaderNode* code)
{

};

void il_gas::generate(FunctionExternNode* code)
{
    emlCODEH(L".extern " << code->fname->str);
};

void il_gas::generate_ssp_init()
{

}

void il_gas::generate_ssp_check()
{

}

void il_gas::generate(FunctionNode* code)
{
    eml(code->head->name->str << L':');
    generate_sf_enter(code->code->stack_s);

    generate(code->code);

    eml(".end_" << ++ret_c << ":");
    generate_sf_leave(code->code->stack_s);
};

void il_gas::generate(FunctionCallNode* code)
{
    tast_arr* t = code->args->items;
    for (size_t i = t->size(); i --> 0;)    //push args in reversed order
        generate(t->at(i));

    eml(L"call " << code->target->str);
    eml(L"add esp, " << code->args_size);

    if (code->return_type->t->size)
        push(rax);
};

void il_gas::generate(AnomymousCallNode* code)
{
    pop(rcx);
    eml(L"call get_eip");
    eml(L"add " << rax << L", " << 2 *__BYTES__);
    push(rax);
    push(rcx);
    eml(L"ret");
}

void il_gas::generate(IfNode* code)
{
    std::wstring name = std::wstring(L"__if_") + std::to_wstring(++if_c);

    eml(name << ':');
    generate(code->cond);
    pop(rax);

    eml(L"test " << rax << ", -1");
    eml(L"jz " << name << L".else");
    generate(code->true_block);
    eml(L"jmp " << name << L".end");

    eml(name << L".else:");

    if (code->false_block)
        generate(code->false_block);

    eml(name << L".end:");
};

void il_gas::generate(WhileNode* code)
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

void il_gas::generate()
{
    generate(input);

    *ss << ".bss"  << endl << ss_bss ->str()  <<
           ".text" << endl << ss_codeh->str() << ss_code->str() <<
           ".data" << endl << ss_data->str();
}

void il_gas::generate_ProgramNode_term(tast* code)
{
    if (VariableNode* v = dynamic_cast<VariableNode*>(code))
        generate_global(v);
    else
        generate(code);
}

void il_gas::generate(tast* code)
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
    else
        ERR(err_t::GEN, L"Type error");
}
