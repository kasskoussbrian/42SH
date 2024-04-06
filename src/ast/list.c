#include "ast/list.h"

#include <stdlib.h>

#include "ast/ast.h"

struct list *list_init(struct ast *ast)
{
    struct list *toret = calloc(1, sizeof(struct list));
    if (!toret)
        return NULL;
    toret->ast = ast;
    toret->next = NULL;
    return toret;
}
/*
 * return new list with a new ast
 */

struct list *list_add(struct list *list, struct ast *ast)
{
    if (!list)
        return list_init(ast);

    struct list *toret = list;
    while (list->next != NULL)
    {
        list = list->next;
    }
    list->next = list_init(ast);
    return toret;
}
/*
 * return new list , adding a new element ast
 */

struct list *list_pop(struct list *list)
{
    if (!list || !list->ast)
        return NULL;
    struct list *toret = list->next;
    ast_cleanup(list->ast);
    free(list);
    return toret;
}
/*
 * pop first element (first command)
 * return the new list
 */

void list_cleanup(struct list *list)
{
    while (list != NULL)
    {
        list = list_pop(list);
    }
    return;
}
