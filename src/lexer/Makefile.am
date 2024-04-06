lib_LIBRARIES = liblexer.a

liblexer_a_SOURCES =  \
  lexer.c \
  lexer.h \
  lexer_utils.c \
  lexer_matching_type.c \
  lexer_backslash.c \
  lexer_command_sub.c \
  lexer_dquote.c \
  lexer_param_sub.c \
  lexer_quoting.c \
  lexer_rules.c


liblexer_a_CPPFLAGS = -I$(top_srcdir)/src

liblexer_a_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic -D_POSIX_C_SOURCE=200909L

noinst_LIBRARIES = liblexer.a
