#include "parser/parser.h"

static enum redir_type give_redir_type(struct token token)
{
    if (token.type == TOKEN_REDIR_L)
    {
        return REDIR_LEFT;
    }
    if (token.type == TOKEN_REDIR_R)
    {
        return REDIR_RIGHT;
    }

    if (token.type == TOKEN_REDIR_R_PIPE)
    {
        return REDIR_RIGHT_PIPE;
    }

    if (token.type == TOKEN_REDIR_APPEND)
    {
        return REDIR_RIGHT_RIGHT;
    }

    if (token.type == TOKEN_REDIR_DUP_INPUT)
    {
        return REDIR_LEFT_UPSAND;
    }

    if (token.type == TOKEN_REDIR_DUP_OUTPUT)
    {
        return REDIR_RIGHT_UPSAND;
    }

    if (token.type == TOKEN_REDIR_OPEN_FILE)
    {
        return REDIR_LEFT_RIGHT;
    }

    return REDIR_NOT_FOUND;
}

static int is_redir(struct token token)
{
    if (token.type == TOKEN_REDIR_L)
    {
        return 1;
    }
    if (token.type == TOKEN_REDIR_R)
    {
        return 1;
    }

    if (token.type == TOKEN_REDIR_R_PIPE)
    {
        return 1;
    }

    if (token.type == TOKEN_REDIR_APPEND)
    {
        return 1;
    }

    if (token.type == TOKEN_REDIR_DUP_INPUT)
    {
        return 1;
    }

    if (token.type == TOKEN_REDIR_DUP_OUTPUT)
    {
        return 1;
    }

    if (token.type == TOKEN_REDIR_OPEN_FILE)
    {
        return 1;
    }

    return 0;
}

enum parser_status parse_redirection(struct parser *parser)
{
    struct token curr = lexer_peek(parser->lexer);

    struct ast_redir *to_add = ast_init_redir();
    int popped = 0;
    // io_numbe
    if (curr.type == TOKEN_IO_NUMBER)
    {
        to_add->IO_nb = curr.value;
        lexer_pop(parser->lexer);
        popped = 1;
    }

    // it is a redir tok
    curr = lexer_peek(parser->lexer);
    if (!is_redir(lexer_peek(parser->lexer)))
    {
        ast_redir_cleanup((void *)to_add);
        if (popped == 0)
            return PARSER_MAYBE;
        return PARSER_KO;
    }
    to_add->redir_type = give_redir_type(curr);
    lexer_pop(parser->lexer);

    // this is a word
    curr = lexer_peek(parser->lexer);
    if (!is_word_or_reserved_word(curr))
    {
        ast_redir_cleanup((void *)to_add);
        return PARSER_KO;
    }

    to_add->filename = my_strdup(curr.content);
    to_add->fd_word = curr.value;

    lexer_pop(parser->lexer);

    // add the ast_redir to the list
    if (parser->ast->type == AST_SIMPLE_COMMAND)
    {
        struct ast_simple_cmd *simp_cmd = (void *)parser->ast;
        simp_cmd->redir_list = list_add(simp_cmd->redir_list, (void *)to_add);
        return PARSER_OK;
    }
    else if (parser->ast->type == AST_SHELL_COMMAND)
    {
        struct ast_shell_cmd *shell_cmd = (void *)parser->ast;
        shell_cmd->redir_list = list_add(shell_cmd->redir_list, (void *)to_add);
        return PARSER_OK;
    }
    else if (parser->ast->type == AST_FUNC)
    {
        struct ast_func *func = (void *)parser->ast;
        func->redir_list = list_add(func->redir_list, (void *)to_add);
        return PARSER_OK;
    }

    warn("Trying to add a redirection to an unknown ast.\n");
    return PARSER_KO;
}
