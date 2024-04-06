#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#include "execution/execution.h"

int evaluate_if(struct exec_args exec_args)
{
    assert(exec_args.ast);
    assert(exec_args.ast->type == AST_IF);

    struct ast_if *curr = (struct ast_if *)exec_args.ast;

    assert(curr->condition);
    assert(curr->then_body);

    exec_args.ast = (void *)curr->condition;
    int status_cond = evaluate_tree(exec_args);
    if (*exec_args.exit || *exec_args.continue_val
        || *exec_args.break_val) // If found exit builtin
        return status_cond;

    if (status_cond == EXIT_SUCCESS)
    {
        exec_args.ast = (void *)curr->then_body;
        return evaluate_tree(exec_args);
    }
    else if (curr->else_body)
    {
        exec_args.ast = curr->else_body;
        return evaluate_tree(exec_args);
    }
    return EXIT_SUCCESS;
}
