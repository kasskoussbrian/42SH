#include "parser.h"

enum parser_status parse_until(struct parser *parser)
{
    if (lexer_peek(parser->lexer).type != TOKEN_UNTIL)
    {
        return PARSER_MAYBE;
    }
    lexer_pop(parser->lexer);

    if (parse_compound_list(parser) == PARSER_KO)
    {
        return PARSER_KO;
    }

    struct ast_until *curr = ast_init_until();
    curr->condition = parser->ast;

    if (lexer_peek(parser->lexer).type != TOKEN_DO)
    {
        parser->ast = (void *)curr;
        return PARSER_KO;
    }
    lexer_pop(parser->lexer);

    if (parse_compound_list(parser) == PARSER_KO)
    {
        curr->to_do = parser->ast;
        parser->ast = (void *)curr;
        return PARSER_KO;
    }
    curr->to_do = parser->ast;
    parser->ast = (void *)curr;

    if (lexer_peek(parser->lexer).type != TOKEN_DONE)
    {
        return PARSER_KO;
    }
    lexer_pop(parser->lexer);

    return PARSER_OK;
}
