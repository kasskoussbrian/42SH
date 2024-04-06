lib_LIBRARIES = libexecution.a

libexecution_a_SOURCES =  \
  execution.c \
  execution.h \
  exec_list.c \
  exec_simple_cmd.c \
  exec_ass_word.c \
  exec_shell_cmd.c \
  exec_redir.c \
  exec_pipeline.c \
  exec_while.c \
  exec_until.c \
  exec_if.c \
  exec_not.c \
  exec_operator.c \
  exec_for.c \
  exec_func.c \
  exec_case.c \
  builtin/echo.c \
  builtin/exit.c \
  builtin/unset.c \
  builtin/dot.c \
  builtin/export.c \
  builtin/cd.c \
  builtin/continue.c \
  builtin/break.c \
  builtin/builtin.h

libexecution_a_CPPFLAGS = -I$(top_srcdir)/src -D_POSIX_C_SOURCE=200809L

libexecution_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libexecution.a
