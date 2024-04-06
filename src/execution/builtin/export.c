#include <stdlib.h>

#include "builtin/builtin.h"
#include "execution/execution.h"
#include "expansion/expansion.h"
#include "string.h"

enum export_check_arg
{
    FLAG,
    NO_FLAG,
    ERROR
};

static enum export_check_arg export_check(int argc, char *argv[])
{
    int len_argv = 0;
    enum export_check_arg flag = NO_FLAG;

    for (int index = 1; index < argc; index++)
    {
        if (argv[index][0] != '-')
            break;

        len_argv = strlen(argv[index]);

        for (int i = 1; i < len_argv; i++)
        {
            if (argv[index][i] != 'p')
                return ERROR;
        }
        flag = FLAG;
    }

    return flag;
}

int my_export(int argc, char *argv[], struct expanse *expansion)
{
    enum export_check_arg check = export_check(argc, argv);
    struct expanse *prev = NULL;
    struct expanse *tmp = NULL;

    if (check == ERROR)
    {
        warnx("usage: export [%s] [name[=value] ...] or export -p", argv[1]);
        return EXIT_CLI_ARGS_ERROR;
    }

    for (int i = 1; i < argc; i++)
    {
        for (struct expanse *cur = expansion; cur; cur = cur->next)
        {
            if (strcmp(cur->variable, argv[i]) == 0)
            {
                setenv(cur->variable, cur->value, 1);

                if (!prev) // delete the head
                    expansion = remove_expanse(cur, prev);
                else
                {
                    tmp = cur->next;
                    remove_expanse(cur, prev);
                    cur = tmp;
                }
                break;
            }
            prev = cur;
        }
    }

    return EXIT_SUCCESS;
}
