lib_LIBRARIES = libexpansion.a

libexpansion_a_SOURCES =  \
  expand_backslash.c \
  expand_parameter.c \
  expand_parameter_normal.c \
  expand_parameter_braces.c \
  expand_single_quote.c \
  expand_double_quote.c \
  expand_substitution.c \
  expand_substitution_backtick.c \
  expand_substitution_parenthesis.c \
  expand_field_split.c \
  expand.c \
  expansion.c \
  expansion.h \
  expand_normal.c \
  builtin_variable.c

libexpansion_a_CPPFLAGS = -I$(top_srcdir)/src

libexpansion_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic -D_POSIX_C_SOURCE=200809L

noinst_LIBRARIES = libexpansion.a
