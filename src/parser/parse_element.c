#include "parser/parser.h"

/**
 * \brief Parse an element
 *
 * element =
 *    WORD
 *
 */
enum parser_status parse_element(struct parser *parser)
{
    /*
    if (parse_redir(parser) == PARSER_OK)
    {
        return PARSER_OK;
    }
    */

    struct token curr = lexer_peek(parser->lexer);
    if (!is_word_or_reserved_word(curr))
    {
        return parse_redirection(parser);
    }

    ast_concat_word(parser->ast, &curr);
    lexer_pop(parser->lexer);

    return PARSER_OK;
}
