#include "lexer/lexer.h"

// if the word's content is a number, return the number otherwise -1
int matching_io_number(char *buff)
{
    int res = 0;
    for (int i = 0; buff[i] != '\0'; i++)
    {
        if (!isdigit(buff[i]))
        {
            return -1;
        }
        res *= 10;
        res += (buff[i] - '0');
    }
    return res;
}

/**
 * \brief NAME :
 * - word[0] != digit
 * - a word consisting of underscores, digits, and alphabetics (from the
 * portable character set)
 */
static bool matching_name(const char *str)
{
    if (str && !isdigit(str[0])) // check rule 1
    {
        int len = strlen(str);
        for (int i = 0; i < len; i++)
        {
            if (!isalnum(str[i]) && str[i] != '_') // check rule 2
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

/**
 * \brief ASSIGNEMENT WORD :
 * - word contains '='
 * - word[0] != '='
 * - word before '=' is a NAME
 */
static enum token_type matching_assignment_word(const char *str)
{
    char *buff = my_strdup(str);
    int i = 0;
    while (buff[i] != '\0'
           && buff[i] != '=') // find the '=' in str if it exists
    {
        i++;
    }
    if (buff[i] != '\0' && i > 0) // check rule 1 et 2
    {
        buff[i] = '\0';
        if (matching_name(buff) == true) // check rule 3
        {
            free(buff);
            return TOKEN_ASSIGNMENT_WORD;
        }
    }
    free(buff);
    return TOKEN_NONE;
}

enum token_type matching_redir(char *buff)
{
    if (strcmp("<", buff) == 0)
    {
        return TOKEN_REDIR_L;
    }
    else if (strcmp(">", buff) == 0)
    {
        return TOKEN_REDIR_R;
    }
    else if (strcmp(">|", buff) == 0)
    {
        return TOKEN_REDIR_R_PIPE;
    }
    else if (strcmp(">>", buff) == 0)
    {
        return TOKEN_REDIR_APPEND;
    }
    else if (strcmp("<&", buff) == 0)
    {
        return TOKEN_REDIR_DUP_INPUT;
    }
    else if (strcmp(">&", buff) == 0)
    {
        return TOKEN_REDIR_DUP_OUTPUT;
    }
    else if (strcmp("<>", buff) == 0)
    {
        return TOKEN_REDIR_OPEN_FILE;
    }
    else
    {
        return TOKEN_NONE;
    }
}

enum token_type matching_operator(char *buff)
{
    enum token_type tmp = {
        0,
    };
    if (strcmp("&&", buff) == 0)
    {
        return TOKEN_AND;
    }
    else if (strcmp("||", buff) == 0)
    {
        return TOKEN_OR;
    }
    else if (strcmp("|", buff) == 0)
    {
        return TOKEN_PIPE;
    }
    else if (strcmp(";", buff) == 0)
    {
        return TOKEN_SEMC;
    }
    else if (strcmp(";;", buff) == 0)
    {
        return TOKEN_DSEMC;
    }
    else if (strcmp("\n", buff) == 0)
    {
        return TOKEN_NEWLINE;
    }
    else if (strcmp("(", buff) == 0)
    {
        return TOKEN_PAR_L;
    }
    else if (strcmp(")", buff) == 0)
    {
        return TOKEN_PAR_R;
    }
    else if ((tmp = matching_redir(buff)) != TOKEN_NONE)
    {
        return tmp;
    }
    else
    {
        return TOKEN_NONE;
    }
}

static enum token_type matching_loop(char *buff)
{
    if (strcmp("while", buff) == 0)
    {
        return TOKEN_WHILE;
    }
    else if (strcmp("until", buff) == 0)
    {
        return TOKEN_UNTIL;
    }
    else if (strcmp("in", buff) == 0)
    {
        return TOKEN_IN;
    }
    else if (strcmp("for", buff) == 0)
    {
        return TOKEN_FOR;
    }
    else if (strcmp("do", buff) == 0)
    {
        return TOKEN_DO;
    }
    else if (strcmp("done", buff) == 0)
    {
        return TOKEN_DONE;
    }
    else
    {
        return TOKEN_NONE;
    }
}

static enum token_type matching_if(char *buff)
{
    if (strcmp("if", buff) == 0)
    {
        return TOKEN_IF;
    }
    else if (strcmp("then", buff) == 0)
    {
        return TOKEN_THEN;
    }
    else if (strcmp("elif", buff) == 0)
    {
        return TOKEN_ELIF;
    }
    else if (strcmp("else", buff) == 0)
    {
        return TOKEN_ELSE;
    }
    else if (strcmp("fi", buff) == 0)
    {
        return TOKEN_FI;
    }
    else
    {
        return TOKEN_NONE;
    }
}

struct token tokenize(struct lexer *lex, char delim)
{
    struct token tok = {
        0,
    };
    enum token_type tmp = {
        0,
    };

    tok.content = lex->buff;
    tok.value = -1;
    if (matching_io_number(lex->buff) != -1 && (delim == '<' || delim == '>'))
    {
        tok.type = TOKEN_IO_NUMBER;
        tok.value = matching_io_number(lex->buff);
    }
    else if (strcmp("!", lex->buff) == 0)
    {
        tok.type = TOKEN_NEGATION;
    }
    else if (strcmp("{", lex->buff) == 0)
    {
        tok.type = TOKEN_LBRACE;
    }
    else if (strcmp("}", lex->buff) == 0)
    {
        tok.type = TOKEN_RBRACE;
    }
    else if (strcmp("case", lex->buff) == 0)
    {
        tok.type = TOKEN_CASE;
    }
    else if (strcmp("esac", lex->buff) == 0)
    {
        tok.type = TOKEN_ESAC;
    }
    else if ((tmp = matching_if(lex->buff)) != TOKEN_NONE)
    {
        tok.type = tmp;
    }
    else if ((tmp = matching_operator(lex->buff)) != TOKEN_NONE)
    {
        tok.type = tmp;
    }
    else if ((tmp = matching_loop(lex->buff)) != TOKEN_NONE)
    {
        tok.type = tmp;
    }
    else if ((tmp = matching_assignment_word(lex->buff)) != TOKEN_NONE)
    {
        tok.type = tmp;
    }
    else
    {
        tok.type = TOKEN_WORD;
        tok.value = matching_io_number(lex->buff);
    }

    /* Reset the lexer buffer to an empty string. */
    lex->buff = calloc(1, sizeof(char));
    lex->buff[0] = '\0';

    return tok;
}
