#include <lexer/lexer.h>

/* RULE 5 : RECURSIVE FUNCTION */
/*
 * \brief Applies all rules of the Token Recognition of the SCL to find
 delimiter
 * that opened a subshell. The delimiter can be either ')' or '`'
 * Append to lexer->buff all the content of the subshell, including the dollar
 and parenthesis
 * Returns TOKEN_WORD if lexed the subshell correctly,
 * Returns MISSING QUOTE if:
     - couldn't find the matching delimiter of the subshell
     - couldn't find a quote delimiter that was opened in the subshell
 */
enum token_type lex_command_substitution(struct lexer *lexer, char delimiter)
{
    assert(lexer);
    assert(delimiter == '`' || delimiter == ')');
    int len_buff = 0;

    /* Hahaha 42 goes brrrr */
    while (42)
    {
        int curr = io_backend_readchar(lexer->io_backend);

        len_buff = strlen(lexer->buff); // pas optimise du tout :((

        /* RULE 1 */
        if (curr == EOF)
        {
            return TOKEN_MISSING_SIQUOTE;
        }

        /* No need to delimiter operators, so no need of rule 2 3 6*/
        /* No need to skip whitespace so no need of rule 7 */
        /* No need to rule 8 10 because we append every char */

        /* RULE 4 */
        if (is_quote(curr))
        {
            if (lexer_quoting(lexer, curr) == TOKEN_MISSING_SIQUOTE)
            {
                return TOKEN_MISSING_SIQUOTE;
            }
        }

        /* Append every character to the buffer except EOF */
        else
        {
            append_char(lexer, curr);
        }

        /* RULE 9 */
        if (curr == '#')
        {
            if (lexing_comment(lexer, curr) == TOKEN_MISSING_SIQUOTE)
            {
                return TOKEN_MISSING_SIQUOTE;
            }
        }

        /* End of rule 5 */
        else if (curr == delimiter)
        {
            return TOKEN_WORD;
        }

        /* Start of rule 5 */
        else if (curr == '`')
        {
            enum token_type child_result = lex_command_substitution(lexer, '`');
            if (child_result == TOKEN_MISSING_SIQUOTE)
            {
                return TOKEN_MISSING_SIQUOTE;
            }
        }

        // Rule 5 : After Dollar...
        else if (len_buff > 0 && lexer->buff[len_buff - 1] == '$'
                 && (curr == '{' || curr == '('))
        {
            if (lexing_after_dollar(lexer, curr) == TOKEN_MISSING_SIQUOTE)
            {
                return TOKEN_MISSING_SIQUOTE;
            }
        }
    }
}

/* RULE 5 : ` ` */
enum token_type lexing_backquoted(struct lexer *lexer, char curr)
{
    append_char(lexer, curr);
    enum token_type child_result = lex_command_substitution(lexer, '`');
    if (child_result == TOKEN_MISSING_SIQUOTE)
    {
        return TOKEN_MISSING_SIQUOTE; // LEXING KO
    }
    return TOKEN_WORD; // LEXING OK
}
