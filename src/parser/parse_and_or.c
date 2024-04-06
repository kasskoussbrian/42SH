#include "parser/parser.h"
/**
 * \brief Parse an and_or
 *
 * and_or =
 *    pipeline
 *
 */
enum parser_status parse_and_or(struct parser *parser)
{
    if (parse_pipeline(parser) == PARSER_KO)
    {
        return PARSER_KO;
    }
    struct token curr = lexer_peek(parser->lexer);
    while (curr.type == TOKEN_OR || curr.type == TOKEN_AND)
    {
        lexer_pop(parser->lexer);
        // Any mistakes form now one should be taken as an exit code

        // create and or or type
        struct ast_operator *op = ast_init_operator(curr);
        op->left = parser->ast;
        while (lexer_peek(parser->lexer).type == TOKEN_NEWLINE)
        {
            lexer_pop(parser->lexer);
        }
        if (parse_pipeline(parser) == PARSER_KO)
        {
            op->right = parser->ast;
            parser->ast = (void *)op;
            return PARSER_KO;
        }
        op->right = parser->ast;

        parser->ast = (void *)op;
        curr = lexer_peek(parser->lexer);
    }
    return PARSER_OK;
}
