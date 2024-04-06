#include <assert.h>
#include <ctype.h>
#include <err.h>

#include "expansion/expand.h"
#include "expansion/expansion.h"
#include "utils/utils.h"

/*
 * ${abcd} -> ${abcd}
 */
char *get_enclosed_parameter_braces(struct expand_args *args)
{
    size_t *i = &args->i;
    char *dst = args->dst;
    char *src = args->src;

    /* Add the dollar and braces char */
    dst = add_char(dst, src[*i]);
    (*i)++;

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

    /* Add brace char */
    dst = add_char(dst, src[*i]);
    (*i)++;

    return dst;
}

/*
 * Expand a dollar, WITH braces enclosing. (ex: $parameter )
 * ${brianteoarthurrobin}
 * ^                     ^
 * start                 end
 */
char *expand_parameter_braces(struct expand_args *args, bool double_quoted)
{
    size_t *i = &args->i;
    char *dst = args->dst;
    char *src = args->src;

    // Skip the dollar char
    (*i)++;

    assert(src[*i] == '{');

    // Skip the braces char
    (*i)++;

    char *parameter = get_parameter_string(src, i);

    if (src[*i] != '}' || strcmp(parameter, "") == 0)
    {
        warnx("%s: bad substitution", src);
        free(parameter);
        free(dst);
        return NULL;
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

    (*i)++;

    return dst;
}
