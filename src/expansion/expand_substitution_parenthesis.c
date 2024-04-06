#include <assert.h>
#include <err.h>
#include <sys/wait.h>

#include "execution/execution.h"
#include "expansion/expand.h"
#include "expansion/expansion.h"
#include "lexer/lexer.h"
#include "loop/loop.h"
#include "parse_args/parse_args.h"
#include "parser/parser.h"
#include "utils/utils.h"

char *expand_substitution_parenthesis(struct expand_args *args,
                                      bool double_quoted)
{
    char *dst = args->dst;

    char *command = calloc(1, sizeof(char));
    command[0] = '\0';

    args->dst = command;

    command = get_enclosed_substitution_parenthesis(args);

    if (!command)
    {
        free(dst);
        return NULL;
    }

    size_t cmd_len = strlen(command);

    // Remove the dollar start and end parenthesis
    command = memmove(command, command + 2, cmd_len - 2);

    command[cmd_len - 3] = '\0';

    args->dst = dst;

    char *output = get_command_output(args, command);

    // Do field splitting if not in dquote
    if (!double_quoted)
    {
        output = field_splitting(args->exp, output);
    }

    // Concat dst and output
    size_t output_len = strlen(output);
    size_t dst_len = strlen(args->dst);
    args->dst = realloc(args->dst, (dst_len + output_len + 1) * sizeof(char));
    args->dst = strcat(args->dst, output);

    free(output);
    free(command);

    return args->dst;
}

char *get_enclosed_comment(struct expand_args *args)
{
    char *src = args->src;

    size_t *i = &args->i;

    // Skip all subsequent characters
    while (src[*i] && src[*i] != '\n')
    {
        args->dst = add_char(args->dst, src[*i]);
        (*i)++;
    }
    if (!src[*i])
    {
        free(args->dst);
        return NULL;
    }
    /* we need the '\n' */
    args->dst = add_char(args->dst, src[*i]);

    return args->dst;
}

/*
 * Called on the first character after the parenthesis, get all characters until
 * the parenthesis excluded.
 * $(abcdeeeeeeef )
 * ^               ^
 * start           end
 * i should point on the closing parenthesis
 */
char *get_enclosed_substitution_parenthesis(struct expand_args *args)
{
    char *src = args->src;

    size_t *i = &args->i;

    // Add the dollar and the parenthesis
    args->dst = add_char(args->dst, src[*i]);
    args->dst = add_char(args->dst, src[*i + 1]);
    (*i) += 2;

    /* Hahaha 42 goes brrrr */
    while (42)
    {
        /* RULE 1, means missing parenthesiss */
        if (!src[*i])
        {
            free(args->dst);
            return NULL;
        }

        /* RULE 9 */
        else if (src[*i] == '#')
        {
            args->dst = get_enclosed_comment(args);
        }

        /* No need to delimit operators, so no need of rule 2 3 6*/
        /* No need to skip whitespace so no need of rule 7 */
        /* No need to rule 8 10 because we append every char */
        else if (src[*i] == '\\')
        {
            args->dst = get_enclosed_backslash(args, false);
        }
        else if (src[*i] == '"')
        {
            args->dst = get_enclosed_double_quote(args);
        }
        else if (src[*i] == '\'')
        {
            args->dst = get_enclosed_simple_quote(args);
        }
        else if (src[*i] == '`')
        {
            args->dst = get_enclosed_substitution_backtick(args);
        }
        else if (src[*i] == '$' && (src[*i + 1]) == '(')
        {
            args->dst = get_enclosed_substitution_parenthesis(args);
        }
        else if (src[*i] == '$' && src[*i + 1] == '{') /* {parameter} form */
        {
            args->dst = get_enclosed_parameter_braces(args);
        }
        else if (src[*i] == '$') /* $parameter form */
        {
            args->dst = get_enclosed_parameter_normal(args);
        }
        else
        {
            /* Append every character to the buffer */
            args->dst = add_char(args->dst, src[*i]);

            if (src[*i] == ')')
            {
                (*i)++;

                return args->dst;
            }

            (*i)++;
        }

        /* We need to check if dst was returned null in one of the functions
         * call above */
        if (!args->dst)
            return NULL;
    }
}
