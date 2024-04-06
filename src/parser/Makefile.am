lib_LIBRARIES = libparser.a

libparser_a_SOURCES =  \
  parser.c \
  parse_and_or.c \
  parse_cmd.c \
  parse_simple_cmd.c \
  parse_element.c \
  parse_list.c \
  parse_pipe.c \
  parse_redir.c \
  parse_prefix.c \
  parse_rule_if.c\
  parse_while.c \
  parse_until.c \
  parse_for.c \
  parse_shell_cmd.c\
  parse_else_clause.c\
  parse_compound_list.c\
  parse_func.c\
  parse_case.c\
  parser.h

libparser_a_CPPFLAGS = -I$(top_srcdir)/src

libparser_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

noinst_LIBRARIES = libparser.a
