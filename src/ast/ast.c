#include "ast/ast.h"

typedef void (*free_ast_fun)(struct ast *ast);

static free_ast_fun get_free_ast_fun[] = {
    [AST_SIMPLE_COMMAND] = &ast_simple_cmd_cleanup,
    [AST_LIST] = &ast_list_cleanup,
    [AST_IF] = &ast_if_cleanup,
    [AST_SHELL_COMMAND] = &ast_shell_cmd_cleanup,
    [AST_REDIR] = &ast_redir_cleanup,
    [AST_PIPELINE] = &ast_pipeline_cleanup,
    [AST_WHILE] = &ast_while_cleanup,
    [AST_UNTIL] = &ast_until_cleanup,
    [AST_NOT] = &ast_not_cleanup,
    [AST_AND] = &ast_operator_cleanup,
    [AST_FOR] = &ast_for_cleanup,
    [AST_OR] = &ast_operator_cleanup,
    [AST_ASS_WORD] = &ast_ass_word_cleanup,
    [AST_FUNC] = &ast_func_cleanup,
    [AST_CASE] = &ast_case_cleanup,
    [AST_SUBSHELL] = &ast_subshell_cleanup,
    [AST_ITEM_CASE] = &ast_item_case_cleanup
};

void ast_cleanup(struct ast *ast)
{
    if (ast == NULL)
        return;

    free_ast_fun cleanup_fun = get_free_ast_fun[ast->type];
    cleanup_fun(ast);
}

//////////
//////////
