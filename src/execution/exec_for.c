#include "execution.h"
#include "expansion/expand.h"

int evaluate_for(struct exec_args exec_args)
{
    assert(exec_args.ast);
    struct ast_for *ast_for = (void *)exec_args.ast;
    assert(ast_for->to_do);
    if (ast_for->argc == 0)
    {
        // word should iterate threw all programm variables
        // and execute the to_do tree

        // should return last_command
        return 0;
    }
    if (!ast_for->is_base && !*exec_args.is_in_base)
    {
        ast_for->is_base = 1;
        *exec_args.is_in_base = 1;
    }
    int status = 0;
    for (int i = 0; i < ast_for->argc; i++)
    {
        char *arg_expansion = expand(exec_args.expansion, ast_for->argv[i]);
        if (strlen(arg_expansion) == 0)
        {
            free(arg_expansion);
            continue;
        }

        exec_args.expansion =
            expanse_add(exec_args.expansion, ast_for->word, arg_expansion);
        // iterate threw all the words given in the for command
        // replace the word by the words given after "in"

        exec_args.ast = ast_for->to_do;
        status = evaluate_tree(exec_args);
        free(arg_expansion);
        // break and continue
        if (*exec_args.exit)
            return status;

        if (*exec_args.break_val)
        {
            if (ast_for->is_base)
            {
                *exec_args.break_val = 0;
                break;
            }
            *exec_args.break_val -= 1;
            break;
        }

        if (*exec_args.continue_val)
        {
            if (ast_for->is_base || *exec_args.continue_val == 1)
            {
                *exec_args.continue_val = 0;
                continue;
            }
            *exec_args.continue_val -= 1;
            break;
        }
    }

    if (ast_for->is_base)
        *exec_args.is_in_base = 0;

    return status;
}
