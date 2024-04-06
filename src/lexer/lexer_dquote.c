#include <lexer/lexer.h>

bool do_backslash_escape_in_dquote(char c)
{
    return c == '$' || c == '`' || c == '"'
        || c == '\\'; // \\n est gerer dans le lexeur habibi
}

/*              end
 *               v
 * "nik les acus"pd
 *  ^ start
 *
 * Returns :
 * - TOKEN_WORD if lexing OK
 * - TOKEN_MISSING_QUOTE directly if one missing quote or brace is found
 */
enum token_type lexing_double_quote(struct lexer *lexer)
{
    while (42)
    {
        int len_buff = strlen(lexer->buff);
        int curr = io_backend_readchar(lexer->io_backend);
        if (curr == EOF)
        {
            return TOKEN_MISSING_SIQUOTE;
        }
        else if (curr == '"') // END
        {
            append_char(lexer, curr);
            return TOKEN_WORD;
        }
        else if (curr == '\\')
        {
            append_char(lexer, curr);
            lexing_backslash(lexer);
        }
        else if (curr == '`')
        {
            if (lexing_backquoted(lexer, curr) == TOKEN_MISSING_SIQUOTE)
            {
                return TOKEN_MISSING_SIQUOTE;
            }
        }
        // CASE : AFTER DOLLAR -> { , ( or name
        else if (len_buff > 0 && lexer->buff[len_buff - 1] == '$'
                 && (curr == '{' || curr == '('))
        {
            append_char(lexer, curr);
            if ((lexing_after_dollar(lexer, curr)) == TOKEN_MISSING_SIQUOTE)
            {
                return TOKEN_MISSING_SIQUOTE;
            }
        }
        else
        {
            append_char(lexer, curr);
        }
    }
}
