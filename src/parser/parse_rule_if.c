#include "parser/parser.h"
/*
 * rule_if =
 *  if' compound_list 'then' compound_list [else_clause] 'fi' ;
 */
enum parser_status parse_rule_if(struct parser *parser)
{
    // START--IF NODE
    if (lexer_peek(parser->lexer).type != TOKEN_IF)
    {
        return PARSER_MAYBE;
    }
    lexer_pop(parser->lexer);

    if (parse_compound_list(parser) != PARSER_OK)
    {
        return PARSER_KO;
    }

    struct ast_if *ast_if = ast_init_if();
    ast_if->condition = (struct ast_list *)parser->ast;
    // END--IF NODE

    // START--IF NODE
    if (lexer_peek(parser->lexer).type != TOKEN_THEN)
    {
        parser->ast = (void *)ast_if;
        return PARSER_KO;
    }
    lexer_pop(parser->lexer);

    if (parse_compound_list(parser) != PARSER_OK)
    {
        parser->ast = (void *)ast_if;
        return PARSER_KO;
    }
    ast_if->then_body = (struct ast_list *)parser->ast;

    // END--THEN NODE
    if (parse_else_clause(parser) == PARSER_OK)
    {
        ast_if->else_body = (struct ast *)parser->ast;
    }

    // END--ELSE NODE
    if (lexer_peek(parser->lexer).type != TOKEN_FI)
    {
        parser->ast = (void *)ast_if;
        return PARSER_KO;
    }
    lexer_pop(parser->lexer);

    // put the result in the main ast (the one in the parser struct)
    parser->ast = (struct ast *)ast_if;

    return PARSER_OK;
}
