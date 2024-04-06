#include "execution/execution.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

typedef int (*evaluate_ast_fun)(struct exec_args exec_args);

static evaluate_ast_fun get_ast_evaluate_fun[] = {
    [AST_REDIR] = NULL,
    [AST_PIPELINE] = &evaluate_pipe_line,
    [AST_IF] = &evaluate_if,
    [AST_SIMPLE_COMMAND] = &evaluate_simple_cmd,
    [AST_LIST] = &evaluate_list,
    [AST_SHELL_COMMAND] = &evaluate_shell_cmd,
    [AST_NOT] = &evaluate_not,
    [AST_WHILE] = &evaluate_while,
    [AST_UNTIL] = &evaluate_until,
    [AST_AND] = &evaluate_operator,
    [AST_OR] = &evaluate_operator,
    [AST_FOR] = &evaluate_for,
    [AST_FUNC] = &evaluate_func,
    [AST_CASE] = &evaluate_case,
    [AST_NOT_CODED] = NULL,
};

/*
 * BRIEF:
 * Evalue the ast tree;
 * return 'the return value' on success | -1 on fail
 */
int evaluate_tree(struct exec_args exec_args)
{
    if (!exec_args.ast)
    {
        // value returned in case nothing was executed
        return EXIT_SUCCESS;
    }

    // no need to end the programm if the tree is NULL just dont excecute the
    // line

    evaluate_ast_fun eval_ast = get_ast_evaluate_fun[exec_args.ast->type];

    if (!eval_ast)
    {
        fprintf(stderr, "AST Evaluation not implemented yet\n");
        return -1;
    }

    return eval_ast(exec_args);
}
