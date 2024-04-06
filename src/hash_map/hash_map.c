#include "hash_map/hash_map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hash_map *hash_map_init(size_t size)
{
    struct hash_map *h = malloc(sizeof(struct hash_map));
    if (!h)
        return NULL;

    h->size = size;
    h->data = malloc(sizeof(struct pair_list *) * size);
    for (size_t i = 0; i < size; i++)
        h->data[i] = NULL;

    return h;
}

bool hash_map_insert(struct hash_map *hash_map, const char *key,
                     struct ast *ast, bool *updated)
{
    if (!hash_map || hash_map->size == 0)
        return false;

    size_t index = hash(key) % hash_map->size;

    struct pair_list *pl = hash_map->data[index];
    struct pair_list *head = pl;

    for (; pl && strcmp(pl->key, key) != 0; pl = pl->next)
    {}
    if (!pl)
    {
        struct pair_list *new = malloc(sizeof(struct pair_list));
        if (!new)
            return false;
        new->key = key;
        new->ast = ast;
        new->next = head;
        if (updated)
            *updated = false;
        hash_map->data[index] = new;
        return true;
    }
    pl->ast = ast;
    if (updated)
        *updated = true;
    return true;
}

void hash_map_free(struct hash_map *hash_map)
{
    if (!hash_map)
        return;

    for (size_t i = 0; i < hash_map->size; i++)
    {
        struct pair_list *pl = hash_map->data[i];
        struct pair_list *next;
        while (pl)
        {
            next = pl->next;
            free(pl);
            pl = next;
        }
    }
    free(hash_map->data);
    free(hash_map);
}

/*
void hash_map_dump(struct hash_map *hash_map)
{
    for (size_t i = 0; i < hash_map->size; i++)
    {
        struct pair_list *pl = hash_map->data[i];
        if (!pl)
            continue;

        printf("%s: %s", pl->key, pl->ast);
        pl = pl->next;

        while (pl)
        {
            printf(", %s: %s", pl->key, pl->ast);
            pl = pl->next;
        }
        printf("\n");
    }
}
*/

const struct ast *hash_map_get(const struct hash_map *hash_map, const char *key)
{
    if (!hash_map)
        return NULL;
    if (hash_map->size == 0)
        return NULL;

    size_t index = hash(key) % hash_map->size;
    struct pair_list *pl = hash_map->data[index];

    while (pl && strcmp(pl->key, key) != 0)
        pl = pl->next;

    if (!pl)
        return NULL;

    return pl->ast;
}

bool hash_map_remove(struct hash_map *hash_map, const char *key)
{
    if (!hash_map || hash_map->size == 0)
        return false;

    size_t index = hash(key) % hash_map->size;
    struct pair_list *pl = hash_map->data[index];
    struct pair_list *prev = NULL;

    while (pl && strcmp(pl->key, key))
    {
        prev = pl;
        pl = pl->next;
    }

    if (!pl)
        return false;

    if (prev)
        prev->next = pl->next;
    else
        hash_map->data[index] = pl->next;

    free(pl);
    return true;
}
