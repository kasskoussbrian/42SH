#include "ast/ast.h"

struct ast_for *ast_init_for(void)
{
    struct ast_for *new = calloc(1, sizeof(struct ast_for));

    new->base.type = AST_FOR;
    new->argv = calloc(1, sizeof(char *));
    new->argv[0] = NULL;
    new->argc = 0;

    new->word = NULL;
    return new;
}

void ast_for_cleanup(struct ast *ast)
{
    assert(ast);
    assert(ast->type == AST_FOR);

    struct ast_for *curr = (void *)ast;
    if (curr->argv)
    {
        for (char **arg = curr->argv; *arg; arg++)
        {
            free(*arg);
        }
    }
    if (curr->word)
        free(curr->word);

    if (curr->to_do)
        ast_cleanup(curr->to_do);
    free(curr->argv);
    free(ast);
}
