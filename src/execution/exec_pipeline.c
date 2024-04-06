#include <sys/wait.h>

#include "execution/execution.h"

int evaluate_pipe_line(struct exec_args exec_args)
{
    assert(exec_args.ast);
    assert(exec_args.ast->type == AST_PIPELINE);
    struct ast_pipeline *curr = (void *)exec_args.ast;
    // execute pipeline thing

    int pipe_fds[2];
    if (pipe(pipe_fds))
    {
        perror("pipe:");
        return 1;
    }

    assert(curr->left);
    int fork_pid = fork();

    if (fork_pid == -1)
    {
        perror("fork:");
        return 1;
    }
    // If in the child
    else if (fork_pid == 0)
    {
        close(STDOUT_FILENO);
        close(pipe_fds[0]);
        dup2(pipe_fds[1], STDOUT_FILENO);
        exec_args.ast = curr->left;
        return evaluate_tree(exec_args);
    }

    assert(curr->right);
    // If in the parent
    close(STDIN_FILENO);
    close(pipe_fds[1]);
    dup2(pipe_fds[0], STDIN_FILENO);

    // Wait for the child
    int wstatus;
    wait(&wstatus);

    // No need to check exit return code

    exec_args.ast = curr->right;
    return evaluate_tree(exec_args);
}
