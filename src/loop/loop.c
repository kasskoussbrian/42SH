#include "loop/loop.h"

#include "utils/utils.h"

int main_loop(struct exec_args exec_args, struct parser *parser)
{
    enum parser_status status = PARSER_OK;

    int exit_status = EXIT_SUCCESS;
    save_last_status_code(&exec_args, exit_status);

    struct ast_list *storage = ast_init_list();

    do
    {
        // Returns an AST in parser->ast
        status = parse_input(parser);

        // Display ast
        // print_root_ast(parser->ast);
        if (status == PARSER_KO)
        {
            ast_cleanup(parser->ast);
            parser->ast = NULL;
            warnx("Grammatical error");
            exit_status = EXIT_GRAMMAR_ERROR;
            break;
        }

        exec_args.ast = parser->ast;
        if (parser->ast != NULL)
            storage->list = list_add(storage->list, parser->ast);

        // Execute AST
        if (exec_args.ast != NULL)
        {
            exit_status = evaluate_tree(exec_args);
            save_last_status_code(&exec_args, exit_status);

            if (*exec_args.exit) // If we found an exit builtin in the ast
                break;
        }

    } while (lexer_peek(parser->lexer).type != TOKEN_EOF);

    parser->ast = (void *)storage;
    parser_cleanup(parser);

    expanse_cleanup(exec_args.expansion);

    hash_map_free(exec_args.hash_map);
    return exit_status;
}

// Qui est chaud pour clean le code ?
// Mets une croix si t'es d'accord #2015 #college
// x
void save_last_status_code(struct exec_args *exec_args, int exit_status)
{
    int ndigit = 0;

    if (exit_status < 0)
    {
        ndigit++;
    }

    for (int tmp = exit_status; tmp; tmp /= 10)
    {
        ndigit++;
    }

    if (exit_status == 0)
    {
        ndigit = 1;
    }

    char *exit_status_str = calloc(ndigit + 1, sizeof(char));

    exit_status_str = my_itoa(exit_status, exit_status_str);

    exec_args->expansion =
        expanse_add(exec_args->expansion, "?", exit_status_str);

    free(exit_status_str);
}
