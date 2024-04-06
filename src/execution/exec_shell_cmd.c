#include "execution/execution.h"

int evaluate_shell_cmd(struct exec_args exec_args)
{
    struct ast_shell_cmd *curr = (struct ast_shell_cmd *)exec_args.ast;

    return evaluate_redir(curr->redir_list, exec_args);
}
