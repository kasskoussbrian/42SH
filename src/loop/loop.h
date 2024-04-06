#ifndef LOOP_H
#define LOOP_H

#include "execution/execution.h"
#include "expansion/expansion.h"
#include "hash_map/hash_map.h"
#include "parse_args/parse_args.h"
#include "parser/parser.h"

int main_loop(struct exec_args exec_args, struct parser *parser);
void save_last_status_code(struct exec_args *exec_args, int exit_status);

#endif /* ! LOOP_H */
