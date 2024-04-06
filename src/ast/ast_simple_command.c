#include <string.h>

#include "ast/ast.h"
#include "ast/list.h"

struct ast_simple_cmd *ast_init_simple_cmd(void)
{
    struct ast_simple_cmd *new = calloc(1, sizeof(*new));
    if (!new)
        return NULL;

    new->base.type = AST_SIMPLE_COMMAND;
    new->argc = 0;
    new->argv = calloc(1, sizeof(char *));
    new->argv[0] = NULL;

    return new;
}

int ast_concat_word(struct ast *ast, struct token *tok)
{
    assert(ast);
    assert(ast->type == AST_SIMPLE_COMMAND || ast->type == AST_FOR
           || ast->type == AST_ITEM_CASE);

    /* TODO: ASSERT THAT THE WORD IS A WORD OR RESERVED WORD */

    if (ast->type == AST_SIMPLE_COMMAND)
    {
        struct ast_simple_cmd *curr = (struct ast_simple_cmd *)ast;

        curr->argc++;
        curr->argv = realloc(curr->argv, (curr->argc + 1) * sizeof(char *));

        if (!curr->argv)
            return -1;

        curr->argv[curr->argc - 1] = strdup(tok->content);
        curr->argv[curr->argc] = NULL;
    }
    else if (ast->type == AST_FOR)
    {
        struct ast_for *curr = (struct ast_for *)ast;

        curr->argc++;
        curr->argv = realloc(curr->argv, (curr->argc + 1) * sizeof(char *));

        if (!curr->argv)
            return -1;

        curr->argv[curr->argc - 1] = strdup(tok->content);
        curr->argv[curr->argc] = NULL;
    }
    else
    {
        struct ast_item_case *curr = (struct ast_item_case *)ast;

        curr->argc++;
        curr->argv = realloc(curr->argv, (curr->argc + 1) * sizeof(char *));

        if (!curr->argv)
            return -1;

        curr->argv[curr->argc - 1] = strdup(tok->content);
        curr->argv[curr->argc] = NULL;
    }

    return 0;
}

void ast_simple_cmd_cleanup(struct ast *ast)
{
    assert(ast);
    assert(ast->type == AST_SIMPLE_COMMAND);

    struct ast_simple_cmd *curr = (struct ast_simple_cmd *)ast;

    if (curr->argv)
    {
        for (char **arg = curr->argv; *arg; arg++)
        {
            free(*arg);
        }
    }
    list_cleanup(curr->redir_list);
    list_cleanup(curr->ass_word_list);
    free(curr->argv);
    free(ast);
}
