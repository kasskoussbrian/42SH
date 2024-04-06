#ifndef BUILTIN_H
#define BUILTIN_H

#include "expansion/expansion.h"
#include "hash_map/hash_map.h"

int my_echo(char *argv[]);
int my_exit(int argc, char *argv[], int *exit);
int my_unset(int argc, char *argv[], struct hash_map *hash_map,
             struct expanse *expansion);
int my_dot(int argc, char *argv[], struct expanse *expanse);
int my_export(int argc, char *argv[], struct expanse *expansion);
int my_cd(int argc, char *argv[]);
int my_continue(int argc, char *argv[], int *continue_val);
int my_break(int argc, char *argv[], int *break_val);

#endif /* ! BUILTIN_H */
