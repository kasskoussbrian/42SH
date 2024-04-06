#include "execution/execution.h"
#include "expansion/expand.h"

int evaluate_case(struct exec_args exec_args)
{
    assert(exec_args.ast);
    assert(exec_args.ast->type == AST_CASE);
    struct ast_case *ast_case = (void *)exec_args.ast;

    struct list *it = ast_case->item_case_list;

    char *to_cmp = expand(exec_args.expansion, ast_case->word);
    while (it != NULL)
    {
        // iterate threw each case
        struct ast_item_case *item_case = (void *)it->ast;

        // iterate threw each word of each case
        for (int i = 0; i < item_case->argc; i++)
        {
            char *word = expand(exec_args.expansion, item_case->argv[i]);
            if (strcmp(to_cmp, word) == 0 || strcmp(word, "*") == 0)
            {
                free(word);
                free(to_cmp);
                exec_args.ast = item_case->to_do;
                return evaluate_tree(exec_args);
            }
            free(word);
        }
        it = it->next;
    }
    free(to_cmp);

    return 0;
}
