#include "parser/parser.h"

struct parser *parser_init(struct cli_args args)
{
    struct parser *parser = calloc(1, sizeof(*parser));
    if (!parser)
    {
        warnx("Memory exhausted.");
        return NULL;
    }
    parser->lexer = lexer_init(args);
    if (!parser->lexer)
    {
        parser_cleanup(parser);
        return NULL;
    }
    parser->ast = NULL;
    return parser;
}

void parser_cleanup(struct parser *parser)
{
    if (!parser)
        return;

    lexer_cleanup(parser->lexer);
    ast_cleanup(parser->ast);

    free(parser);
}

/**
 * \brief Parse an input
 *
 * input =
 *    list '\n'
 *  | list EOF
 *  | '\n'
 *  | EOF
 *
 */
enum parser_status parse_input(struct parser *parser)
{
    struct token curr = lexer_peek(parser->lexer);
    if (curr.type == TOKEN_EOF || curr.type == TOKEN_NEWLINE)
    {
        lexer_pop(parser->lexer);
        parser->ast = NULL;
        return PARSER_OK;
    }

    if (parse_list(parser) == PARSER_OK)
    {
        curr = lexer_peek(parser->lexer);
        if (curr.type != TOKEN_EOF && curr.type != TOKEN_NEWLINE)
        {
            return PARSER_KO;
        }
        return PARSER_OK;
    }
    return PARSER_KO;
}

int is_word_or_reserved_word(struct token tok)
{
    switch (tok.type)
    {
    case TOKEN_WORD:
    case TOKEN_NEGATION:
    case TOKEN_DO:
    case TOKEN_DONE:
    case TOKEN_ELIF:
    case TOKEN_ELSE:
    case TOKEN_FI:
    case TOKEN_FOR:
    case TOKEN_IF:
    case TOKEN_IN:
    case TOKEN_THEN:
    case TOKEN_UNTIL:
    case TOKEN_WHILE:
    case TOKEN_LBRACE:
    case TOKEN_RBRACE:
        return 1;
    default:
        return 0;
    }
}
