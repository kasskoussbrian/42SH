#include "ast/ast.h"

struct ast_pipeline *ast_init_pipeline(void)
{
    struct ast_pipeline *new = calloc(1, sizeof(struct ast_pipeline));

    new->base.type = AST_PIPELINE;
    return new;
}

void ast_pipeline_cleanup(struct ast *ast)
{
    if (!ast)
        return;
    struct ast_pipeline *curr = (void *)ast;
    if (curr->left)
    {
        ast_cleanup(curr->left);
    }

    if (curr->right)
    {
        ast_cleanup(curr->right);
    }

    free(ast);
}
