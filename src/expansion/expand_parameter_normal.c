#include <assert.h>
#include <ctype.h>
#include <err.h>

#include "expansion/expand.h"
#include "expansion/expansion.h"
#include "utils/utils.h"

/*
 * $abcd -> $abcd
 * $a -> $a
 * $_ -> $
 */
char *get_enclosed_parameter_normal(struct expand_args *args)
{
    size_t *i = &args->i;
    char *dst = args->dst;
    char *src = args->src;

    /* Add the dollar */
    dst = add_char(dst, src[*i]);

    (*i)++;

    char *parameter = get_parameter_string(src, i);

    if (strcmp(parameter, "") == 0) // Means that there is no parameter
    {
        free(parameter);
        return dst;
    }

    // Concat dst and parameter
    size_t parameter_len = strlen(parameter);
    size_t dst_len = strlen(dst);

    dst = realloc(dst, (dst_len + parameter_len + 1) * sizeof(char));
    dst = strcat(dst, parameter);

    free(parameter);

    return dst;
}

/*
 * Expand a dollar, without braces enclosing. (ex: $parameter )
 */
char *expand_parameter_normal(struct expand_args *args, bool double_quoted)
{
    size_t *i = &args->i;
    char *dst = args->dst;
    char *src = args->src;

    // Skip the dollar char
    (*i)++;

    char *parameter = get_parameter_string(src, i);

    if (strcmp(parameter, "") == 0) // Means that there is no parameter
    {
        free(parameter);
        dst = add_char(dst, '$');
        return dst;
    }

    char *value = get_value_from_parameter(args->exp, parameter);

    if (value && !double_quoted)
    {
        value = field_splitting(args->exp, value);
    }

    for (char *v = value; value && *v; v++)
    {
        dst = add_char(dst, *v);
    }

    free(parameter);
    free(value);

    return dst;
}
