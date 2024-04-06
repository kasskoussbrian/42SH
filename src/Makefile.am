SUBDIRS = \
ast \
       parser \
       lexer \
       io_backend \
       parse_args \
       utils \
       execution \
       expansion \
	   loop \
	   hash_map

bin_PROGRAMS = 42sh

42sh_SOURCES = main.c

42sh_CPPFLAGS = -I%D%

42sh_CC = gcc

42sh_CFLAGS = -std=c99 -Werror -Wall -Wextra -Wvla -pedantic

42sh_LDADD =  \
       parser/libparser.a  \
       ast/libast.a \
       lexer/liblexer.a \
       io_backend/libio_backend.a \
       parse_args/libparse_args.a \
	   loop/libloop.a \
       execution/libexecution.a \
       expansion/libexpansion.a \
       utils/libutils.a \
	   hash_map/libhash_map.a
