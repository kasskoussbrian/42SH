#include "execution/execution.h"

int evaluate_while(struct exec_args exec_args)
{
    assert(exec_args.ast);
    assert(exec_args.ast->type == AST_WHILE);

    struct ast_while *curr = (struct ast_while *)exec_args.ast;

    if (!curr->is_base && !(*exec_args.is_in_base))
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
        if (*exec_args.exit)
            return condition;
        if (condition < 0)
            return condition;
        if (condition != EXIT_SUCCESS)
            return status;
        exec_args.ast = curr->to_do;
        status = evaluate_tree(exec_args);
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
