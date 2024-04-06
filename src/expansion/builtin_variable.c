#include <linux/limits.h>

#include "expansion/expansion.h"

static int get_len(int number)
{
    int count = 1;

    for (; number >= 10; count++, number /= 10)
    {}

    return count;
}

static struct expanse *expanse_init_nb_args(struct expanse *exp, int argc) // $#
{
    char *value = malloc(sizeof(char) * get_len(argc - 1) + 1);

    if (!value)
        return NULL;

    my_itoa(argc - 1, value);

    exp = expanse_add(exp, "#", value);

    free(value);

    return exp;
}

static struct expanse *expanse_init_PID(struct expanse *exp) // $$
{
    pid_t process_id = getpid();
    char *value = malloc(sizeof(char) * get_len((int)process_id) + 1);

    if (!value)
        return NULL;

    my_itoa((int)process_id, value);

    exp = expanse_add(exp, "$", value);
    free(value);
    return exp;
}

static struct expanse *expanse_init_UID(void) // $UID
{
    uid_t uid = geteuid();
    char *value = malloc(sizeof(char) * get_len((int)uid) + 1);

    if (!value)
        return NULL;

    my_itoa((int)uid, value);

    struct expanse *res = expanse_init("UID", value);
    free(value);
    return res;
}

static struct expanse *expanse_init_args(struct expanse *exp, int argc,
                                         char *argv[])
{
    int len_argv = 0;
    int index_value = 0;

    for (int i = 1; i < argc; i++)
    {
        len_argv += strlen(argv[i]);
    }

    len_argv += argc - 1; // pour les espaces &  \nentre les argv

    char *value_at = malloc(sizeof(char) * (len_argv + 1));

    for (int i = 1; i < argc; i++)
    {
        strcpy((value_at + index_value), argv[i]);
        index_value += strlen(argv[i]);
        if (i != argc - 1)
        {
            value_at[index_value++] = ' ';
        }

        char *value = malloc(sizeof(char) * get_len(i) + 1); // '\0'

        my_itoa(i, value);

        exp = expanse_add(exp, value, argv[i]);

        free(value);
    }

    value_at[index_value] = '\0';
    exp = expanse_add(exp, "*", value_at);
    exp = expanse_add(exp, "@", value_at);
    free(value_at);

    return exp;
}

static struct expanse *expanse_init_random(struct expanse *exp)
{
    char *numb = generate_random();
    if (!numb)
        return NULL;

    exp = expanse_add(exp, "RANDOM", numb);
    free(numb);

    return exp;
}

static struct expanse *expanse_init_ifs(struct expanse *exp)
{
    char *numb = generate_random();
    if (!numb)
        return NULL;

    exp = expanse_add(exp, "IFS", " \t\n");
    free(numb);

    return exp;
}

static struct expanse *expanse_init_pwd(struct expanse *exp)
{
    char absolute_cwd[PATH_MAX] = {
        0,
    };

    getcwd(absolute_cwd, PATH_MAX);

    if (!getenv("PWD"))
        setenv("PWD", absolute_cwd, 1);
    if (!getenv("OLDPWD"))
        setenv("OLDPWD", absolute_cwd, 1);

    return exp;
}

struct expanse *expanse_init_var(int argc, char *argv[])
{
    struct expanse *exp = expanse_init_UID(); // $UID
    if (!exp)
        return NULL;

    exp = expanse_init_PID(exp);
    if (!exp) // $$
        return NULL;

    exp = expanse_init_nb_args(exp, argc);
    if (!exp) // $#
        return NULL;

    exp = expanse_init_args(exp, argc, argv);
    if (!exp) // $1..$argc-1
        return NULL;

    exp = expanse_init_random(exp);
    if (!exp) // $RANDOM
        return NULL;

    exp = expanse_init_pwd(exp);
    if (!exp) // $PWD && $OLDPWD
        return NULL;

    exp = expanse_init_ifs(exp);

    return exp;
}
