#include "ast/ast.h"

struct ast_func *ast_init_func(void)
{
    struct ast_func *new = calloc(1, sizeof(struct ast_func));
    new->base.type = AST_FUNC;

    return new;
}

void ast_func_cleanup(struct ast *ast)
{
    assert(ast);
    assert(ast->type == AST_FUNC);

    struct ast_func *curr = (void *)ast;

    if (curr->function)
    {
        ast_cleanup(curr->function);
    }
    list_cleanup(curr->redir_list);
    free(curr->name);
    free(ast);
}
