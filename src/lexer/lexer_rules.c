#include "lexer/lexer.h"
#include "utils/utils.h"

/* RULE 1 */
struct token end_of_lexing(struct lexer *lexer, char curr)
{
    if (strcmp(lexer->buff, "") == 0)
    {
        return end_of_file(lexer);
    }
    return tokenize(lexer, curr);
}

/* RULE 3 */
struct token delim_operator(struct lexer *lexer, char curr)
{
    struct token tok;
    if (!first_char_of_an_operator(curr, lexer))
    {
        lexer->operator= false;
    }

    /* Rule 0, should not delimit empty tokens */
    if (strlen(lexer->buff) == 0)
    {
        append_char(lexer, curr);
        tok.type = TOKEN_NONE;
        return tok;
    }
    tok = tokenize(lexer, curr);

    if (!isblank(curr))
        append_char(lexer, curr);
    return tok;
}

// Skip all subsequent characters
enum token_type lexing_comment(struct lexer *lexer, char curr)
{
    while (curr != '\n' && curr != EOF)
    {
        curr = io_backend_readchar(lexer->io_backend);
        lexer->buff = add_char(lexer->buff, curr);
    }
    if (curr == EOF)
    {
        return TOKEN_MISSING_SIQUOTE;
    }
    /* we need the '\n' */
    lexer->buff = add_char(lexer->buff, curr);
    return TOKEN_WORD;
}

/* RULE 5 : $(SUBSHELL) OR ${COMMAND} */
enum token_type lexing_after_dollar(struct lexer *lexer, char curr)
{
    if (curr == '(')
    {
        enum token_type child_result = lex_command_substitution(lexer, ')');
        if (child_result == TOKEN_MISSING_SIQUOTE)
        {
            return TOKEN_MISSING_SIQUOTE; // LEXING KO
        }
    }
    else if (curr == '{')
    {
        enum token_type child_result = lex_param_expansion(lexer);
        if (child_result == TOKEN_MISSING_SIQUOTE)
        {
            return TOKEN_MISSING_SIQUOTE; // LEXING KO
        }
    }
    return TOKEN_WORD; // LEXING OK
}

/* RULE 6 */
struct token delim_word_and_begin_operator(struct lexer *lexer, char curr)
{
    lexer->operator= true;

    struct token tok;
    /* Rule 0, should not delimit empty tokens */
    if (strlen(lexer->buff) == 0)
    {
        append_char(lexer, curr);
        tok.type = TOKEN_NONE;
        return tok;
    }
    tok = tokenize(lexer, curr);

    if (!isblank(curr))
        append_char(lexer, curr);

    return tok;
}

/* RULE 9 */
struct token skip_comment(struct lexer *lexer, char curr)
{
    while (curr != '\n' && curr != EOF)
    {
        curr = io_backend_readchar(lexer->io_backend);
    }
    if (curr == EOF)
    {
        return end_of_file(lexer);
    }
    append_char(lexer, curr);
    return tokenize(lexer, curr);
}
