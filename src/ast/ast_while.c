#include "ast/ast.h"

struct ast_while *ast_init_while(void)
{
    struct ast_while *new = calloc(1, sizeof(struct ast_while));
    new->base.type = AST_WHILE;

    return new;
}

void ast_while_cleanup(struct ast *ast)
{
    assert(ast->type == AST_WHILE);

    struct ast_while *curr = (void *)ast;

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
