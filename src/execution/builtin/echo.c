#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin/builtin.h"

static size_t parse_mode(char *argv[], int *mode_n, int *mode_e)
{
    size_t i = 1;
    int mode_n_save = 0;
    int mode_e_save = 0;

    while (argv[i] != NULL)
    {
        if ((strlen(argv[i]) < 2) || (argv[i][0] != '-'))
            break;

        size_t len_arg = strlen(argv[i]);
        for (size_t j = 1; j < len_arg; j++)
        {
            if (argv[i][j] == 'e')
                mode_e_save = 1;
            else if (argv[i][j] == 'E')
                mode_e_save = 0;
            else if (argv[i][j] == 'n')
                mode_n_save = 1;
            else
                return i;
        }
        *mode_n = mode_n_save;
        *mode_e = mode_e_save;
        i++;
    }
    return i;
}

void print_string_escaping(char *str)
{
    size_t c = 0;
    while (str[c] != '\0')
    {
        if (str[c] == '\\')
        {
            switch (str[++c])
            {
            case '\\':
                printf("\\");
                break;
            case 't':
                printf("\t");
                break;
            case 'n':
                printf("\n");
                break;
            default:
                break;
            }
        }
        else
            printf("%c", str[c]);
        c++;
    }
}

void print_string(int mode_e, char *str)
{
    if (mode_e)
    {
        print_string_escaping(str);
    }
    else
    {
        printf("%s", str);
    }
}

int my_echo(char *argv[])
{
    int mode_n = 0;
    int mode_e = 0;
    size_t i = parse_mode(argv, &mode_n, &mode_e);

    while (argv[i] && strlen(argv[i]) == 0)
    {
        i++;
    }

    if (argv[i])
    {
        print_string(mode_e, argv[i++]);
    }

    for (; argv[i]; i++)
    {
        if (strlen(argv[i]) == 0)
            continue;
        printf(" ");
        print_string(mode_e, argv[i]);
    }

    if (!mode_n)
        printf("\n");

    fflush(stdout);

    return EXIT_SUCCESS;
}
