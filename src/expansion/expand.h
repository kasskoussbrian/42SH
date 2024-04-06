#ifndef EXPAND_H
#define EXPAND_H

#include "expansion/expansion.h"

struct expand_args
{
    char *dst;
    char *src;
    size_t i;
    struct expanse *exp;
};

char *expand(struct expanse *exp, char *text);

char *expand_simple_quote(struct expand_args *expand_args);

char *get_enclosed_simple_quote(struct expand_args *expand_args);

char *expand_backslash(struct expand_args *expand_args, bool double_quoted);

char *get_enclosed_backslash(struct expand_args *expand_args,
                             bool double_quoted);

char *expand_double_quote(struct expand_args *expand_args);

char *get_enclosed_double_quote(struct expand_args *expand_args);

char *expand_parameter_normal(struct expand_args *expand_args,
                              bool double_quoted);

char *get_enclosed_parameter_normal(struct expand_args *args);

char *expand_parameter_braces(struct expand_args *expand_args,
                              bool double_quoted);

char *get_enclosed_parameter_braces(struct expand_args *args);

char *get_parameter_string(char *src, size_t *i);

bool do_dollar_escape_next(char c);

bool is_special_character(char c);

char *expand_substitution_parenthesis(struct expand_args *expand_args,
                                      bool double_quoted);

char *get_enclosed_substitution_parenthesis(struct expand_args *args);

char *expand_substitution_backtick(struct expand_args *expand_args,
                                   bool double_quoted);

char *get_enclosed_substitution_backtick(struct expand_args *args);

char *expand_normal(struct expand_args *args);

char *read_everything(int fd, size_t *size);

char *get_command_output(struct expand_args *expand_args, char *command);

void exec_main_loop_with_command(struct expand_args *expand_args,
                                 char *command);

#endif /* (! EXPAND_H )*/
