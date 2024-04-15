# 42sh

## This is the repository of our 42sh project: A POSIX compliant shell.

We wanted to beat everyone and thats what we did. @robin | @teo | @arthur | @ brian


# How to run the shell ? :

1 ) autoreconf --install ( make sure to have the latest version installed)

2 ) ./configure

3 ) make or make check to run the testsuite

Now that you have the binary you can run these command: 
./42sh -c {the command as a string}
./42sh {the bash file you want to run}
./42sh to lauch the interactive terminal ( It shouldn't work on all the commands there are some bugs to fix)



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
