#include "parser/parser.h"
enum parser_status return_ko(struct parser *parser,
                             struct ast_shell_cmd *shell_cmd)
{
    shell_cmd->ast = parser->ast;
    parser->ast = (void *)shell_cmd;
    return PARSER_KO;
}

enum parser_status parse_shell_command(struct parser *parser)
{
    // return parse_rule_if(parser);
    // for redir parsing

    struct ast_shell_cmd *shell_cmd = ast_init_shell_cmd();

    // command block
    enum parser_status status = PARSER_KO;

    if (lexer_peek(parser->lexer).type == TOKEN_LBRACE)
    {
        lexer_pop(parser->lexer);
        status = parse_compound_list(parser);

        if (status != PARSER_OK
            || lexer_peek(parser->lexer).type != TOKEN_RBRACE)
        {
            return return_ko(parser, shell_cmd);
        }
        lexer_pop(parser->lexer);
    }
    /*
    if (status !=PARSER_OK)
    {
        status = parse_subshell(parser);
        if (status == PARSER_KO)
        {
            return return_ko(parser,shell_cmd);
        }
    }
    */
    // for rule
    if (status != PARSER_OK)
    {
        status = parse_for(parser);
        if (status == PARSER_KO)
        {
            return return_ko(parser, shell_cmd);
        }
    }

    // while rule
    if (status != PARSER_OK)
    {
        status = parse_while(parser);
        if (status == PARSER_KO)
        {
            return return_ko(parser, shell_cmd);
        }
    }
    // until rule
    if (status != PARSER_OK)
    {
        status = parse_until(parser);
        if (status == PARSER_KO)
        {
            return return_ko(parser, shell_cmd);
        }
    }

    // rule case
    if (status != PARSER_OK)
    {
        status = parse_case(parser);
        if (status == PARSER_KO)
        {
            return return_ko(parser, shell_cmd);
        }
    }

    // if  rule
    if (status != PARSER_OK)
    {
        status = parse_rule_if(parser);
        if (status == PARSER_KO)
        {
            return return_ko(parser, shell_cmd);
        }
    }
    if (status == PARSER_MAYBE)
    {
        ast_cleanup((void *)shell_cmd);
        return PARSER_MAYBE;
    }

    // put the resulted ast in the shell cmd
    shell_cmd->ast = parser->ast;
    // Put the shell cmd ast in parser->ast so that parse redirection
    parser->ast = (void *)shell_cmd;
    // parser_redir
    status = parse_redirection(parser);
    while (status == PARSER_OK)
    {
        status = parse_redirection(parser);
    }

    return ((status == PARSER_KO) ? PARSER_KO : PARSER_OK);
}
