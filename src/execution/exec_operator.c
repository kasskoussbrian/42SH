#include "execution/execution.h"

int evaluate_operator(struct exec_args exec_args)
{
    assert(exec_args.ast);
    assert(exec_args.ast->type == AST_OR || exec_args.ast->type == AST_AND);

    struct ast_operator *cur = (void *)exec_args.ast;

    if (cur->base.type == AST_AND)
    {
        exec_args.ast = cur->left;
        int o1 = evaluate_tree(exec_args);
        if (*exec_args.exit)
            return o1;
        if (o1 == EXIT_SUCCESS)
        {
            exec_args.ast = cur->right;
            return evaluate_tree(exec_args);
        }
        return o1;
    }

    exec_args.ast = cur->left;
    int o1 = evaluate_tree(exec_args);

    if (o1 == EXIT_SUCCESS || *exec_args.exit || *exec_args.continue_val
        || *exec_args.break_val)
        return o1;

    exec_args.ast = cur->right;
    return evaluate_tree(exec_args);
}
