#include <assert.h>

#include "ast/ast.h"

struct ast_if *ast_init_if(void)
{
    struct ast_if *new = calloc(1, sizeof(*new));
    if (!new)
        return NULL;

    new->base.type = AST_IF;
    return new;
}

void ast_if_cleanup(struct ast *ast)
{
    assert(ast);
    assert(ast->type == AST_IF);

    struct ast_if *curr = (struct ast_if *)ast;

    ast_cleanup((void *)curr->condition);
    ast_cleanup((void *)curr->then_body);
    ast_cleanup((void *)curr->else_body);

    free(curr);
}
