#include <assert.h>
#include <err.h>
#include <sys/wait.h>

#include "execution/execution.h"
#include "expansion/expand.h"
#include "expansion/expansion.h"
#include "lexer/lexer.h"
#include "loop/loop.h"
#include "parse_args/parse_args.h"
#include "parser/parser.h"
#include "utils/utils.h"

char *read_everything(int fd, size_t *size)
{
    size_t capacity = 512;
    *size = 0;
    ssize_t nread = 0;
    char *buff = calloc(capacity + 1, sizeof(char));

    do
    {
        nread = read(fd, buff + *size, capacity - *size);
        if (nread == -1)
            break;
        *size += nread;
        if (*size == capacity)
        {
            capacity *= 2;
            buff = realloc(buff, (capacity + 1) * sizeof(char));
        }
    } while (nread > 0);

    buff[*size] = '\0';
    return buff;
}

void exec_main_loop_with_command(struct expand_args *expand_args, char *command)
{
    struct cli_args args = { .io_mode = IO_BACKEND_STRING,
                             .is_pretty_print = false,
                             .is_verbose = false,
                             .filename = NULL,
                             .script = command };

    struct parser *parser = parser_init(args);

    int exit = 0;
    int continue_val = 0;
    int break_val = 0;
    int is_in_base = 0;

    struct exec_args exec_args = {
        .ast = NULL,
        .expansion = expanse_copy(expand_args->exp),
        .exit = &exit,
        .hash_map = hash_map_init(10),
        .updated = false,
        .continue_val = &continue_val,
        .break_val = &break_val,
        .is_in_base = &is_in_base,
    };

    main_loop(exec_args, parser);
}

char *remove_trailing_newlines(char *src)
{
    if (!src)
        return NULL;

    size_t src_len = strlen(src);

    for (int i = src_len - 1; i >= 0 && src[i] == '\n'; i--)
    {
        src[i] = '\0';
    }

    return src;
}

/*
 * Create a pipe
 * In the child, execute the main loop and write stdout in the pipe
 * In the father, wait for the child to finish, read from the pipe into a
 * buffer add buffer to dst.
 */
char *get_command_output(struct expand_args *expand_args, char *command)
{
    assert(command);

    int stdout_pipe[2];

    if (pipe(stdout_pipe))
    {
        warn("pipe():");
        return NULL;
    }

    int fork_pid = fork();

    if (fork_pid == -1)
    {
        warn("fork:");
        return NULL;
    }

    else if (fork_pid == 0)
    {
        // If in the child

        close(STDOUT_FILENO);
        close(stdout_pipe[0]);
        dup2(stdout_pipe[1], STDOUT_FILENO);

        exec_main_loop_with_command(expand_args, command);

        // Can cause leaks
        _exit(EXIT_SUCCESS);
    }
    else
    {
        // If in the parent
        close(stdout_pipe[1]);

        // Wait for the child
        int wstatus;
        wait(&wstatus);

        size_t output_len = 0;

        char *output = read_everything(stdout_pipe[0], &output_len);

        output = remove_trailing_newlines(output);

        return output;
    }
}
