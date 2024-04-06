#include "ast/ast.h"

struct ast_until *ast_init_until(void)
{
    struct ast_until *new = calloc(1, sizeof(struct ast_until));
    new->base.type = AST_UNTIL;

    return new;
}

void ast_until_cleanup(struct ast *ast)
{
    assert(ast->type == AST_UNTIL);

    struct ast_until *curr = (void *)ast;

    if (curr->condition)
    {
        ast_cleanup(curr->condition);
    }
    if (curr->to_do)
    {
        ast_cleanup(curr->to_do);
    }
    free(ast);
}
