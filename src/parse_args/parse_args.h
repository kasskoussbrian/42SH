#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

#include <stdbool.h>
#include <stdlib.h>

enum io_backend_mode
{
    IO_BACKEND_FILE = 0,
    IO_BACKEND_STRING,
    IO_BACKEND_STDIN,
};

struct cli_args
{
    enum io_backend_mode io_mode;
    bool is_pretty_print;
    bool is_verbose;
    char *filename;
    char *script;
};

int parse_cli_args(struct cli_args *args, int argc, char **argv);
void filter_args(int argc, char *argv[], int *new_argc, char **new_argv[]);

#endif /* (! PARSE_ARGS_H ) */
