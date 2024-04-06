#include "parser/parser.h"

/**
 * \brief Parse a pipeline
 *
 * pipeline =
 *    command
 *
 */
enum parser_status parse_pipeline(struct parser *parser)
{
    // check if theres a negation
    int neg = 0;
    if (lexer_peek(parser->lexer).type == TOKEN_NEGATION)
    {
        neg = 1;
        lexer_pop(parser->lexer);
    }
    /////
    if (parse_command(parser) != PARSER_OK)
    {
        return PARSER_KO;
    }
    while (lexer_peek(parser->lexer).type == TOKEN_PIPE)
    {
        lexer_pop(parser->lexer);

        // put actual tree on left pipe
        struct ast_pipeline *pipe = ast_init_pipeline();
        pipe->left = parser->ast;

        while (lexer_peek(parser->lexer).type == TOKEN_NEWLINE)
        {
            lexer_pop(parser->lexer);
        }

        if (parse_command(parser) != PARSER_OK)
        {
            parser->ast = (void *)pipe;
            return PARSER_KO;
        }
        pipe->right = parser->ast;
        parser->ast = (void *)pipe;
    }

    if (neg)
    {
        struct ast_not *nega = ast_init_not();
        nega->ast = parser->ast;
        parser->ast = (void *)nega;
    }

    return PARSER_OK;
}
