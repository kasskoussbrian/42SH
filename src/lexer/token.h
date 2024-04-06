#pragma once

#include <unistd.h>

enum token_type
{
    // TOKEN INITIAL
    TOKEN_NONE,

    // TOKENS IF
    TOKEN_IF, // if
    TOKEN_THEN, // then
    TOKEN_ELIF, // elif
    TOKEN_ELSE, // else
    TOKEN_FI, // fi

    // TOKENS LOOP
    TOKEN_WHILE, // while
    TOKEN_UNTIL, // until
    TOKEN_FOR, // for
    TOKEN_DO, // do
    TOKEN_IN, // in
    TOKEN_DONE, // done

    // TOKEN PAS LOOP NI OPERATOR (switch ducoup xd)
    TOKEN_CASE, // case
    TOKEN_ESAC, // esac (a pas confondre avec le REDIR_DUP_OUTPUT)

    // TOKENS OPERATOR
    TOKEN_AND, // &&
    TOKEN_OR, // ||
    TOKEN_SEMC, // ;
    TOKEN_DSEMC, // ;;
    TOKEN_NEWLINE, // \n
    TOKEN_PIPE, // |
    TOKEN_PAR_L, // (
    TOKEN_PAR_R, // )

    // TOKENS REDIRECTION
    TOKEN_REDIR_L, // <
    TOKEN_REDIR_R, // >
    TOKEN_REDIR_R_PIPE, // >|
    TOKEN_REDIR_APPEND, // >>
    TOKEN_REDIR_DUP_INPUT, // <&
    TOKEN_REDIR_DUP_OUTPUT, // >&
    TOKEN_REDIR_OPEN_FILE, // <>

    // OTHERS TOKENS
    TOKEN_NEGATION, // !
    TOKEN_LBRACE, // {
    TOKEN_RBRACE, // }
    TOKEN_IO_NUMBER, // io number #logique
    TOKEN_ASSIGNMENT_WORD, // assignment word #naaaaanJUUUUURREEE
    TOKEN_WORD, // WORD

    // TOKENS END OF FILE
    TOKEN_EOF, // EOF
    TOKEN_MISSING_SIQUOTE // error : if missing single quote
};

struct token
{
    enum token_type type; // The kind of token
    char *content; // If the token is a word
    int value; // If the word is a IO_NUMBER
};
