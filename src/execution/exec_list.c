#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#include "execution/execution.h"

int evaluate_list(struct exec_args exec_args)
{
    assert(exec_args.ast);
    assert(exec_args.ast->type == AST_LIST);

    struct ast_list *curr = (struct ast_list *)exec_args.ast;
    assert(curr->list);

    struct list *list = curr->list;
    int status = EXIT_SUCCESS;
    while (list)
    {
        /*
         * NOTE: We might change the ast type in the list structure to be
         * a defined ast type and not the gereric 'ast' one.
         */
        exec_args.ast = list->ast;
        int tmp_status = status;
        status = evaluate_tree(exec_args);
        if (*exec_args.exit || *exec_args.continue_val
            || *exec_args.break_val) // If found exit builtin
        {
            return ((status < 0) ? tmp_status : status);
        }

        list = list->next;
    }
    return status;
}
