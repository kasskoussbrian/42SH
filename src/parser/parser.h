#pragma once

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "ast/list.h"
#include "lexer/lexer.h"
#include "parse_args/parse_args.h"

struct parser
{
    struct lexer *lexer;
    struct ast *ast;
};

enum parser_status
{
    PARSER_OK,
    PARSER_KO,
    PARSER_MAYBE,
};

int is_reserved(struct token token);

struct parser *parser_init(struct cli_args args);

void parser_cleanup(struct parser *parser);

enum parser_status parse_input(struct parser *parser);

enum parser_status parse_element(struct parser *parser);

enum parser_status parse_simple_command(struct parser *parser);

enum parser_status parse_command(struct parser *parser);

enum parser_status parse_pipeline(struct parser *parser);

enum parser_status parse_and_or(struct parser *parser);

enum parser_status parse_list(struct parser *parser);

enum parser_status parse_input(struct parser *parser);

enum parser_status parse_compound_list(struct parser *parser);

enum parser_status parse_rule_if(struct parser *parser);

enum parser_status parse_else_clause(struct parser *parser);

enum parser_status parse_shell_command(struct parser *parser);

enum parser_status parse_redirection(struct parser *parser);

enum parser_status parse_prefix(struct parser *parser);

enum parser_status parse_while(struct parser *parser);

enum parser_status parse_until(struct parser *parser);

enum parser_status parse_for(struct parser *parser);

enum parser_status parse_func(struct parser *parser, struct token *tok);

enum parser_status parse_case(struct parser *parser);

enum parser_status parse_case_clause(struct parser *parser);

enum parser_status parse_case_item(struct parser *parser);

enum parser_status parse_subshell(struct parser *parser);

int is_word_or_reserved_word(struct token tok);
