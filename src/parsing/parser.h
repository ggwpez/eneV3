#ifndef PARSER_H
#define PARSER_H

#include <vector>

#include "lexing/token.hpp"
#include "parsing/ast/uast.h"

class parser
{
public:
    parser(std::vector<tok*>* input);
    uast* parse();

private:
    uast*                   parse_program_statement(int s, int& l);
    uast*                   parse_single_ident_program(int s, int& l);
    uast*                   parse_single_ident_block(int s, int& l);
    uast*                   parse_single_ident_expression(int s, int& l);
    uast*                   parse_namespace_block(int s, int& l);
    uast*                   parse_asm(int s, int& l);
    uast*                   parse_struct(int s, int& l);
    VariableUNode*          parse_variable_definition(int s, int& l);
    uast*                   parse_function_definition(int s, int& l);
    FunctionCallUNode*      parse_function_call(int s, int& l);
    AnomymousCallUNode*     parse_anonymous_call(int s, int& l);
    IdentNode*              parse_ident(int s, int& l);
    NumNode*                parse_number(int s, int& l);
    AssignUNode*            parse_assign(int s, int& l);
    StringNode*             parse_string(int s, int& l);
    WhileUNode*             parse_while(int s, int& l);
    IfUNode*                parse_if(int s, int& l);
    ReturnUNode*            parse_return(int s, int& l);
    ProgramUNode*           parse_program(int s, int& l);
    ArgUNode*               parse_arg(int s, int& l);
    ListArgUNode*           parse_arg_list(int s, int& l);
    ListTypeUNode*          parse_type_list(int s, int& l);
    ListUNode*              parse_list(int s, int& l);
    BlockUNode*             parse_block(int s, int& l);
    uast*                   parse_block_statement(int s, int& l);
    uast*                   parse_expression(int s, int& l);
    ExpressionTermUNode*    parse_expression_term(int s, int& l);
    TypeUNode* parse_type(int s, int& l);
    TypeUNode* parse_fptr_type(int s, int& l);

    size_t length;
    std::vector<tok*> input;
};

#endif // PARSER_H
