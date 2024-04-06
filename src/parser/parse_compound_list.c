#include "parser/parser.h"
/*
    compound_list =
    {'\n'} and_or { ( ';' | '\n' ) {'\n'} and_or } [';'] {'\n'} ;
*/

enum parser_status parse_compound_list(struct parser *parser)
{
    // skip first new lines
    while (lexer_peek(parser->lexer).type == TOKEN_NEWLINE)
    {
        lexer_pop(parser->lexer);
    }

    // parse_and_or
    if (parse_and_or(parser) != PARSER_OK)
    {
        return PARSER_KO;
    }
    struct ast_list *ast_list = ast_init_list();
    ast_list->list = list_add(ast_list->list, (struct ast *)parser->ast);

    // { ( ';' | '\n' ) {'\n'} and_or }
    struct token curr = lexer_peek(parser->lexer);
    while (curr.type == TOKEN_SEMC || curr.type == TOKEN_NEWLINE)
    {
        lexer_pop(parser->lexer);
        curr = lexer_peek(parser->lexer);
        while (curr.type == TOKEN_NEWLINE)
        {
            lexer_pop(parser->lexer);
            curr = lexer_peek(parser->lexer);
        }
        if (parse_and_or(parser) != PARSER_OK)
        {
            break;
        }
        ast_list->list = list_add(ast_list->list, (struct ast *)parser->ast);
        curr = lexer_peek(parser->lexer);
    }
    curr = lexer_peek(parser->lexer);

    while (curr.type == TOKEN_NEWLINE)
    {
        lexer_pop(parser->lexer);
        curr = lexer_peek(parser->lexer);
    }
    parser->ast = (struct ast *)ast_list;

    return PARSER_OK;
}
