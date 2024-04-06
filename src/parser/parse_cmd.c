#include "parser/parser.h"

/**
 * \brief Parse a command
 *
 * command =
 *    simple_command
 *    | shell_command {redirection}
 *    |funcdec {redirection}
 *    ;
 *
 */
enum parser_status parse_command(struct parser *parser)
{
    // save token for after in case of conflicting types
    struct token tok;
    tok.type = lexer_peek(parser->lexer).type;
    tok.content = my_strdup(lexer_peek(parser->lexer).content);

    // SIMPLE COMMAND
    enum parser_status status = parse_simple_command(parser);
    if (status != PARSER_MAYBE)
    {
        if (lexer_peek(parser->lexer).type != TOKEN_PAR_L)
        {
            free(tok.content);
            return status;
        }
        ast_cleanup(parser->ast);
        parser->ast = NULL;
        status = PARSER_MAYBE;
    }

    // SHELL COMMAND
    status = parse_shell_command(parser);
    if (status != PARSER_MAYBE)
    {
        free(tok.content);
        return status;
    }

    // FUNCTION
    status = parse_func(parser, &tok);
    if (status != PARSER_OK)
    {
        free(tok.content);
        return PARSER_KO;
    }

    free(tok.content);
    return PARSER_OK;
}
