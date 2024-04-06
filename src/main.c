#include "loop/loop.h"
int main(int argc, char **argv)
{
    struct cli_args args = { 0 };

    if (parse_cli_args(&args, argc, argv) == -1)
    {
        return EXIT_CLI_ARGS_ERROR;
    }

    // TODO: Get the information about why couldn't init parser
    // (It might be because the file doesn't exist, and so we should return with
    // a return code of 1 )
    struct parser *parser = parser_init(args);
    if (!parser)
    {
        return EXIT_SUCCESS;
    }

    int new_argc;
    char **new_argv = NULL;
    if (args.io_mode != IO_BACKEND_FILE)
    {
        new_argc = 1;
        new_argv = NULL;
    }
    else
    {
        filter_args(argc, argv, &new_argc, &new_argv);
    }

    // TODO: get real argc and argv in cli_args
    struct expanse *expanse = expanse_init_var(new_argc, new_argv);
    if (new_argv) // free if not NULL
        free(new_argv);
    struct hash_map *hash_map = hash_map_init(10);

    // TODO: Init all args (undefined behaviors can occur)
    struct exec_args exec_args;
    exec_args.expansion = expanse;
    int exit = 0;
    exec_args.exit = &exit;
    exec_args.hash_map = hash_map;
    int continue_val = 0;
    int break_val = 0;
    exec_args.continue_val = &continue_val;
    exec_args.break_val = &break_val;
    int is_in_base = 0;
    exec_args.is_in_base = &is_in_base;

    return main_loop(exec_args, parser);
}
