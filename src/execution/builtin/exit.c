#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin/builtin.h"

#define EXIT_GRAMMAR_ERROR 2
/*
 * If fails because of arguments, returns 0.
 * Otherwise, returns the exit code that the shell should return + 1, multiplied
 * by -1.
 */
int my_exit(int argc, char *argv[], int *exit)
{
    if (argc == 1)
    {
        *exit = 1;
        return -42; // pour le cas jfisdi ; exit 123
    }

    int n = atoi(argv[1]);
    /* If argv[0] does not represent a number and too many args */
    if (argc >= 2 && (strcmp(argv[1], "0") != 0) && n == 0)
    {
        warnx("exit: %s: numeric argument required", argv[1]);
        fflush(stderr);
        return EXIT_GRAMMAR_ERROR;
    }

    *exit = 1;

    if (argc > 2)
    {
        warnx("exit: too many arguments");
        fflush(stderr);
        return EXIT_FAILURE;
    }

    // Code imcomprehensible bruh
    if (n < 0)
        return 256 - (-n % 256);
    return n % 256;
}
