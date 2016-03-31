#include "parser.h"
#include "errors/errors.hpp"
#include "fmod.h"

#define is(v, t) (dynamic_cast<t*>(v) != nullptr)
#define dc(t, v)  dynamic_cast<t*>(v)

#define null nullptr
#define tassert(tt, to) {if (to->type != tt) ERR(err_t::PAR_WRONG_BUT, (int)tt, to); }

parser::parser(std::vector<tok *> *input)
{
    this->input = *input;
    this->length = input->size();
}

uast* parser::parse()
{
    int l = 0;
    uast* prog_ast = parse_program(0, l);

    if (input[l]->type == tok_type::EOS)
        return prog_ast;
    else
        ERR(err_t::GEN, L"Parsing could not be completed");

    return nullptr;
}

ProgramUNode* parser::parse_program(int s, int& l)
{
    uast_arr* code = new uast_arr();
    int tmp_l = 0; l = 0;
    uast* statement = null;

    while ((input[s +l]->type != tok_type::EOS) && ((statement = parse_program_statement(s +l, tmp_l)) != nullptr))
    {
        l += tmp_l;
        code->push_back(statement);
    }

    return new ProgramUNode(code);
}

uast* parser::parse_program_statement(int s, int& l)
{
    if (input[s]->type == tok_type::IDENT)
        return parse_single_ident_program(s, l);                   //first one is ident
    if (input[s]->type == tok_type::NSPACE)
        return parse_namespace_block(s, l);
    else if (input[s]->type == tok_type::ASM)
        return parse_asm(s, l);
    else if (input[s]->type == tok_type::STRUCT)
        return parse_struct(s, l);
    else if (input[s]->type == tok_type::SEMI)
    { l = 1; return new ExpressionTermUNode(new uast_arr()); }

    ERR(err_t::PAR_WRONG, input[s]);
}

uast* parser::parse_single_ident_program(int s, int& l)
{
    tassert(tok_type::IDENT, input[s]);

    if (input[s +1]->type == tok_type::DDOT || input[s +1]->type == tok_type::CIRCLE)     //global var definition
        return parse_variable_definition(s, l);
    else if (input[s +1]->type == tok_type::IDENT)                             //global function definition
        return parse_function_definition(s, l);
    else
        ERR(err_t::PAR_WRONG, input[s +1]);

    l = 0; return nullptr;
}

VariableUNode* parser::parse_variable_definition(int s, int& l)
{
    tassert(tok_type::IDENT, input[s]);

    int type_l = 0, var_name_l = 0;
    TypeUNode* type = parse_type(s, type_l);
    l = type_l;

    tassert(tok_type::DDOT, input[s +l]); l++;

    IdentNode* var_name = parse_ident(s +l, var_name_l);
    l += var_name_l;

    tassert(tok_type::SEMI, input[s +l]); l++;

    return new VariableUNode(type, var_name);
}

FMod parser::parse_mods(int s, int& l)
{
    tassert(tok_type::LEBRK, input[s]); l = 1;
    FMod mod = (FMod)0;

    while ((s +l) < length && input[s +l]->type == tok_type::IDENT)
    {
        int i;
        for (i = (int)FMod::_NONE +1; i <= (int)FMod::_SIZE; i++)
            if (!wcscmp(input[s +l]->string, FMod_strings[i]))
            {
                mod = (FMod)((int)mod | (1 << i));      //wtf
                break;
            }

        if (i == (int)FMod::_SIZE)
            ERR(err_t::PAR_WRONG, input[s +l]);
        else
            l++;
    }

    tassert(tok_type::REBRK, input[s +l]); l++;
}

uast* parser::parse_function_definition(int s, int& l)
{
    int name_len = 0, args_len = 0, block_len = 0, mods_l = 0; l = 0;

    TypeUNode* ftype = parse_type(s, l);

    tassert(tok_type::IDENT, input[s +l]);
    IdentNode* fname = parse_ident(s +l, name_len);
    l += name_len;

    ListArgUNode* fargs = parse_arg_list(s +l, args_len);
    l += args_len;

    FMod mods = FMod::_NONE;
    if (input[s +l]->type == tok_type::LEBRK)
        mods = parse_mods(s +l, mods_l);
    l += mods_l;

    FunctionHeaderUNode* head = new FunctionHeaderUNode(ftype, fname, fargs, mods);

    if (input[s +l]->type == tok_type::SEMI)            //is it only a header?
        return head;

    BlockUNode* code = parse_block(s +l, block_len);
    l += block_len;

    if (input[s +l]->type == tok_type::SEMI) l++;       //; behind function block is legal

    return new FunctionUNode(head, code);
}

uast* parser::parse_namespace_block(int s, int& l)
{

}

uast* parser::parse_asm(int s, int& l)
{
    l = 0;
    tassert(tok_type::ASM,      input[s]);      l++;
    tassert(tok_type::LBRK,     input[s +l]);   l++;

    int str_l = 0;
    StringNode* str = parse_string(s +l, str_l);
    l += str_l;

    tassert(tok_type::RBRK,     input[s +l]);   l++;

    return new ASMNode(str);
}

uast* parser::parse_struct(int s, int& l)
{

}

BlockUNode *parser::parse_block(int s, int& l)
{
    uast_arr* code = new uast_arr();
    int tmp_l = 0; l = 0;
    uast* statement = null;

    if (input[s]->type == tok_type::SEMI)
        return l = 1, new BlockUNode(code);

    tassert(tok_type::LCBRK, input[s]); l++;

    if (input[s]->type == tok_type::RCBRK)
        return l = 2, new BlockUNode(code);

    while (input[s +l]->type != tok_type::RCBRK)
    {
        statement = parse_block_statement(s +l, tmp_l);

        if (tmp_l == 0)
            ERR(err_t::PAR_WRONG, input[s +l]);

        l += tmp_l;
        code->push_back(statement);
    } l++;

    if (input[s +l]->type == tok_type::SEMI) l++;

    return new BlockUNode(code);
}

uast* parser::parse_block_statement(int s, int& l)
{
    if (input[s]->type == tok_type::IDENT)
        return parse_single_ident_block(s, l);
    else if (input[s]->type == tok_type::WHILE)
        return parse_while(s, l);
    else if (input[s]->type == tok_type::IF)
        return parse_if(s, l);
    else if (input[s]->type == tok_type::RETURN)
        return parse_return(s, l);
    else if (input[s]->type == tok_type::ASM)
        return parse_asm(s, l);
    else if (input[s]->type == tok_type::LBRK)
        return parse_anonymous_call(s, l);
    else
        return parse_expression_term(s, l);
}

uast* parser::parse_single_ident_block(int s, int& l)
{
    tassert(tok_type::IDENT, input[s]); l = 1;

    if (input[s +l]->type == tok_type::DDOT || input[s +l]->type == tok_type::CIRCLE)     //local var definition
        return parse_variable_definition(s, l);
    else if (input[s +l]->type == tok_type::LBRK || input[s +l]->type == tok_type::DDDOT) //function call
        return parse_function_call(s, l);
    else                                                            //just two pushes
        return parse_expression_term(s, l);
}

IdentNode* parser::parse_ident(int s, int& l)
{
    tassert(tok_type::IDENT, input[s]);

    l = 1;
    return new IdentNode(input[s]);
}

NumNode* parser::parse_number(int s, int& l)
{
    tassert(tok_type::NUM, input[s]);

    l = 1;
    return new NumNode(input[s]);
}

StringNode* parser::parse_string(int s, int& l)
{
    tassert(tok_type::STRING, input[s]);

    l = 1;
    return new StringNode(input[s]);
}

uast* parser::parse_single_ident_expression(int s, int& l)
{
    tassert(tok_type::IDENT, input[s]);

    if (input[s +1]->type == tok_type::LBRK)
        return parse_function_call(s, l);
    else
        return parse_expression(s, l);
}

uast* parser::parse_expression(int s, int& l)
{
    if (input[s]->type == tok_type::IDENT)
    {
        uast* typeAST = parse_type(s, l);   //TODO: ### was parse_type
        if (l == 1)
        {
            l = 0;
            delete typeAST;
            return parse_ident(s, l);
        }

        return typeAST;
    }
    else if (input[s]->type == tok_type::NUM)
    {
        NumNode* n = parse_number(s, l);
        return new PushUNode(n);
    }
    else if (input[s]->type == tok_type::OP)
    {
        l = 1;
        return new OperatorUNode(input[s]);
    }
    else
        ERR(err_t::PAR_WRONG, input[s]);

    l = 0; return null;
}

ExpressionTermUNode* parser::parse_expression_term(int s, int& l)
{
    uast_arr* term = new uast_arr();
    int exp_l = 0; l = 0;
    uast* expressionAST = null;

    while (true)
    {
       //Token tmp = toMatch[s +l];

       if (input[s +l]->type == tok_type::ASSIGN)
           expressionAST = parse_assign(s +l, exp_l);
       else if (input[s +l]->type == tok_type::NUM)
           expressionAST = parse_expression(s +l, exp_l);
       else if (input[s +l]->type == tok_type::IDENT)
           expressionAST = parse_single_ident_expression(s +l, exp_l);
       else if (input[s +l]->type == tok_type::OP)
           expressionAST = parse_expression(s +l, exp_l);
       else if (input[s +l]->type == tok_type::ASM)
           expressionAST = parse_asm(s +l, exp_l);
       else if (input[s +l]->type == tok_type::STRING)
           expressionAST = parse_string(s +l, exp_l);
      // else if (input[s +l]->type == tok_type::DOT)
        //   expressionAST = parse_member_access(s +l, exp_l);
       else if (input[s +l]->type == tok_type::SEMI)
       { l += 1; break; }
       else if (input[s +l]->type == tok_type::RBRK || input[s +l]->type == tok_type::COMMA)
           break;
       else if (l == 0)
           ERR(err_t::PAR_WRONG, input[s +l]);
       else
           break;

       l += exp_l;
       term->push_back(expressionAST);

       if (input[s +l]->type == tok_type::SEMI) break;
       //if (expressionAST is AssignUNode && toMatch[s +l] is TokSemi)
        //   l--;
    }

    ExpressionTermUNode* ret = new ExpressionTermUNode(term);
    if (ret->exps->empty())
        ret->set_pos(input[s]);
    return ret;
}

ListArgUNode* parser::parse_arg_list(int s, int& l)
{
    int arg_l = 0; l = 0;
    std::vector<ArgUNode*>* args = new std::vector<ArgUNode*>();
    ListArgUNode* ret = new ListArgUNode(args);
    ret->set_pos(input[s]);

    tassert(tok_type::LBRK, input[s +l]); l++;

    if (input[s +l]->type == tok_type::RBRK)            //no arguments there!
        return l++, ret;

    ArgUNode* arg0 = parse_arg(s +l, arg_l);
    args->push_back(arg0);

    while ((l += arg_l) != 0 && input[s +l]->type == tok_type::COMMA)
        args->push_back(parse_arg(s + ++l, arg_l));

    tassert(tok_type::RBRK, input[s +l]); l++;

    ret->set_pos_end(input[s +l]);
    return ret;
}

ArgUNode* parser::parse_arg(int s, int& l)
{
    int type_l, name_l = 0;
    tassert(tok_type::IDENT, input[s]); l = 0;

    TypeUNode* type = parse_type(s, type_l);
    l += type_l;

    tassert(tok_type::IDENT, input[s +l]);
    IdentNode* name = parse_ident(s +l, name_l);
    l += name_l;

    return new ArgUNode(name, type);
}

TypeUNode* parser::parse_type(int s, int& l)
{
    if (input[s]->type == tok_type::BACKSLASH)
        return parse_fptr_type(s, l);
    else
        tassert(tok_type::IDENT, input[s]);

    l = 0;
    IdentNode* tname = parse_ident(s, l);
    std::vector<IdentNode*>* parts = new std::vector<IdentNode*>();
    parts->push_back(tname);

    while (input[s +l]->type == tok_type::CIRCLE)
    {
        l++;
        parts->push_back(new IdentNode(L"°"));
    }

    return new AtomTypeUNode(parts);
}

TypeUNode* parser::parse_fptr_type(int s, int& l)
{
    tassert(tok_type::BACKSLASH, input[s +l++]);
    int args_l = 0, type_l = 0;

    ListTypeUNode* args = parse_type_list(s +l, args_l);
    args_l += l;
    tassert(tok_type::DDOT, input[s +l++]);

    TypeUNode* ret_type = parse_type(s +l, type_l);
    l += type_l;

    return new FptrTypeUNode(args, ret_type);
}

ListTypeUNode* parser::parse_type_list(int s, int& l)
{
    std::vector<TypeUNode*>* types = new std::vector<TypeUNode*>();
    ListTypeUNode* ret = new ListTypeUNode(types); l = 0;
    int type_l;
    ret->set_pos_start(input[s]);

    TypeUNode* next = null;

    while ((l += type_l) +s < this->length && (input[s +l]->type == tok_type::IDENT || input[s +l]->type == tok_type::BACKSLASH))
    {
        type_l = 0;
        next = parse_type(s +l, type_l);
        types->push_back(next);
    }

    ret->set_pos_end(input[s +l]);
    return ret;
}

AssignUNode* parser::parse_assign(int s, int& l)
{
    tassert(tok_type::ASSIGN, input[s]);

    int val_len = 0; l = 1;
    ExpressionTermUNode* term = parse_expression_term(s +l, val_len);

    l += val_len;

    return new AssignUNode(term);
}

FunctionCallUNode* parser::parse_function_call(int s, int& l)
{
    int args_l = 0;
    IdentNode* target = parse_ident(s, l);

    ListUNode* call_args = parse_list(s +l, args_l);
    l += args_l;

    return new FunctionCallUNode(target, call_args);
}

AnomymousCallUNode* parser::parse_anonymous_call(int s, int& l)
{
    int args_l = 0;

    ListUNode* call_args = parse_list(s, args_l);
    l += args_l;

    return new AnomymousCallUNode(call_args);
}

ListUNode* parser::parse_list(int s, int& l)
{
    int it_l = 0; l = 0;
    uast_arr* items = new uast_arr();

    tassert(tok_type::LBRK, input[s]); l++;

    if (input[s +l]->type == tok_type::RBRK)
        return l++, new ListUNode(items);

    uast* it0 = parse_expression_term(s +l, it_l);
    items->push_back(it0);

    while ((l += it_l) != 0 && input[s +l]->type == tok_type::COMMA)
        items->push_back(parse_expression_term(s + ++l, it_l));

    tassert(tok_type::RBRK, input[s +l]); l++;

    return new ListUNode(items);
}

WhileUNode* parser::parse_while(int s, int& l)
{
    tassert(tok_type::WHILE, input[s]);
    tassert(tok_type::LBRK, input[s +1]);

    int cond_l = 0; l = 2;
    ExpressionTermUNode* cond = parse_expression_term(s +l, cond_l);
    l += cond_l;

    tassert(tok_type::RBRK, input[s +l]); l++;

    int block_l = 0;
    BlockUNode* block = parse_block(s +l, block_l);
    l += block_l;

    return new WhileUNode(cond, block);
}

IfUNode* parser::parse_if(int s, int& l)
{
    tassert(tok_type::IF, input[s]);
    tassert(tok_type::LBRK, input[s +1]);

    int cond_l = 0; l = 2;
    ExpressionTermUNode* cond = parse_expression_term(s +l, cond_l);
    l += cond_l;

    tassert(tok_type::RBRK, input[s +l]); l++;

    int block_l = 0;
    BlockUNode* true_block  = parse_block(s +l, block_l);
    BlockUNode* false_block = null;
    l += block_l; block_l = 0;

    if (input[s +l]->type == tok_type::ELSE)
        false_block = parse_block(s + (++l), block_l);
    l += block_l;

    return new IfUNode(cond, true_block, false_block);
}

ReturnUNode* parser::parse_return(int s, int& l)
{
    tassert(tok_type::RETURN, input[s]); l = 1;

    int retv_l = 0;
    ExpressionTermUNode* val = parse_expression_term(s +l, retv_l);
    l += retv_l;

    return new ReturnUNode(val);
}
