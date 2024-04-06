#include "execution/execution.h"

int evaluate_func(struct exec_args exec_args)
{
    assert(exec_args.ast);
    assert(exec_args.ast->type == AST_FUNC);

    struct ast_func *curr = (void *)exec_args.ast;

    assert(curr->function);

    // here put the function in the hashmap
    hash_map_insert(exec_args.hash_map, curr->name, (void *)curr->function,
                    &exec_args.updated);
    return 0;
}
