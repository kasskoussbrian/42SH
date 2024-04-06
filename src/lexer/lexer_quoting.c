#include "lexer/lexer.h"

/* "  asdiodfnwoef "
 * ^
 *  start
 */
enum token_type lexer_quoting(struct lexer *lexer, int curr)
{
    append_char(lexer, curr);
    if (curr == '\\')
    {
        lexing_backslash(lexer);
        return TOKEN_WORD;
    }
    else if (curr == '"')
    {
        if (lexing_double_quote(lexer) == TOKEN_MISSING_SIQUOTE)
        {
            return TOKEN_MISSING_SIQUOTE;
        }
    }
    else if (curr == '\'')
    {
        while (42)
        {
            int curr = io_backend_readchar(lexer->io_backend);
            while (curr != EOF && curr != '\'')
            {
                append_char(lexer, curr);
                curr = io_backend_readchar(lexer->io_backend);
            }
            if (curr == EOF)
            {
                return TOKEN_MISSING_SIQUOTE;
            }

            // add the second single quote
            append_char(lexer, curr);
            return TOKEN_WORD;
        }
    }
    return TOKEN_WORD;
}
