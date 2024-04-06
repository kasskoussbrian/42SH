#include "parser/parser.h"
#include "utils/utils.h"

static enum parser_status parse_for_inside(struct parser *parser,
                                           struct ast_for *ast_for)
{
    struct token curr = lexer_peek(parser->lexer);
    if (curr.type == TOKEN_SEMC)
    {
        lexer_pop(parser->lexer);
    }
    else
    {
        while (lexer_peek(parser->lexer).type == TOKEN_NEWLINE)
        {
            lexer_pop(parser->lexer);
        }
        int not_in = 0;
        if (lexer_peek(parser->lexer).type != TOKEN_IN)
        {
            not_in = 1;
        }
        else
        {
            lexer_pop(parser->lexer);
        }

        curr = lexer_peek(parser->lexer);
        while (is_word_or_reserved_word(curr) && !not_in)
        {
            ast_concat_word((void *)ast_for, &curr);
            lexer_pop(parser->lexer);
            curr = lexer_peek(parser->lexer);
        }
        if ((curr.type != TOKEN_SEMC && curr.type != TOKEN_NEWLINE) && !not_in)
        {
            parser->ast = (void *)ast_for;
            return PARSER_KO;
        }
        if (!not_in)
            lexer_pop(parser->lexer);
    }
    return PARSER_OK;
}
enum parser_status parse_for(struct parser *parser)
{
    struct token curr = lexer_peek(parser->lexer);
    if (curr.type != TOKEN_FOR)
        return PARSER_MAYBE;
    lexer_pop(parser->lexer);

    curr = lexer_peek(parser->lexer);
    if (!is_word_or_reserved_word(curr))
        return PARSER_KO;

    // set up the word in for
    struct ast_for *ast_for = ast_init_for();
    ast_for->word = my_strdup(curr.content);
    lexer_pop(parser->lexer);

    if (parse_for_inside(parser, ast_for) != PARSER_OK)
        return PARSER_KO;
    // check  the reules
    while (lexer_peek(parser->lexer).type == TOKEN_NEWLINE)
    {
        lexer_pop(parser->lexer);
    }

    if (lexer_peek(parser->lexer).type != TOKEN_DO)
    {
        parser->ast = (void *)ast_for;
        return PARSER_KO;
    }
    lexer_pop(parser->lexer);

    if (parse_compound_list(parser) != PARSER_OK)
    {
        ast_for->to_do = parser->ast;
        parser->ast = (void *)ast_for;
        return PARSER_KO;
    }

    ast_for->to_do = parser->ast;
    parser->ast = (void *)ast_for;
    if (lexer_peek(parser->lexer).type != TOKEN_DONE)
        return PARSER_KO;
    lexer_pop(parser->lexer);

    return PARSER_OK;
}
