#include "ast/ast.h"

struct ast_not *ast_init_not(void)
{
    struct ast_not *new = calloc(1, sizeof(struct ast_not));
    new->base.type = AST_NOT;
    return new;
}

void ast_not_cleanup(struct ast *ast)
{
    struct ast_not *curr = (void *)ast;

    if (!curr)
    {
        return;
    }
    if (curr->ast)
    {
        ast_cleanup(curr->ast);
    }
    free(ast);
}
