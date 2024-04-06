#include "parser/parser.h"

enum parser_status parse_case(struct parser *parser)
{
    if (lexer_peek(parser->lexer).type != TOKEN_CASE)
        return PARSER_MAYBE;
    lexer_pop(parser->lexer);
    struct token curr = lexer_peek(parser->lexer);
    if (!is_word_or_reserved_word(curr))
        return PARSER_KO;

    struct ast_case *toret = ast_init_case();
    toret->word = my_strdup(lexer_peek(parser->lexer).content);
    parser->ast = (void *)toret;

    lexer_pop(parser->lexer);
    while (lexer_peek(parser->lexer).type == TOKEN_NEWLINE)
    {
        lexer_pop(parser->lexer);
    }

    if (lexer_peek(parser->lexer).type != TOKEN_IN)
        return PARSER_KO;
    lexer_pop(parser->lexer);

    while (lexer_peek(parser->lexer).type == TOKEN_NEWLINE)
    {
        lexer_pop(parser->lexer);
    }

    if (parse_case_clause(parser) != PARSER_OK)
        return PARSER_KO;

    if (lexer_peek(parser->lexer).type != TOKEN_ESAC)
        return PARSER_KO;
    lexer_pop(parser->lexer);
    return PARSER_OK;
}

enum parser_status parse_case_clause(struct parser *parser)
{
    struct ast_case *toret = (void *)parser->ast;
    if (parse_case_item(parser) != PARSER_OK)
    {
        toret->item_case_list =
            list_add(toret->item_case_list, (void *)parser->ast);
        parser->ast = (void *)toret;
        return PARSER_KO;
    }
    toret->item_case_list =
        list_add(toret->item_case_list, (void *)parser->ast);
    while (lexer_peek(parser->lexer).type == TOKEN_DSEMC)
    {
        lexer_pop(parser->lexer);
        while (lexer_peek(parser->lexer).type == TOKEN_NEWLINE)
        {
            lexer_pop(parser->lexer);
        }
        parser->ast = NULL;
        enum parser_status status = parse_case_item(parser);
        if (parser->ast)
        {
            toret->item_case_list =
                list_add(toret->item_case_list, (void *)parser->ast);
        }
        if (status == PARSER_MAYBE)
            break;
        if (status != PARSER_OK)
        {
            parser->ast = (void *)toret;
            return PARSER_KO;
        }
    }
    if (lexer_peek(parser->lexer).type == TOKEN_DSEMC)
    {
        lexer_pop(parser->lexer);
    }
    while (lexer_peek(parser->lexer).type == TOKEN_NEWLINE)
    {
        lexer_pop(parser->lexer);
    }
    parser->ast = (void *)toret;
    return PARSER_OK;
}

enum parser_status parse_case_item(struct parser *parser)
{
    int popped = 0;
    if (lexer_peek(parser->lexer).type == TOKEN_PAR_L)
    {
        lexer_pop(parser->lexer);
        popped = 1;
    }
    struct token curr = lexer_peek(parser->lexer);
    struct ast_item_case *toret = ast_init_item_case();
    if (!is_word_or_reserved_word(curr))
    {
        parser->ast = (void *)toret;
        if (popped)
            return PARSER_KO;
        return PARSER_MAYBE;
    }
    ast_concat_word((void *)toret, &curr);
    lexer_pop(parser->lexer);

    while (lexer_peek(parser->lexer).type == TOKEN_PIPE)
    {
        lexer_pop(parser->lexer);
        if (!is_word_or_reserved_word(curr))
        {
            parser->ast = (void *)toret;
            return PARSER_KO;
        }
        curr = lexer_peek(parser->lexer);
        ast_concat_word((void *)toret, &curr);
        lexer_pop(parser->lexer);
    }

    if (lexer_peek(parser->lexer).type != TOKEN_PAR_R)
    {
        parser->ast = (void *)toret;
        return PARSER_KO;
    }
    lexer_pop(parser->lexer);
    while (lexer_peek(parser->lexer).type == TOKEN_NEWLINE)
    {
        lexer_pop(parser->lexer);
    }
    if (parse_compound_list(parser) != PARSER_OK)
    {
        toret->to_do = parser->ast;
        parser->ast = (void *)toret;
        return PARSER_KO;
    }
    toret->to_do = parser->ast;

    parser->ast = (void *)toret;
    return PARSER_OK;
}
