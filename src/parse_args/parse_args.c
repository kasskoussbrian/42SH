#include "parse_args/parse_args.h"

#include <assert.h>
#include <err.h>
#include <string.h>

int parse_cli_args(struct cli_args *args, int argc, char **argv)
{
    assert(args);
    assert(argv);

    args->io_mode = IO_BACKEND_STDIN;
    args->is_pretty_print = false;
    args->is_verbose = false;
    args->filename = NULL;
    args->script = NULL;

    int i = 1;

    while (i < argc)
    {
        if (strcmp(argv[i], "--pretty-print") == 0)
        {
            args->is_pretty_print = true;
        }
        else if (strcmp(argv[i], "--verbose") == 0)
        {
            args->is_verbose = true;
        }
        else if (strcmp(argv[i], "-c") == 0
                 && args->io_mode == IO_BACKEND_STDIN)
        {
            i++;
            if (i >= argc)
            {
                warn("-c: option requires an argument");
                return -1;
            }
            args->script = argv[i];
            args->io_mode = IO_BACKEND_STRING;
        }
        else
        {
            args->io_mode = IO_BACKEND_FILE;
            args->filename = argv[i];
            break;
        }

        i++;
    }

    return 0;
}

void filter_args(int argc, char *argv[], int *new_argc, char **new_argv[])
{
    *new_argc = argc - 1;
    *new_argv = calloc(*new_argc + 1, sizeof(char *));
    *new_argv[0] = argv[0];
    if (argc <= 2)
        return;
    for (int i = 0; i < *new_argc; i++)
    {
        (*new_argv)[i + 1] = argv[2 + i];
    }
}
