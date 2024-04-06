#include "parser/parser.h"

/**
 * \brief Parse a list
 *
 * list =
 *    and_or { ';' and_or } [ ';' ] ;
 *
 */
enum parser_status parse_list(struct parser *parser)
{
    enum parser_status status = parse_and_or(parser);

    if (status == PARSER_KO)
        return PARSER_KO;

    struct ast_list *root = ast_init_list();
    root->list = list_add(root->list, parser->ast);

    struct token curr = {
        .type = TOKEN_WORD,
        .content = NULL,
    };

    // Lol 42
    while (42)
    {
        curr = lexer_peek(parser->lexer);
        if (curr.type != TOKEN_SEMC)
            break;
        lexer_pop(parser->lexer);
        curr = lexer_peek(parser->lexer);
        if (curr.type == TOKEN_EOF || curr.type == TOKEN_NEWLINE)
        {
            parser->ast = (void *)root;
            return PARSER_OK;
        }

        status = parse_and_or(parser);
        if (status == PARSER_KO)
        {
            parser->ast = (void *)root;
            return PARSER_KO;
        }
        root->list = list_add(root->list, parser->ast);
    }

    curr = lexer_peek(parser->lexer);
    if (curr.type == TOKEN_SEMC)
    {
        lexer_pop(parser->lexer);
    }

    parser->ast = (struct ast *)root;
    /* Returns new root node through parser->ast. */
    return PARSER_OK;
}
