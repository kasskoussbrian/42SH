#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#include "execution/builtin/builtin.h"
#include "execution/execution.h"
#include "expansion/expand.h"
#include "expansion/expansion.h"

char *delim_variable(char *str)
{
    int i = 0;
    char *buff = calloc(1, sizeof(char));
    while (str[i] != '\0' && str[i] != '=')
    {
        buff = add_char(buff, str[i]);
        i++;
    }
    return buff;
}

char *delim_value(char *str)
{
    int i = 0;
    char *buff = calloc(1, sizeof(char));
    while (str[i] != '\0' && str[i] != '=')
    {
        i++;
    }

    // skip '='
    i++;
    while (str[i] != '\0')
    {
        buff = add_char(buff, str[i]);
        i++;
    }
    return buff;
}

// copy assigmnent variables into the variables list of PALUSCI
void assign_variables(struct ast *ast, struct expanse *expansion)
{
    struct ast_simple_cmd *curr = (struct ast_simple_cmd *)ast;
    struct list *list = curr->ass_word_list;

    struct ast_ass_word *ass = NULL;

    while (list != NULL)
    {
        ass = (void *)list->ast;
        char *variable = delim_variable(ass->content);
        char *value = delim_value(ass->content);
        expansion = expanse_add(expansion, variable, value);

        free(value);
        free(variable);

        list = list->next;
    }
}

char *concat_variable_value(char *variable, char *value)
{
    int variable_len = strlen(variable);
    int value_len = strlen(value);

    char *new = calloc(variable_len + 1 + value_len + 1, sizeof(char));
    sprintf(new, "%s=%s", variable, value);
    return new;
}

void expand_assignment_word(struct ast_simple_cmd *ast,
                            struct expanse *expansion)
{
    struct list *list = ast->ass_word_list;

    struct ast_ass_word *ass = NULL;

    while (list != NULL)
    {
        ass = (void *)list->ast;
        char *variable = delim_variable(ass->content);
        char *value = delim_value(ass->content);

        char *expanded = expand(expansion, value);

        char *concat = concat_variable_value(variable, expanded);

        free(variable);
        free(value);
        free(expanded);

        free(ass->content);
        ass->content = concat;
        list = list->next;
    }
}

struct expanse *save_environment_variables(struct ast_simple_cmd *ast)
{
    struct list *list = ast->ass_word_list;
    struct expanse *save = NULL;
    struct ast_ass_word *ass = NULL;

    while (list != NULL)
    {
        ass = (void *)list->ast;

        char *variable = delim_variable(ass->content);
        char *value = getenv(variable);

        save = expanse_add(save, variable, value);

        free(variable);

        list = list->next;
    }

    return save;
}

void add_to_env(struct ast_simple_cmd *ast)
{
    assert(ast);

    struct list *list = ast->ass_word_list;
    struct ast_ass_word *ass = NULL;

    while (list != NULL)
    {
        ass = (void *)list->ast;

        char *variable = delim_variable(ass->content);
        char *value = getenv(variable);
        if (value)
            setenv(variable, value, 1);

        free(variable);

        list = list->next;
    }
}

void restore_environment_variables(struct expanse *save)
{
    if (!save)
        return;

    while (save != NULL)
    {
        char *variable = save->variable;
        char *value = save->value;

        if (value == NULL)
        {
            unsetenv(variable);
        }
        else
        {
            setenv(variable, value, 1);
        }

        save = save->next;
    }

    expanse_cleanup(save);
}
