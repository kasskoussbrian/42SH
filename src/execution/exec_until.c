#include "execution/execution.h"

int evaluate_until(struct exec_args exec_args)
{
    assert(exec_args.ast);
    assert(exec_args.ast->type == AST_UNTIL);

    struct ast_until *curr = (struct ast_until *)exec_args.ast;

    if (!curr->is_base && !*exec_args.is_in_base)
    {
        curr->is_base = 1;
        *exec_args.is_in_base = 1;
    }
    int status = EXIT_SUCCESS;
    int condition = EXIT_SUCCESS;
    while (42)
    {
        exec_args.ast = curr->condition;
        condition = evaluate_tree(exec_args);
        if (condition == EXIT_SUCCESS || *exec_args.exit)
            return status;
        if (condition < 0)
            return condition;
        exec_args.ast = curr->to_do;
        status = evaluate_tree(exec_args);
        // break and continue
        if (*exec_args.exit)
            return status;

        if (*exec_args.break_val)
        {
            if (curr->is_base)
            {
                *exec_args.break_val = 0;
                break;
            }
            *exec_args.break_val -= 1;
            break;
        }

        if (*exec_args.continue_val)
        {
            if (curr->is_base || *exec_args.continue_val == 1)
            {
                *exec_args.continue_val = 0;
                continue;
            }
            *exec_args.continue_val -= 1;
            break;
        }
    }

    if (curr->is_base)
        *exec_args.is_in_base = 0;

    return status;
}
