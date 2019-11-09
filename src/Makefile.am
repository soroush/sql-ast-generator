ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = naive-sql-parser

AM_CPPFLAGS = \
    -I$srcdir \
    --pedantic -Wall -Werror \
    -Wpointer-arith -Wformat -Wreturn-type \
    -Wsign-compare -Wmultichar -Wformat-nonliteral -Winit-self \
    -Wuninitialized -Wformat-security \
    -fPIC -fstack-protector

naive_sql_parser_SOURCES = \
    naive-lexer.hpp \
    naive-lexer.cpp \
    state-machine.hpp \
    state-machine.cpp \
    tree.hpp \
    tree.cpp \
    main.cpp
