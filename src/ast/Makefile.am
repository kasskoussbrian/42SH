lib_LIBRARIES = libast.a

libast_a_SOURCES =  \
  ast.c \
  ast_if.c \
  ast_list.c \
  ast_simple_command.c \
  ast_shell_command.c \
  ast_redir.c \
  ast_pipeline.c \
  ast_not.c \
  ast_while.c \
  ast_until.c \
  ast_for.c \
  ast_operator.c \
  ast_ass_word.c \
  ast_func.c \
  ast_case.c \
  ast_item_case.c \
  ast_subshell.c \
  ast_to_dot.h \
  ast.h \
  list.c \
  list.h

libast_a_CPPFLAGS = -I$(top_srcdir)/src -D_POSIX_C_SOURCE=200809L

libast_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libast.a
