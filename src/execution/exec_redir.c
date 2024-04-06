#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast/ast.h"
#include "execution/execution.h"

// TODO: when error occurs on opening file, display an error message depending
// on errno

/* This function associated the io number if it was not detected during parsing.
 */
void init_empty_io_number(struct ast_redir *ast)
{
    if (ast->IO_nb != -1)
    {
        return;
    }

    switch (ast->redir_type)
    {
    case REDIR_RIGHT_UPSAND:
    case REDIR_RIGHT_PIPE:
    case REDIR_RIGHT_RIGHT:
    case REDIR_RIGHT:
        ast->IO_nb = STDOUT_FILENO;
        return;
    case REDIR_LEFT_UPSAND:
    case REDIR_LEFT_RIGHT:
    case REDIR_LEFT:
        ast->IO_nb = STDIN_FILENO;
        return;
    case REDIR_NOT_FOUND:
        return;
    }
}

int get_fd_redir_left(struct ast_redir *ast)
{
    assert(ast);
    assert(ast->redir_type == REDIR_LEFT);

    assert(ast->filename);
    int fd = open(ast->filename, O_RDONLY);
    if (!fd)
    {
        warn("open(%s)", ast->filename);
        return -1;
    }
    return fd;
}

int get_fd_redir_left_upsand(struct ast_redir *ast)
{
    assert(ast);
    assert(ast->redir_type == REDIR_LEFT_UPSAND);

    if (ast->fd_word == -1)
    {
        warn("open(%s)", ast->filename);
    }

    return ast->fd_word;
}

int get_fd_redir_right_upsand(struct ast_redir *ast)
{
    assert(ast);
    assert(ast->redir_type == REDIR_RIGHT_UPSAND);

    if (ast->fd_word == -1)
    {
        warn("open(%s)", ast->filename);
    }

    return ast->fd_word;
}

int get_fd_redir_right(struct ast_redir *ast)
{
    assert(ast);
    assert(ast->redir_type == REDIR_RIGHT);

    assert(ast->filename);

    int fd = open(ast->filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    if (fd == -1)
    {
        warn("open(%s)", ast->filename);
        return -1;
    }
    return fd;
}

int get_fd_redir_right_right(struct ast_redir *ast)
{
    assert(ast);
    assert(ast->redir_type == REDIR_RIGHT_RIGHT);

    assert(ast->filename);

    int fd = open(ast->filename, O_CREAT | O_APPEND | O_RDWR, 0644);
    if (fd == -1)
    {
        warn("open(%s)", ast->filename);
        return -1;
    }
    return fd;
}

int get_fd_redir_left_right(struct ast_redir *ast)
{
    assert(ast);
    assert(ast->redir_type == REDIR_LEFT_RIGHT);

    assert(ast->filename);
    int fd = open(ast->filename, O_CREAT | O_RDWR, 0644);
    if (!fd)
    {
        warnx("no such file or directory: %s", ast->filename);
        return -1;
    }
    return fd;
}

int get_new_fd(struct ast_redir *ast)
{
    assert(ast);

    switch (ast->redir_type)
    {
    case REDIR_LEFT:
        return get_fd_redir_left(ast);
    case REDIR_LEFT_UPSAND:
        return get_fd_redir_left_upsand(ast);
    case REDIR_RIGHT_PIPE:
    case REDIR_RIGHT:
        return get_fd_redir_right(ast);
    case REDIR_RIGHT_RIGHT:
        return get_fd_redir_right_right(ast);
    case REDIR_RIGHT_UPSAND:
        return get_fd_redir_right_upsand(ast);
    case REDIR_LEFT_RIGHT:
        return get_fd_redir_left_right(ast);
    default:
        return -1;
    }

    return -1;
}

int evaluate_redir(struct list *list, struct exec_args exec_args)
{
    assert(exec_args.ast);
    assert(exec_args.ast->type == AST_SHELL_COMMAND
           || exec_args.ast->type == AST_SIMPLE_COMMAND);

    // END OF REDIRECTION LIST
    if (list == NULL)
    {
        if (exec_args.ast->type == AST_SIMPLE_COMMAND)
        {
            return builtin_or_not(exec_args);
        }

        if (exec_args.ast->type == AST_SHELL_COMMAND)
        {
            struct ast_shell_cmd *eval = (void *)exec_args.ast;
            exec_args.ast = eval->ast;
            return evaluate_tree(exec_args);
        }

        if (exec_args.ast->type == AST_FUNC)
        {
            struct ast_func *eval = (void *)exec_args.ast;
            exec_args.ast = eval->function;
            return evaluate_tree(exec_args);
        }
    }

    struct ast *ast = list->ast;

    assert(ast->type == AST_REDIR);

    struct ast_redir *redir = (struct ast_redir *)ast;

    if (redir->IO_nb == -1)
    {
        init_empty_io_number(redir);
    }

    redir->fd_word = get_new_fd(redir);

    if (redir->fd_word == -1)
    {
        return EXIT_FAILURE;
    }

    // Saved redirected to file descriptor
    int io_nb_save = dup(redir->IO_nb);

    // Redirect
    if (dup2(redir->fd_word, redir->IO_nb) == -1)
    {
        warn("dup()");
        return EXIT_FAILURE;
    }

    // Call recursively the linked list
    int status = evaluate_redir(list->next, exec_args);

    // backup the saved file descriptor
    dup2(io_nb_save, redir->IO_nb);

    // close the backup file descriptor
    close(io_nb_save);

    return status;
}
