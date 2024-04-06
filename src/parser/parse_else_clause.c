#include "parser/parser.h"

/*
 * else_clause =
 *      'else' compound_list
 *  |   'elif' compound_list 'then' compound_list [else_clause]
 *
 */
enum parser_status parse_else_clause(struct parser *parser)
{
    /* We pop it because the caller ensured that next token was either a else or
     * a elif */
    struct token curr = lexer_peek(parser->lexer);

    if (curr.type == TOKEN_ELSE)
    {
        lexer_pop(parser->lexer);
        return parse_compound_list(parser);
    }
    if (curr.type != TOKEN_ELIF)
    {
        return PARSER_KO;
    }
    lexer_pop(parser->lexer);

    if (parse_compound_list(parser) == PARSER_KO)
    {
        return PARSER_KO;
    }

    struct ast_if *ast_if = ast_init_if();
    ast_if->condition = (struct ast_list *)parser->ast;

    // then
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

    // else
    if (parse_else_clause(parser) != PARSER_OK)
    {
        parser->ast = (void *)ast_if;
        return PARSER_KO;
    }

    ast_if->else_body = (struct ast *)parser->ast;

    parser->ast = (struct ast *)ast_if;

    return PARSER_OK;
}
