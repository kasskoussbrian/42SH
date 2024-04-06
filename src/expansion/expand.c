#include "expansion/expand.h"

#include "expansion/expansion.h"
#include "utils/utils.h"

/*
 * This function is reponsible for expanding .
 * Returns null if an error occurs.
 * Returns a new allocated string
 */
char *expand(struct expanse *exp, char *src)
{
    char *dst = calloc(1, sizeof(char));
    dst[0] = '\0';

    struct expand_args args = {
        .src = src,
        .dst = dst,
        .exp = exp,
        .i = 0,
    };

    while (src[args.i])
    {
        if (src[args.i] == '\\')
        {
            args.dst = expand_backslash(&args, false);
        }
        else if (src[args.i] == '"')
        {
            args.dst = expand_double_quote(&args);
        }
        else if (src[args.i] == '\'')
        {
            args.dst = expand_simple_quote(&args);
        }
        else if (src[args.i] == '`')
        {
            args.dst = expand_substitution_backtick(&args, false);
        }
        else if (src[args.i] == '$' && (src[args.i + 1]) == '(')
        {
            args.dst = expand_substitution_parenthesis(&args, false);
        }
        else if (src[args.i] == '$'
                 && src[args.i + 1] == '{') /* {parameter} form */
        {
            args.dst = expand_parameter_braces(&args, false);
            if (!args.dst)
                return NULL;
        }
        else if (src[args.i] == '$') /* $parameter form */
        {
            args.dst = expand_parameter_normal(&args, false);
        }
        else
        {
            args.dst = expand_normal(&args);
        }
    }

    return args.dst;
}
