#include <string.h>

#include "builtin/builtin.h"
#include "expansion/expand.h"
#include "expansion/expansion.h"

/* On unset d'abord, les variables
si on ne trouve pas on unset les fonctions
*/
int my_unset(int argc, char *argv[], struct hash_map *hash_map,
             struct expanse *expansion)
{
    struct expanse *prev = NULL;
    struct expanse *tmp = NULL;
    bool find = false;

    for (int i = 1; i < argc; i++)
    {
        for (struct expanse *cur = expansion; cur; cur = cur->next)
        {
            if (strcmp(cur->variable, argv[i]) == 0)
            {
                if (!prev) // delete the head
                    expansion = remove_expanse(cur, prev);
                else
                {
                    tmp = cur->next;
                    remove_expanse(cur, prev);
                    cur = tmp;
                }
                find = true;
                break;
            }
            prev = cur;
        }
        if (!find) // On check les fonctions
        {
            hash_map_remove(hash_map, argv[i]);
        }
    }

    return EXIT_SUCCESS;
}
