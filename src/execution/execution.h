#ifndef EXECUTION_H
#define EXECUTION_H

#include "ast/ast.h"
#include "ast/list.h"
#include "expansion/expansion.h"
#include "hash_map/hash_map.h"

// EXIT_CODES
#define EXIT_CLI_ARGS_ERROR 2
#define EXIT_GRAMMAR_ERROR 2
#define EXIT_COMMAND_UNFOUND 127

struct exec_args
{
    struct ast *ast;
    struct expanse *expansion;
    int *exit;
    struct hash_map *hash_map;
    int *continue_val;
    int *break_val;
    bool updated;
    int *is_in_base;
};

int evaluate_list(struct exec_args exec_args); //
int evaluate_tree(struct exec_args exec_args); //
int evaluate_simple_cmd(struct exec_args exec_args);
int evaluate_if(struct exec_args exec_args); //
int evaluate_shell_cmd(struct exec_args exec_args); //
int evaluate_pipe_line(struct exec_args exec_args); //
int evaluate_redir(struct list *list, struct exec_args exec_args); //
int evaluate_not(struct exec_args exec_args); //
int evaluate_while(struct exec_args exec_args); //
int evaluate_until(struct exec_args exec_args); //
int evaluate_operator(struct exec_args exec_args); //
int evaluate_for(struct exec_args exec_args); //
int evaluate_func(struct exec_args exec_args);
int evaluate_case(struct exec_args exec_args);

/* Execute argv of simple cmd */
int builtin_or_not(struct exec_args exec_args);

/* Remove empty fiels to argv */
void remove_empty_fields_argv(struct ast_simple_cmd *curr);

char *delim_variable(char *str);
char *delim_value(char *str);
void assign_variables(struct ast *ast, struct expanse *expansion);
void assign_environment(struct ast *ast, struct expanse *expansion);
char *concat_variable_value(char *variable, char *value);
void expand_assignment_word(struct ast_simple_cmd *ast,
                            struct expanse *expansion);
void restore_environment_variables(struct expanse *save);
struct expanse *save_environment_variables(struct ast_simple_cmd *ast);
void add_to_env(struct ast_simple_cmd *ast);

#endif /* ( ! EXECUTION_H ) */
