#include <stdlib.h>

#include "ast/ast.h"

struct ast_case *ast_init_case(void)
{
    struct ast_case *toret = calloc(1, sizeof(struct ast_case));

    toret->base.type = AST_CASE;
    return toret;
}

void ast_case_cleanup(struct ast *ast)
{
    assert(ast->type == AST_CASE);

    struct ast_case *tofree = (void *)ast;
    if (tofree->item_case_list)
    {
        list_cleanup(tofree->item_case_list);
    }
    if (tofree->word)
    {
        free(tofree->word);
    }
    free(ast);
}
