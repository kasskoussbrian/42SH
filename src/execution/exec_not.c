#include "execution/execution.h"

int evaluate_not(struct exec_args exec_args)
{
    struct ast_not *curr = (void *)exec_args.ast;
    exec_args.ast = curr->ast;
    int status = evaluate_tree(exec_args);
    if (*exec_args.exit || *exec_args.continue_val
        || *exec_args.break_val) // If found exit builtin
        return status;
    return !status;
}
