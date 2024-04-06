#include "expansion/expand.h"
#include "expansion/expansion.h"
#include "utils/utils.h"

/* Expand double quote */
char *expand_double_quote(struct expand_args *args)
{
    char *src = args->src;

    // Skip the dquote
    (args->i)++;

    while (42)
    {
        if (src[args->i] == '\0')
            break;

        else if (src[args->i] == '\\')
        {
            args->dst = expand_backslash(args, true);
        }
        else if (src[args->i] == '`')
        {
            args->dst = expand_substitution_backtick(args, true);
        }
        else if (src[args->i] == '$' && (src[args->i + 1]) == '(')
        {
            args->dst = expand_substitution_parenthesis(args, true);
        }
        else if (src[args->i] == '$'
                 && src[args->i + 1] == '{') /* {parameter} form */
        {
            args->dst = expand_parameter_braces(args, true);
            if (!args->dst)
                return NULL;
        }
        else if (src[args->i] == '$') /* $parameter form */
        {
            args->dst = expand_parameter_normal(args, true);
        }
        else if (src[args->i] == '"') /* Found matching '"' "*/
        {
            (args->i)++;
            return args->dst;
        }
        else
        {
            args->dst = add_char(args->dst, src[args->i]);
            (args->i)++;
        }
    }

    (args->i)++;
    return args->dst;
}

/*
 * "       abcd   "
 * ^ start         ^end
 */
char *get_enclosed_double_quote(struct expand_args *args)
{
    char *src = args->src;

    // Skip the dquote
    (args->i)++;

    while (42)
    {
        if (src[args->i] == '\0')
        {
            free(args->dst);
            return NULL;
        }
        else if (src[args->i] == '\\')
        {
            args->dst = get_enclosed_backslash(args, true);
        }
        else if (src[args->i] == '`')
        {
            args->dst = get_enclosed_substitution_backtick(args);
        }
        else if (src[args->i] == '$' && (src[args->i + 1]) == '(')
        {
            args->dst = get_enclosed_substitution_parenthesis(args);
        }
        else if (src[args->i] == '$'
                 && src[args->i + 1] == '{') /* {parameter} form */
        {
            args->dst = get_enclosed_parameter_braces(args);
        }
        else if (src[args->i] == '$') /* $parameter form */
        {
            args->dst = get_enclosed_parameter_normal(args);
        }
        else
        {
            args->dst = add_char(args->dst, src[args->i]);

            if (src[args->i] == '"')
            {
                (args->i)++;
                return args->dst;
            }

            (args->i)++;
        }
    }

    return args->dst;
}
