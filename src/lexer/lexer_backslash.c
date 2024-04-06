#include <lexer/lexer.h>

/*  \$
 *   ^
 *   start
 */

// Return rien car ya jamais d'erreur c carre
void lexing_backslash(struct lexer *lexer)
{
    char curr = io_backend_readchar(lexer->io_backend);
    if (curr != EOF)
    {
        if (curr == '\n') // on supprime \ et skip \n
        {
            int len = strlen(lexer->buff);
            lexer->buff[len - 1] = '\0';
        }
        else
        {
            append_char(lexer, curr);
        }
    }
}
