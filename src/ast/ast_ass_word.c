#include "ast/ast.h"

struct ast_ass_word *ast_init_ass_word(void)
{
    struct ast_ass_word *new = calloc(1, sizeof(struct ast_ass_word));

    new->base.type = AST_ASS_WORD;
    return new;
}

void ast_ass_word_cleanup(struct ast *ast)
{
    assert(ast);
    assert(ast->type == AST_ASS_WORD);

    struct ast_ass_word *curr = (void *)ast;

    if (curr->content)
        free(curr->content);
    free(ast);
}
