#define EXIT_UNSUCCESSFUL_COMMAND_SEARCH 127;
#include <err.h>
#include <stddef.h>
#include <string.h>

#include "builtin/builtin.h"
#include "execution/execution.h"
#include "loop/loop.h"
#include "parser/parser.h"

int my_dot(int argc, char *argv[], struct expanse *expanse)
{
    if (argc == 1)
    {
        warnx(".: usage: . filename [arguments]");
        return EXIT_CLI_ARGS_ERROR;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile)
    {
        warnx("%s: No such file or directory", argv[1]);
        return EXIT_FAILURE;
    }

    struct cli_args args = { 0 };
    char **new_argv = calloc(2, sizeof(char *));
    new_argv[0] = ".";
    new_argv[1] = argv[1];

    if (parse_cli_args(&args, 2, new_argv) == -1)
    {
        free(new_argv);
        return EXIT_CLI_ARGS_ERROR;
    }

    struct parser *parserx = parser_init(args);

    if (!parserx)
    {
        free(new_argv);
        return EXIT_SUCCESS;
    }

    struct expanse *expansex = expanse_copy(expanse);

    struct hash_map *hash_map = hash_map_init(10);
    int exit = 0;
    int continue_val = 0;
    int break_val = 0;
    int is_in_base = 0;
    struct exec_args exec_args = {
        .ast = NULL,
        .expansion = expansex,
        .exit = &exit,
        .hash_map = hash_map,
        .updated = 0,
        .continue_val = &continue_val,
        .break_val = &break_val,
        .is_in_base = &is_in_base,
    };

    int ret = main_loop(exec_args, parserx);

    free(new_argv);
    return ret;
}
