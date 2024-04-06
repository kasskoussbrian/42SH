#include <assert.h>
#include <ctype.h>
#include <err.h>

#include "expansion/expand.h"
#include "expansion/expansion.h"
#include "utils/utils.h"

/* Returns true if the character is a special parameter (SCL 2.5.2) */
bool is_special_character(char c)
{
    return (c == '@' || c == '*' || c == '#' || c == '?' || c == '-' || c == '$'
            || c == '!' || c == '0');
}

/* Returns true if a dollar escape the next character when not quoted */
bool do_dollar_escape_next(char c)
{
    /* Must represent a SCL name */
    return is_special_character(c) || isalpha(c) || c == '_';
}

/*
 * Returns the string representing parameter.
 * argument i should be the index to first char of parameter
 * The function increments i so that i points just after the last letter of
 * parameter.
 */
char *get_parameter_string(char *src, size_t *i)
{
    char *parameter = calloc(1, sizeof(char));
    parameter[0] = '\0';

    /* Check if the character is a special variable */
    if (is_special_character(src[*i]))
    {
        parameter = add_char(parameter, src[*i]);
        (*i)++;
        return parameter;
    }

    /* Check if the parameter is a number */
    else if (isdigit(src[*i]))
    {
        while (isdigit(src[*i]))
        {
            parameter = add_char(parameter, src[*i]);
            (*i)++;
        }
        return parameter;
    }

    /* Check if the parameter is a name */
    else if (isalpha(src[*i]) || src[*i] == '_')
    {
        while (isalpha(src[*i]) || src[*i] == '_' || isdigit(src[*i]))
        {
            parameter = add_char(parameter, src[*i]);
            (*i)++;
        }
        return parameter;
    }

    /* Else, returns an empty string */
    return parameter;
}
