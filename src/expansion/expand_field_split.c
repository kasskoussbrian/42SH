#include "expansion/expansion.h"
#include "utils/utils.h"

/* Returns a string of ifs's delimiters which ARE whitespace (<space>, <tab>, or
 * <newline>) */
static char *get_ifs_whitespace(char *ifs)
{
    if (!ifs)
        return strdup(" \t\n");

    char *whitespace_list = calloc(1, sizeof(char));

    for (size_t i = 0; i < strlen(ifs); i++)
    {
        if (ifs[i] == ' ' || ifs[i] == '\t' || ifs[i] == '\n')
        {
            whitespace_list = add_char(whitespace_list, ifs[i]);
        }
    }

    return whitespace_list;
}

static bool is_delim(char curr, char *delim)
{
    for (size_t i = 0; i < strlen(delim); i++)
    {
        if (delim[i] == curr)
        {
            return true;
        }
    }
    return false;
}

char *strip_string(char *start, char *whitespaces)
{
    if (!whitespaces)
        return NULL;

    char *curr = start;

    while (*curr != '\0' && is_delim(*curr, whitespaces))
    {
        curr++;
    }

    char *new = start;

    size_t len = strlen(new);

    char *stripped = strdup(new);

    char *end = stripped + len - 1;

    while (end > stripped && is_delim(*end, whitespaces))
    {
        end--;
    }

    end++;
    *end = '\0';

    return stripped;
}

static char *skip_ifs_whitespace(char *buff, char *delim)
{
    while (*buff != '\0')
    {
        if (!is_delim(*buff, delim))
        {
            break;
        }
        buff++;
    }
    return buff;
}

char *field_splitting(struct expanse *exp, char *output)
{
    // IFS : If unset then value == NULL ; if NULL then value == ""
    char *ifs = get_value_from_parameter(exp, "IFS");

    // Hey ! Let's separate IFS's delimiters in two !
    char *whitespace_ifs = get_ifs_whitespace(ifs);

    char *fields = calloc(1, sizeof(char));

    char *stripped = strip_string(output, whitespace_ifs);

    char *curr = stripped;

    /* Rule 1 :
     * If the value of IFS is a <space>, <tab>, and <newline>,
     * or if it is unset, any sequence of <space>, <tab>, or <newline>
     * characters at the beginning or end of the input shall be ignored and any
     * sequence of those characters within the input shall delimit a field. Met
     * une croix si tu as vraiment lu le texte : x x
     */
    if (!ifs || (strcmp(whitespace_ifs, ifs) == 0 && strlen(ifs) == 3))
    {
        while (*curr != '\0')
        {
            // skip delim whitespace at the beginning
            curr = skip_ifs_whitespace(curr, " \t\n");

            // append until to find the next delim
            while (*curr != '\0')
            {
                if (is_delim(*curr, " \t\n"))
                {
                    fields = add_char(fields, ' ');
                    curr++;
                    break;
                }
                fields = add_char(fields, *curr);
                curr++;
            }
        }
    }
    // Rule 2: doesn't do anything if IFS is unset
    else if (strcmp(ifs, "") == 0)
    {
        free(fields);
        fields = strdup(output);
    }

    /* Rule 3:
     * Otherwise, the following rules shall be applied in sequence :
     * 1) IFS white space shall be ignored at the beginning and end of the
     * input. 2) Each occurrence in the input of an IFS character that is not
     * IFS white space, along with any adjacent IFS white space, shall delimit a
     * field, as described previously. 3) Non-zero-length IFS white space shall
     * delimit a field.
     */
    else
    {
        while (*curr != '\0')
        {
            // skip delim whitespace at the beginning
            curr = skip_ifs_whitespace(curr, whitespace_ifs);

            // append until to find the next delim
            while (*curr != '\0')
            {
                if (is_delim(*curr, ifs))
                {
                    fields = add_char(fields, ' ');
                    curr++;
                    break;
                }
                fields = add_char(fields, *curr);
                curr++;
            }
        }
    }

    if (stripped)
        free(stripped);
    if (whitespace_ifs)
        free(whitespace_ifs);
    if (ifs)
        free(ifs);

    free(output);

    return fields;
}
