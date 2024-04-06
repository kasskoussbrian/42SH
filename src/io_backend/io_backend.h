#ifndef IO_BACKEND_H
#define IO_BACKEND_H

#include <stdio.h>

#include "parse_args/parse_args.h"

#define NO_CHARACTER -2

struct io_backend
{
    char last_char;
    enum io_backend_mode mode;
    FILE *stream;
};

struct io_backend *io_backend_init(struct cli_args args);
struct io_backend *io_backend_init_string(char *string);
struct io_backend *io_backend_init_file(char *filename);
struct io_backend *io_backend_init_stdin(void);
void io_backend_cleanup(struct io_backend *io_backend);
char io_backend_readchar(struct io_backend *io_backend);
char io_backend_peek(struct io_backend *io_backend);

#endif /* ( !IO_BACKEND_H ) */
