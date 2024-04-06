#include <assert.h>

#include "expansion/expand.h"
#include "expansion/expansion.h"
#include "lexer/lexer.h"

char *expand_normal(struct expand_args *args)
{ // On check si "~" || "~/" || "isspace~/" || "isspace~isspace" || ~:

    char *src = args->src;
    char *dst = args->dst;
    size_t *i = &(args->i);

    if (src[*i] == '~' && (*i == 0 || (*i > 0 && isspace(src[*i - 1])))
        && (isspace(src[*i + 1]) || src[*i + 1] == '/' || src[*i + 1] == '\0'
            || src[*i + 1] == ':'))
    {
        char *path = getenv("HOME");
        for (size_t j = 0; j <= strlen(path); j++)
        {
            dst = add_char(dst, path[j]);
        }
    }
    else
    {
        if (src[*i] == '~' && (*i == 0 || (*i > 0 && isspace(src[*i - 1])))
            && isalpha(src[*i + 1]))
            dst = add_char(dst, '/'); // check SCL of course : ~ -> /
        else
            dst = add_char(dst, src[*i]);
    }
    (*i)++;

    return dst;
}
