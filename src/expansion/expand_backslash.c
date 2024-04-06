#include <err.h>

#include "expansion/expand.h"
#include "expansion/expansion.h"
#include "utils/utils.h"

static bool do_backslash_escape_in_dquote(char c)
{
    return c == '$' || c == '`' || c == '"' || c == '\\';
}

/* Expand backslash
 *      start
 *      v
 * "    \a"
 *        ^ end
 */
char *expand_backslash(struct expand_args *expand_args, bool double_quoted)
{
    char *dst = expand_args->dst;
    char *src = expand_args->src;

    size_t *i = &expand_args->i;

    if (!double_quoted)
    {
        // Skip the backslash
        (*i)++;

        if (src[*i] == '\n')
        {
            return dst;
        }

        dst = add_char(dst, src[*i]);
        (*i)++;
        return dst;
    }

    else
    {
        if (do_backslash_escape_in_dquote(src[*i + 1]))
        {
            // Skip the backslash
            (*i)++;

            dst = add_char(dst, src[*i]);
            (*i)++;
            return dst;
        }
        else if (src[*i + 1] == '\n')
        {
            // Skip the backslash
            (*i)++;

            // Skip the newline
            (*i)++;

            return dst;
        }
        else
        {
            // Add backslash and the character
            dst = add_char(dst, src[*i]);
            (*i)++;

            dst = add_char(dst, src[*i]);
            (*i)++;
            return dst;
        }

        // return dst;
    }
}

/* Get enclosed backslash
 *      start
 *      v
 * "    \a"
 *        ^ end
 */
char *get_enclosed_backslash(struct expand_args *expand_args,
                             bool double_quoted)
{
    char *dst = expand_args->dst;
    char *src = expand_args->src;

    size_t *i = &expand_args->i;

    if (!double_quoted)
    {
        // Add the backslash
        dst = add_char(dst, src[*i]);
        (*i)++;

        // Should not happend
        if (src[*i] == '\n')
        {
            warnx("Detected a backslash followed by a newline in expansion.");
            return dst;
        }

        // Add the following character
        dst = add_char(dst, src[*i]);
        (*i)++;

        return dst;
    }

    else
    {
        if (do_backslash_escape_in_dquote(src[*i + 1]))
        {
            // Add the backslash
            dst = add_char(dst, src[*i]);
            (*i)++;

            // Add the following character
            dst = add_char(dst, src[*i]);
            (*i)++;

            return dst;
        }
        else if (src[*i + 1] == '\n') /* Should not happend */
        {
            warnx("Detected a backslash followed by a newline in expansion.");
            return dst;
        }
        else
        {
            // Add backslash
            dst = add_char(dst, src[*i]);
            (*i)++;

            return dst;
        }
    }
}
