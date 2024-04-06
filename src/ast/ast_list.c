#include "ast/ast.h"
#include "ast/list.h"

struct ast_list *ast_init_list(void)
{
    struct ast_list *new = calloc(1, sizeof(*new));
    if (!new)
        return NULL;

    new->base.type = AST_LIST;
    return new;
}

void ast_list_cleanup(struct ast *ast)
{
    assert(ast);
    assert(ast->type == AST_LIST);

    struct ast_list *curr = (struct ast_list *)ast;

    list_cleanup(curr->list);

    free(curr);
}
