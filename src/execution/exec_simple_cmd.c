#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "execution/builtin/builtin.h"
#include "execution/execution.h"
#include "expansion/expand.h"
#include "expansion/expansion.h"

/*
 * BRIEF:
 * Execute a command found in a node
 * return -1 on error | return value of the executed command otherwise
 */
static int exec_cmd(char *argv[])
{
    int pid = fork();

    if (pid == 0)
    {
        execvp(argv[0], argv);
        warnx("command not found: %s", argv[0]);

        // TODO: use exit in exec_args to exit normally in the child.
        // _exit might leave file descriptors open (man 2 exit)
        _exit(EXIT_COMMAND_UNFOUND);
    }
    else
    {
        int wstatus;
        waitpid(pid, &wstatus, 0);
        return WEXITSTATUS(wstatus);
    }
}

static int switch_builtin(struct ast_simple_cmd *curr,
                          struct exec_args exec_args)
{
    if (strcmp(curr->argv[0], "echo") == 0)
    {
        return my_echo(curr->argv);
    }
    else if (strcmp(curr->argv[0], "true") == 0)
    {
        return EXIT_SUCCESS;
    }
    else if (strcmp(curr->argv[0], "false") == 0)
    {
        return EXIT_FAILURE;
    }
    else if (strcmp(curr->argv[0], "exit") == 0)
    {
        return my_exit(curr->argc, curr->argv, exec_args.exit);
    }
    else if (strcmp(curr->argv[0], "unset") == 0)
    {
        return my_unset(curr->argc, curr->argv, exec_args.hash_map,
                        exec_args.expansion);
    }
    else if (strcmp(curr->argv[0], ".") == 0)
    {
        return my_dot(curr->argc, curr->argv, exec_args.expansion);
    }
    else if (strcmp(curr->argv[0], "export") == 0)
    {
        return my_export(curr->argc, curr->argv, exec_args.expansion);
    }
    else if (strcmp(curr->argv[0], "cd") == 0)
    {
        return my_cd(curr->argc, curr->argv);
    }
    else if (strcmp(curr->argv[0], "continue") == 0)
    {
        return my_continue(curr->argc, curr->argv, exec_args.continue_val);
    }
    else if (strcmp(curr->argv[0], "break") == 0)
    {
        return my_break(curr->argc, curr->argv, exec_args.break_val);
    }
    else
    {
        return exec_cmd(curr->argv);
    }
}

/*
 * BRIEF:
 * Checking wether the cmd is builtin and calling the function
 * or else and excetue with excevp
 * return value of the command
 */
int builtin_or_not(struct exec_args exec_args)
{
    assert(exec_args.ast);
    assert(exec_args.ast->type == AST_SIMPLE_COMMAND);

    struct ast_simple_cmd *curr = (void *)exec_args.ast;

    // SCL 4: Each variable assignment shall be expanded for tilde expansion,
    // parameter expansion, command substitution, arithmetic expansion, and
    // quote removal prior to assigning the value.
    expand_assignment_word(curr, exec_args.expansion);

    int status = EXIT_SUCCESS;

    if (curr->argc == 0) /*If there is no command */
    {
        assign_variables(exec_args.ast, exec_args.expansion);
        return EXIT_SUCCESS;
    }

    struct expanse *save = save_environment_variables(curr);

    add_to_env(curr);

    // when defining function you could overload a builtin
    // so when calling it it should check if an overloaded one exists first
    struct ast *exec_ast =
        (void *)hash_map_get(exec_args.hash_map, curr->argv[0]);

    if (exec_ast)
    {
        exec_args.ast = exec_ast;
        struct expanse *tmp_exp = expanse_init_var(curr->argc, curr->argv);
        exec_args.expansion = tmp_exp;
        int res = evaluate_tree(exec_args);
        expanse_cleanup(exec_args.expansion);
        return res;
    }
    else
    {
        status = switch_builtin(curr, exec_args);
    }

    restore_environment_variables(save);
    expanse_cleanup(save);

    return status;
}

int evaluate_simple_cmd(struct exec_args exec_args)
{
    assert(exec_args.ast);
    assert(exec_args.ast->type == AST_SIMPLE_COMMAND);

    struct ast_simple_cmd *curr = (struct ast_simple_cmd *)exec_args.ast;

    // save argv to replace it
    char **s_argv = calloc(curr->argc + 1, sizeof(char *)); // copied argv
    for (size_t i = 0; i < curr->argc; i++)
    {
        s_argv[i] = my_strdup(curr->argv[i]);
    }

    s_argv[curr->argc] = NULL;

    // SCL: 2. The words that are not variable assignments or redirections shall
    // be expanded.
    for (char **arg = curr->argv; *arg; arg++)
    {
        char *new = expand(exec_args.expansion, *arg);
        free(*arg);
        *arg = new;
    }

    // SCL: 3. Redirections shall be performed as described in Redirection
    int ret_eval = evaluate_redir(curr->redir_list, exec_args);

    // free previous argv and replace it with s_argv
    for (char **arg = curr->argv; *arg; arg++)
    {
        free(*arg);
    }
    free(curr->argv);
    curr->argv = s_argv; // restore argv after expansion
    return ret_eval;
}
