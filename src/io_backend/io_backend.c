#include "io_backend/io_backend.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct io_backend *io_backend_init(struct cli_args args)
{
    switch (args.io_mode)
    {
    case IO_BACKEND_STRING:
        return io_backend_init_string(args.script);
    case IO_BACKEND_FILE:
        return io_backend_init_file(args.filename);
    case IO_BACKEND_STDIN:
        return io_backend_init_stdin();
    }
    warnx("io_backend_init(): unknown io mode");
    return NULL;
}

struct io_backend *io_backend_init_string(char *string)
{
    assert(string);

    struct io_backend *io_backend = calloc(1, sizeof(*io_backend));
    if (!io_backend)
    {
        fprintf(stderr, "Memory exhausted\n");
        return NULL;
    }

    io_backend->mode = IO_BACKEND_STRING;
    io_backend->stream = fmemopen(string, strlen(string), "r");
    io_backend->last_char = NO_CHARACTER;

    if (io_backend->stream == NULL)
    {
        warn("fmemopen()");
        io_backend_cleanup(io_backend);
        return NULL;
    }
    return io_backend;
}

struct io_backend *io_backend_init_file(char *filename)
{
    struct io_backend *io_backend = calloc(1, sizeof(*io_backend));
    if (!io_backend)
    {
        fprintf(stderr, "Memory exhausted\n");
        return NULL;
    }

    io_backend->mode = IO_BACKEND_FILE;
    io_backend->stream = fopen(filename, "r");
    io_backend->last_char = NO_CHARACTER;
    if (io_backend->stream == NULL)
    {
        warn("fmemopen()");
        io_backend_cleanup(io_backend);
        return NULL;
    }
    return io_backend;
}

struct io_backend *io_backend_init_stdin(void)
{
    struct io_backend *io_backend = calloc(1, sizeof(*io_backend));
    if (!io_backend)
    {
        fprintf(stderr, "Memory exhausted\n");
        return NULL;
    }

    io_backend->mode = IO_BACKEND_STDIN;
    io_backend->last_char = NO_CHARACTER;
    io_backend->stream = stdin;
    return io_backend;
}

void io_backend_cleanup(struct io_backend *io_backend)
{
    if (!io_backend)
        return;

    switch (io_backend->mode)
    {
    case IO_BACKEND_STRING:
    case IO_BACKEND_FILE:
        if (io_backend->stream != NULL)
            fclose(io_backend->stream);
        break;
    /* We shouldn't close stdin */
    case IO_BACKEND_STDIN:
        break;
    }
    free(io_backend);
}

char io_backend_readchar(struct io_backend *io_backend)
{
    assert(io_backend);
    assert(io_backend->stream);

    io_backend->last_char = NO_CHARACTER;
    return io_backend_peek(io_backend);
}

char io_backend_peek(struct io_backend *io_backend)
{
    assert(io_backend);
    assert(io_backend->stream);

    if (io_backend->last_char == NO_CHARACTER)
    {
        io_backend->last_char = fgetc(io_backend->stream);
        return io_backend->last_char;
    }

    return io_backend->last_char;
}
