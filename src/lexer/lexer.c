#include "lexer/lexer.h"

/**
 * \brief Creates a new lexer given an input string.
 */
struct lexer *lexer_init(struct cli_args args)
{
    struct lexer *lex = calloc(1, sizeof(struct lexer));
    if (!lex)
    {
        fprintf(stderr, "Not enough memory.\n");
        return NULL;
    }
    lex->io_backend = io_backend_init(args);
    if (!lex->io_backend)
    {
        lexer_cleanup(lex);
        return NULL;
    }

    lex->buff = calloc(1, sizeof(char));
    lex->buff[0] = '\0';

    lex->operator= false;
    lex->quoted = false;
    lex->quote_char = '\0';

    lex->current_tok.type = TOKEN_NONE;
    lex->current_tok.content = NULL;
    return lex;
}

/**
 ** \brief Free the given lexer, but not its input.
r *input; // The input data
  1     size_t pos; // The c */
void lexer_cleanup(struct lexer *lexer)
{
    if (!lexer)
        return;

    io_backend_cleanup(lexer->io_backend);

    if (lexer->buff)
        free(lexer->buff);

    if (lexer->current_tok.content)
        free(lexer->current_tok.content);

    free(lexer);
}

/* Applies all rules of the Token Recognition of the SCL */
struct token parse_input_for_tok(struct lexer *lexer)
{
    assert(lexer);
    struct token tok;

    /* Hahaha 42 goes brrrr */
    while (42)
    {
        int len_buff = strlen(lexer->buff);
        int curr = io_backend_readchar(lexer->io_backend);

        /* RULE 1 */
        if (curr == EOF)
        {
            return end_of_lexing(lexer, curr);
        }

        /* RULE 2 */
        if (lexer->operator&& can_create_operator(lexer->buff, curr))
        {
            append_char(lexer, curr);
        }

        /* RULE 3 */
        else if (lexer->operator&& !can_create_operator(lexer->buff, curr))
        {
            if ((tok = delim_operator(lexer, curr)).type != TOKEN_NONE)
            {
                return tok;
            }
        }

        /* RULE 4 */
        else if (is_quote(curr))
        {
            if (lexer_quoting(lexer, curr) == TOKEN_MISSING_SIQUOTE)
            {
                return missing_quote(lexer);
            }
        }

        /* RULE 5 */
        else if (curr == '`') // CASE : BACKQUOTE
        {
            if (lexing_backquoted(lexer, curr) == TOKEN_MISSING_SIQUOTE)
            {
                return missing_quote(lexer);
            }
        }

        // CASE : AFTER DOLLAR -> { , ( or name
        else if (len_buff > 0 && lexer->buff[len_buff - 1] == '$'
                 && (curr == '{' || curr == '('))
        {
            append_char(lexer, curr);
            if (lexing_after_dollar(lexer, curr) == TOKEN_MISSING_SIQUOTE)
            {
                return missing_quote(lexer);
            }
        }

        /* RULE 6 */
        else if (!lexer->operator&& first_char_of_an_operator(curr, lexer))
        {
            if ((tok = delim_word_and_begin_operator(lexer, curr)).type
                != TOKEN_NONE)
            {
                return tok;
            }
        }

        /* RULE 7 */
        else if (isblank(curr))
        {
            /* Rule 0, should not delimit empty tokens */
            if (strlen(lexer->buff) != 0)
            {
                return tokenize(lexer, curr);
            }
        }

        /* RULE 8 */
        else if (len_buff > 0)
        {
            append_char(lexer, curr);
        }

        /* RULE 9 */
        else if (curr == '#')
        {
            // Skip all subsequent characters
            return skip_comment(lexer, curr);
        }

        /* RULE 10 */
        else
        {
            append_char(lexer, curr);
        }
    }
}

/**
 * \brief Returns the next token, but doesn't move forward: calling
 * lexer_peek multiple times in a row always returns the same result.
 * This functions is
 */
struct token lexer_peek(struct lexer *lexer)
{
    if (lexer->current_tok.type == TOKEN_NONE)
    {
        lexer->current_tok = parse_input_for_tok(lexer);
        return lexer->current_tok;
    }
    return lexer->current_tok;
}

/**
 * \brief Returns the next token, and removes it from the stream:
 *   calling lexer_pop in a loop will iterate over all tokens until EOF.
 */
void lexer_pop(struct lexer *lexer)
{
    lexer_peek(lexer);

    if (lexer->current_tok.content)
    {
        free(lexer->current_tok.content);
        lexer->current_tok.content = NULL;
    }

    lexer->current_tok.type = TOKEN_NONE;
}
