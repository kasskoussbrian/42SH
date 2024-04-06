#include "ast/ast.h"

struct ast_redir *ast_init_redir(void)
{
    struct ast_redir *new = calloc(1, sizeof(struct ast_redir));
    if (!new)
        return NULL;
    new->base.type = AST_REDIR;
    new->IO_nb = -1;
    return new;
}

void ast_redir_cleanup(struct ast *ast)
{
    if (ast->type != AST_REDIR)
    {
        return;
    }
    struct ast_redir *tofree = (void *)ast;
    if (!ast)
        return;

    if (tofree->filename)
    {
        free(tofree->filename);
    }

    free(tofree);
}
