#include "ast/ast.h"

struct ast_operator *ast_init_operator(struct token tok)
{
    struct ast_operator *new = calloc(1, (sizeof(struct ast_operator)));
    if (tok.type == TOKEN_AND)
    {
        new->base.type = AST_AND;
    }
    else if (tok.type == TOKEN_OR)
    {
        new->base.type = AST_OR;
    }
    else
    {
        warnx("error wrong operator type\n");
        return NULL;
    }
    return new;
}

void ast_operator_cleanup(struct ast *ast)
{
    if (!ast)
        return;
    assert(ast->type == AST_AND || ast->type == AST_OR);
    struct ast_operator *curr = (void *)ast;
    if (curr->left)
        ast_cleanup(curr->left);
    if (curr->right)
        ast_cleanup(curr->right);

    free(ast);
}
