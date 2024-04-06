#include "builtin/builtin.h"

int my_break(int argc, char *argv[], int *break_val)
{
    if (argc == 1)
    {
        *break_val = 1;
        return EXIT_SUCCESS;
    }
    *break_val += atoi(argv[1]);
    return EXIT_SUCCESS;
}
