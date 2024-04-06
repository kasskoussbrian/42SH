#include "parser/parser.h"

enum parser_status parse_func(struct parser *parser, struct token *tok)
{
    // word
    if (tok->type != TOKEN_WORD)
        return PARSER_MAYBE;

    struct ast_func *new = ast_init_func();
    new->name = my_strdup(tok->content);

    // ' ( '
    if (lexer_peek(parser->lexer).type != TOKEN_PAR_L)
    {
        parser->ast = (void *)new;
        return PARSER_KO;
    }
    lexer_pop(parser->lexer);
    //' ) '
    if (lexer_peek(parser->lexer).type != TOKEN_PAR_R)
    {
        parser->ast = (void *)new;
        return PARSER_KO;
    }
    lexer_pop(parser->lexer);

    // ' { \n } '
    while (lexer_peek(parser->lexer).type == TOKEN_NEWLINE)
    {
        lexer_pop(parser->lexer);
    }

    // compound_list
    if (parse_shell_command(parser) != PARSER_OK)
    {
        new->function = (void *)parser->ast;
        parser->ast = (void *)new;
        return PARSER_KO;
    }

    new->function = (void *)parser->ast;
    parser->ast = (void *)new;

    // TO_DO:
    // PARSE REDIR
    //.............
    // END
    enum parser_status status = parse_redirection(parser);
    while (status == PARSER_OK)
    {
        status = parse_redirection(parser);
    }

    if (status == PARSER_KO)
        return PARSER_KO;

    return PARSER_OK;
}
