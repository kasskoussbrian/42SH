#include "expansion/expansion.h"

#include <stdlib.h>
#include <string.h>

#include "expansion/expand.h"
#include "utils/utils.h"

struct expanse *expanse_init(char *variable, char *value)
{
    struct expanse *new = malloc(sizeof(struct expanse));

    if (!new)
        return NULL;

    new->variable = my_strdup(variable);
    new->value = value == NULL ? value : my_strdup(value);
    new->next = NULL;

    return new;
}

struct expanse *expanse_add(struct expanse *exp, char *variable, char *value)
{
    char *env = getenv(variable);
    if (env != NULL)
    {
        setenv(variable, value, 1);
        return exp;
    }

    struct expanse *head = exp;

    if (!head)
    {
        head = expanse_init(variable, value);
        return head;
    }

    while (exp->next && strcmp(exp->variable, variable) != 0)
    {
        exp = exp->next;
    }

    if (strcmp(exp->variable, variable) == 0)
    {
        free(exp->value);
        exp->value = my_strdup(value);
    }
    else
    {
        exp->next = expanse_init(variable, value);
    }

    return head;
}

/*
 * \brief Returns a string representing the value associated to the parameter.
 * Returns NULL if variable is unset.
 * The returned string should be freed if not NULL.
 */
char *get_value_from_parameter(struct expanse *exp, char *parameter)
{
    for (; exp; exp = exp->next)
    {
        if (strcmp(exp->variable, parameter) == 0)
        {
            if (strcmp(exp->variable, "$RANDOM") == 0)
                return generate_random();
            return strdup(exp->value);
        }
    }

    /* If reached, it means we didn't found a variable inside the linked list.
     */
    char *value = getenv(parameter);
    if (value)
        return strdup(value);

    /* If didn't find in environment either, return NULL */
    return NULL;
}

struct expanse *expanse_copy(struct expanse *exp)
{
    struct expanse *new_begin = expanse_init(exp->variable, exp->value);
    struct expanse *new = new_begin;

    exp = exp->next;

    for (; exp; exp = exp->next)
    {
        if (!expanse_add(new, exp->variable, exp->value))
            return NULL;
        new = new->next;
    }

    return new_begin;
}

struct expanse *remove_expanse(struct expanse *del, struct expanse *prev)
{
    if (prev)
        prev->next = del->next;
    else
        prev = del->next; // delete on head

    del->next = NULL;
    expanse_cleanup(del);
    return prev;
}

void expanse_cleanup(struct expanse *exp)
{
    struct expanse *tmp;

    if (!exp)
        return;

    while (exp)
    {
        tmp = exp;
        exp = exp->next;

        if (tmp->variable)
            free(tmp->variable);

        if (tmp->value)
            free(tmp->value);
        free(tmp);
    }
}
