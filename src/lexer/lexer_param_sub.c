#include "lexer/lexer.h"

/* RULE 5 : lex parameter expansion --> ${PARAMETER} */
enum token_type lex_param_expansion(struct lexer *lexer)
{
    int curr = io_backend_readchar(lexer->io_backend);
    while (curr != EOF && curr != '}')
    {
        append_char(lexer, curr);
        curr = io_backend_readchar(lexer->io_backend);
    }
    if (curr == EOF)
    {
        return TOKEN_MISSING_SIQUOTE;
    }
    append_char(lexer, curr); // Add '}' into lexer->buff
    return TOKEN_WORD;
}
