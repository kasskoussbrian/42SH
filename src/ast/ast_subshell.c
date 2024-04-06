#include "ast/ast.h"

struct ast_subshell *ast_init_subshell(void)
{
    struct ast_subshell *new = calloc(1, sizeof(struct ast_subshell));
    new->base.type = AST_SUBSHELL;
    return new;
}

void ast_subshell_cleanup(struct ast *ast)
{
    if (!ast)
        return;
    struct ast_subshell *curr = (void *)ast;
    ast_cleanup(curr->to_do);
    free(ast);
}
