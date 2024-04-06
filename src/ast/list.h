#ifndef LIST_H
#define LIST_H

struct ast;

struct list
{
    struct ast *ast;
    struct list *next;
};

struct list *list_add(struct list *list, struct ast *ast);
struct list *list_init(struct ast *ast);
struct list *list_pop(struct list *list);
void list_cleanup(struct list *list);

#endif /* ( ! LIST_H )*/
