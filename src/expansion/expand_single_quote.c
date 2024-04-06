#include "expansion/expand.h"
#include "expansion/expansion.h"
#include "utils/utils.h"

/* Expand simple quotes */
char *expand_simple_quote(struct expand_args *expand_args)
{
    char *dst = expand_args->dst;
    char *src = expand_args->src;

    size_t *i = &expand_args->i;
    (*i)++;
    for (; src[*i] && src[*i] != '\''; (*i)++)
    {
        dst = add_char(dst, src[*i]);
    }

    (*i)++;
    return dst;
}

/*
 * Get simple quotes
 * '       abcd   '
 * ^               ^
 */
char *get_enclosed_simple_quote(struct expand_args *expand_args)
{
    char *dst = expand_args->dst;
    char *src = expand_args->src;

    size_t *i = &expand_args->i;

    dst = add_char(dst, src[*i]);
    (*i)++;

    for (; src[*i] && src[*i] != '\''; (*i)++)
    {
        dst = add_char(dst, src[*i]);
    }

    dst = add_char(dst, src[*i]);
    (*i)++;

    return dst;
}
