#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin/builtin.h"
#include "execution/execution.h"
#define PATH_MAX 4096 /* # chars in a path name including nul */

int my_cd(int argc, char *argv[])
{
    char *path;
    char *old_pwd;
    char cwd[PATH_MAX];
    bool print = false;

    if (argc > 2)
    {
        warnx("cd: too many arguments");
        return EXIT_FAILURE;
    }

    if (argc == 1)
    {
        path = getenv("HOME");
    }
    else if (argv[1][0] == '-') // cd -
    {
        if (strcmp(argv[1], "-") == 0)
        {
            print = true;
            path = getenv("OLDPWD");
        }
        else if (strcmp(argv[1], "--") == 0)
        {
            path = getenv("HOME");
        }
        else
        {
            warnx("bash: cd: -a: invalid option");
            return EXIT_CLI_ARGS_ERROR;
        }
    }
    else
    {
        path = argv[1];
    }

    if (chdir(path) != 0)
    {
        warnx("cd: %s: No such file or directory", path);
        return EXIT_FAILURE;
    }
    old_pwd = getenv("PWD");
    path = getcwd(cwd, PATH_MAX);

    if (print)
        printf("%s\n", path);
    setenv("PWD", path, 1);

    if (old_pwd == NULL)
        old_pwd = path;

    setenv("OLDPWD", old_pwd, 1);

    return EXIT_SUCCESS;
}
