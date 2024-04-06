#include "parser/parser.h"

enum parser_status parse_prefix(struct parser *parser)
{
    struct token tok = lexer_peek(parser->lexer);
    if (tok.type == TOKEN_ASSIGNMENT_WORD)
    {
        struct ast_ass_word *new = ast_init_ass_word();
        new->content = my_strdup(tok.content);

        if (parser->ast->type == AST_SIMPLE_COMMAND)
        {
            struct ast_simple_cmd *simp_cmd = (void *)parser->ast;
            simp_cmd->ass_word_list =
                list_add(simp_cmd->ass_word_list, (void *)new);
            lexer_pop(parser->lexer);
            return PARSER_OK;
        }
        else if (parser->ast->type == AST_SHELL_COMMAND)
        {
            struct ast_shell_cmd *shell_cmd = (void *)parser->ast;
            shell_cmd->ass_word_list =
                list_add(shell_cmd->ass_word_list, (void *)new);
            lexer_pop(parser->lexer);
            return PARSER_OK;
        }
        warnx("Trying to add assignment word to wrong ast");
        return PARSER_KO;
    }

    return parse_redirection(parser);
}
