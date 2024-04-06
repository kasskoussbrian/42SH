#ifndef EXPANSION_H
#define EXPANSION_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <utils/utils.h>

struct expanse
{
    char *variable; /* Pointer to a string that is dynamically allocated */
    char *value; /* Pointer to a string that is dynamically allocated */
    struct expanse *next;
};

struct expanse *expanse_init_var(int argc, char *argv[]);

struct expanse *expanse_init(char *variable, char *value);

struct expanse *get_expanse(struct expanse *exp, char *text);

struct expanse *expanse_add(struct expanse *exp, char *variable, char *value);

struct expanse *expanse_copy(struct expanse *exp);

struct expanse *remove_expanse(struct expanse *del, struct expanse *prev);

char *get_value_from_parameter(struct expanse *exp, char *parameter);

void expanse_cleanup(struct expanse *exp);

char *field_splitting(struct expanse *exp, char *output);

#endif /* ( ! EXPANSION_H ) */
