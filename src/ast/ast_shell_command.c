#include "ast/ast.h"

struct ast_shell_cmd *ast_init_shell_cmd(void)
{
    struct ast_shell_cmd *new = calloc(1, sizeof(struct ast_shell_cmd));
    if (!new)
        return NULL;
    new->base.type = AST_SHELL_COMMAND;
    return new;
}

void ast_shell_cmd_cleanup(struct ast *ast)
{
    if (!ast)
        return;

    struct ast_shell_cmd *tofree = (void *)ast;
    list_cleanup(tofree->redir_list);
    list_cleanup(tofree->ass_word_list);
    ast_cleanup(tofree->ast);
    free(ast);
}
