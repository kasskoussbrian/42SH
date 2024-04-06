#include "utils/utils.h"
#define PATH_MAX 4096

char *my_strdup(const char *buff)
{
    if (!buff)
        return NULL;

    char *copy = malloc((strlen(buff) + 1) * sizeof(char));

    if (!copy)
        return NULL;

    strcpy(copy, buff);

    return copy;
}

char *my_itoa(int value, char *s)
{
    if (value == 0)
    {
        s[0] = '0';
        s[1] = 0;
        return s;
    }

    int ndigit = 0;

    if (value < 0)
    {
        ndigit++;
        value = -value;
        s[0] = '-';
    }

    for (int tmp = value; tmp; tmp /= 10)
    {
        ndigit++;
    }

    s[ndigit] = 0;

    while (value != 0)
    {
        s[--ndigit] = '0' + value % 10;
        value /= 10;
    }

    return s;
}

/*
 * Append the character c to the buffer 'buff'
 */
char *add_char(char *buff, char c)
{
    // ACU "regle d'or de la prog : aller au + simple"
    int buffer_len = strlen(buff) + 1; // DONC TG ROBIN
    buff = realloc(buff, (buffer_len + 1) * sizeof(char));
    buff[buffer_len - 1] = c;
    buff[buffer_len] = '\0';
    return buff;
}

// Generate number between 0 and 32767 cast it in char * and return
char *generate_random(void)
{
    char *numb = malloc(sizeof(char) * 6);
    if (!numb)
        return NULL;

    int random_number = rand();
    srand((unsigned)time(NULL));
    random_number %= 32768;

    my_itoa(random_number, numb);

    return numb;
}

char *get_pwd(void)
{
    char *pwd = malloc(sizeof(char) * PATH_MAX);
    if (!pwd)
        return NULL;

    getcwd(pwd, PATH_MAX);

    return pwd;
}
