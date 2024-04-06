#ifndef AST_H
#define AST_H

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast/list.h"
#include "lexer/token.h"

enum ast_type
{
    AST_LIST,
    AST_SIMPLE_COMMAND,
    AST_REDIR,
    AST_PIPELINE,
    AST_IF,
    AST_SHELL_COMMAND,
    AST_FOR,
    AST_NOT,
    AST_WHILE,
    AST_UNTIL,
    AST_OR,
    AST_AND,
    AST_ASS_WORD,
    AST_FUNC,
    AST_NOT_CODED,
    AST_CASE,
    AST_SUBSHELL,
    AST_ITEM_CASE
};

enum redir_type
{
    REDIR_RIGHT,
    REDIR_LEFT,
    REDIR_RIGHT_RIGHT,
    REDIR_RIGHT_UPSAND,
    REDIR_LEFT_UPSAND,
    REDIR_RIGHT_PIPE,
    REDIR_LEFT_RIGHT,
    REDIR_NOT_FOUND
};

struct ast
{
    enum ast_type type;
};

struct ast_simple_cmd
{
    struct ast base;

    size_t argc;
    char **argv;

    struct list *redir_list;
    struct list *ass_word_list;
};

struct ast_if
{
    struct ast base;
    struct ast_list *condition;
    struct ast_list *then_body;
    struct ast *else_body;
};

struct ast_list
{
    struct ast base;
    struct list *list;
};

struct ast_shell_cmd
{
    struct ast base;

    struct list *redir_list;

    struct list *ass_word_list;

    struct ast *ast;
};

struct ast_redir
{
    struct ast base;

    enum redir_type redir_type;
    int IO_nb;

    char *filename; // A effacer pliustard
    int fd_word;
};

struct ast_pipeline
{
    struct ast base;

    struct ast *left; // left side of the pipeline
    struct ast *right; // right side of the pipeline
};

struct ast_not
{
    struct ast base;

    struct ast *ast;
};

struct ast_while
{
    struct ast base;

    struct ast *condition; // while this
    struct ast *to_do; // do this

    int is_base;
};

struct ast_until
{
    struct ast base;

    struct ast *condition; // while this
    struct ast *to_do; // do this
    int is_base;
};

struct ast_operator
{
    struct ast base;

    struct ast *left;
    struct ast *right;
};

struct ast_for
{
    struct ast base;

    int argc;
    char **argv;

    char *word;
    struct ast *to_do;

    int is_base;
};
struct ast_ass_word
{
    struct ast base;

    char *content;
};

struct ast_func
{
    struct ast base;

    char *name;
    struct ast *function;

    struct list *redir_list;
};

struct ast_case
{
    struct ast base;

    char *word;
    struct list *item_case_list;
};

struct ast_item_case
{
    struct ast base;
    int argc;
    char **argv;

    struct ast *to_do;
};

struct ast_subshell
{
    struct ast base;

    struct ast *to_do;
};

/**
 ** \brief Concat word in the ast's cmd_line
 ** if error with allocation it returns -1;
 */
int ast_concat_word(struct ast *ast, struct token *tok);

/**
 ** \brief Allocate a new IF ast
 */
struct ast_if *ast_init_if(void);

/**
 ** \brief Allocate a new LIST ast
 */
struct ast_list *ast_init_list(void);

/**
 ** \brief Allocate a new SIMPLE_CMD ast
 */
struct ast_simple_cmd *ast_init_simple_cmd(void);

/**
 ** \brief Allocate a new SHELL_CMD ast
 **/
struct ast_shell_cmd *ast_init_shell_cmd(void);

/**
 ** \brief Allocate a new REDIR ast
 **/

struct ast_redir *ast_init_redir(void);

/**
 ** \brief Allocate a new Pipeline ast
 **/
struct ast_pipeline *ast_init_pipeline(void);

/**
 ** \brief Allocate a new NOT ast
 */
struct ast_not *ast_init_not(void);

/**
 ** \brief Allocate new UNTIL  ast
 */
struct ast_until *ast_init_until(void);

/**
 ** \brief Allocate new WHILE ast
 */
struct ast_while *ast_init_while(void);

/**
 ** \brief Allocate new OR ast
 **/

struct ast_operator *ast_init_operator(struct token tok);

/**
 ** \brief Allocate new FOR ast
 **/

struct ast_for *ast_init_for(void);

/**
 ** \brief Allocate new ass_word ast
 **/

struct ast_ass_word *ast_init_ass_word(void);

/**
 ** \brief Allocate new func ast
 **/
struct ast_func *ast_init_func(void);

/**
 ** \brief Allocate new case ast
 **/

struct ast_case *ast_init_case(void);

/**
 ** \brief Allocate new item_case ast
 **/

struct ast_item_case *ast_init_item_case(void);

/**
 ** \brief Allocate new subshell ast
 **/
struct ast_subshell *ast_init_subshell(void);

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//

/**
 ** \brief Recursively free the given ast
 **/

void ast_cleanup(struct ast *ast);
void ast_simple_cmd_cleanup(struct ast *ast);
void ast_list_cleanup(struct ast *ast);
void ast_if_cleanup(struct ast *ast);
void ast_redir_cleanup(struct ast *ast);
void ast_shell_cmd_cleanup(struct ast *ast);
void ast_pipeline_cleanup(struct ast *ast);
void ast_not_cleanup(struct ast *ast);
void ast_while_cleanup(struct ast *ast);
void ast_until_cleanup(struct ast *ast);
void ast_operator_cleanup(struct ast *ast);
void ast_for_cleanup(struct ast *ast);
void ast_ass_word_cleanup(struct ast *ast);
void ast_func_cleanup(struct ast *ast);
void ast_case_cleanup(struct ast *ast);
void ast_item_case_cleanup(struct ast *ast);
void ast_subshell_cleanup(struct ast *ast);

#endif /* (! AST_H ) */
