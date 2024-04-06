#include "builtin/builtin.h"

int my_continue(int argc, char *argv[], int *continue_val)
{
    if (argc == 1)
    {
        *continue_val = 1;
        return EXIT_SUCCESS;
    }
    *continue_val += atoi(argv[1]);
    return EXIT_SUCCESS;
}
