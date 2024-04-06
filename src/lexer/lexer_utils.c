#include <err.h>
#include <string.h>

#include "lexer/lexer.h"

struct token end_of_file(struct lexer *lex)
{
    if (lex->quoted)
    {
        warnx("expected an end of quote");
        return missing_quote(lex);
    }
    struct token tok = { .type = TOKEN_EOF, .content = NULL, .value = -1 };
    lex->current_tok = tok;
    return tok;
}

struct token missing_quote(struct lexer *lex)
{
    struct token tok = { .type = TOKEN_MISSING_SIQUOTE,
                         .content = NULL,
                         .value = -1 };
    lex->current_tok = tok;
    return tok;
}

/*
 * Append the character c to the buffer 'buff' in the lexer if c is not blank,
 * otherwise does nothing.
 */
void append_char(struct lexer *lexer, char c)
{
    // ACU "regle d'or de la prog : aller au + simple"
    int buffer_len = strlen(lexer->buff) + 1; // DONC TG ROBIN
    lexer->buff = realloc(lexer->buff, (buffer_len + 1) * sizeof(char));
    lexer->buff[buffer_len - 1] = c;
    lexer->buff[buffer_len] = '\0';
}

bool can_create_operator(char *buff, char c)
{
    assert(buff);

    int s1 = strlen(buff);
    char *concat = calloc(s1 + 1 + 1, sizeof(char));
    memcpy(concat, buff, s1);
    memcpy(concat + s1, &c, 1);
    concat[s1 + 1] = '\0';

    int can_create = matching_operator(concat) != TOKEN_NONE;

    free(concat);

    return can_create;
}

bool is_quote(char c)
{
    return c == '\'' || c == '"' || c == '\\';
}

bool first_char_of_an_operator(char c, struct lexer *lex)
{
    if (c == '(')
    {
        int len_buff = strlen(lex->buff);
        if (len_buff > 0 && lex->buff[len_buff - 1] == '$')
        {
            return false;
        }
    }
    return c == '&' || c == '|' || c == '>' || c == '<' || c == '\n' || c == ';'
        || c == '(' || c == ')';
}
