#include "parser/parser.h"
/**
 ** \brief
    simple_command =
        prefix { prefix }
    |  {prefix} WORD {element}
 */
enum parser_status parse_simple_command(struct parser *parser)
{
    parser->ast = (struct ast *)ast_init_simple_cmd();

    int prefix_done = 0;

    enum parser_status status = parse_prefix(parser);
    while (status == PARSER_OK)
    {
        prefix_done += 1;
        status = parse_prefix(parser);
    }

    if (status == PARSER_KO)
        return status;

    struct token curr = lexer_peek(parser->lexer);

    /* First token of a command name can't be a reserved word. */
    if (curr.type != TOKEN_WORD)
    {
        if (prefix_done)
            return PARSER_OK;
        else
        {
            ast_cleanup(parser->ast);
            parser->ast = NULL;
            return PARSER_MAYBE;
        }
    }

    ast_concat_word(parser->ast, &curr);
    lexer_pop(parser->lexer);

    // hahahaha
    while (42)
    {
        enum parser_status status = parse_element(parser);
        if (status == PARSER_KO)
            return PARSER_KO;
        if (status == PARSER_MAYBE)
            return PARSER_OK;
    }

    return PARSER_OK;
}
