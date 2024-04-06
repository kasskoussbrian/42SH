#include "ast/ast.h"

struct ast_item_case *ast_init_item_case(void)
{
    struct ast_item_case *toret = calloc(1, sizeof(struct ast_item_case));
    toret->base.type = AST_ITEM_CASE;
    toret->argv = calloc(1, sizeof(char *));
    return toret;
}

void ast_item_case_cleanup(struct ast *ast)
{
    assert(ast);
    assert(ast->type == AST_ITEM_CASE);
    struct ast_item_case *curr = (void *)ast;
    if (curr->argv)
    {
        for (char **arg = curr->argv; *arg; arg++)
        {
            free(*arg);
        }
    }
    free(curr->argv);
    ast_cleanup(curr->to_do);
    free(ast);
}
