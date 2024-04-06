#pragma once

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <utils/utils.h>

#include "io_backend/io_backend.h"
#include "lexer/token.h"
#include "parse_args/parse_args.h"

struct lexer
{
    struct token current_tok;
    struct io_backend *io_backend;
    char *buff;
    bool quoted;
    bool operator;
    char quote_char;
};

struct lexer *lexer_init(struct cli_args args);

/**
 ** \brief Free the given lexer, but not its input.
 */
void lexer_cleanup(struct lexer *lexer);

/**
 * \brief Returns a token from the input string
 */
struct token parse_input_for_tok(struct lexer *lexer);

/**
 * \brief Returns the next token, but doesn't move forward: calling lexer_peek
 * multiple times in a row always returns the same result. This functions is
 * meant to help the parser check if the next token matches some rule.
 */
struct token lexer_peek(struct lexer *lexer);

/**
 * \brief Removes the current token from the stream:
 */
void lexer_pop(struct lexer *lexer);

/* LEXER UTILS */
struct token end_of_file(struct lexer *lex);
void append_char(struct lexer *lex, char c);
bool is_quote(char c);
bool first_char_of_an_operator(char c, struct lexer *lex);
bool can_create_operator(char *buff, char c);
struct token missing_quote(struct lexer *lex);

/* LEXER MATCHING TYPE */
struct token tokenize(struct lexer *lexer, char delim);
int matching_io_number(char *buff);
enum token_type matching_operator(char *buff);

/* LEXER RULES */
struct token end_of_lexing(struct lexer *lexer, char curr); // R1
struct token delim_operator(struct lexer *lexer, char curr); // R3
enum token_type lexing_backquoted(struct lexer *lexer, char curr); // R5
enum token_type lexing_after_dollar(struct lexer *lexer, char curr); // R5
struct token delim_word_and_begin_operator(struct lexer *lexer,
                                           char curr); // R6
struct token skip_comment(struct lexer *lexer, char curr); // R9

/* REWORK LEXER */
void lexing_backslash(struct lexer *lexer);
enum token_type lexing_comment(struct lexer *lexer, char curr);
enum token_type lexing_double_quote(struct lexer *lexer);
enum token_type lexer_quoting(struct lexer *lexer, int curr);
enum token_type lex_param_expansion(struct lexer *lexer);
enum token_type lexing_backquoted(struct lexer *lexer, char curr);
enum token_type lex_command_substitution(struct lexer *lexer, char delimiter);
