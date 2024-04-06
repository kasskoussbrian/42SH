lib_LIBRARIES = libio_backend.a

libio_backend_a_SOURCES =  \
  io_backend.c \
  io_backend.h

libio_backend_a_CPPFLAGS = -I$(top_srcdir)/src -D_POSIX_C_SOURCE=200809L

libio_backend_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libio_backend.a
