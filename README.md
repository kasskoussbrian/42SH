# 42sh

## This is the repository of our 42sh project: A POSIX compliant shell.

In memory of Rose, Jose And Klervi. (sans oublier BIEN EVIDEMMENT les big julie et andrea <3)

Were going to fuck everyone (RObin was telling me while writing this thtat the coach could see this : robin shut fthe fuck up )

## NOTES

- 10/01: Created a standard for the parser:
    * Returns the created ast throught the parser structure
    * Set the number of parser status to three: PARSER_OK, PARSER_MAYBE, PARSER_KO. The grammar rule should not check the first \
     symbols of its subrules before calling their functions.

- 13/01: Reworked lexer so that if follows exactly the Shell Common Language documentation.

- 16/01: Created a standard for the strings in the whole code:
    * Every string, when passed to another structure, should be made a new memory allocated copy to the destination structure. \
    No structures should have the same pointer to the same block of memory, because it is error prone.

- 18/01: Created a new standard for the API between lexer and parser: The lexer should free the string representing the token when it pops it, \
    and the parser should not try to access a token between it pops it.
    Also, if the lexer peeked a token, the parser should first use its content, and then pop it to free the token.

- 23/01: Decided that an exit value should be represented by a negative value.

- 25/01: Changed our mind about exit (ambiguity if some commands returns a negative value.) A pointer dedicated to exit is now used in the ast executions

- 25/01: Implemented a recursive rule for the lexing of command substitutions. \
        If nested, command substitutions should be represented as a whole string, the lexer should not create an ast, \
        which is the role of the expansion to create a new lexer. 