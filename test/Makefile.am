ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
AUTOMAKE_OPTIONS = subdir-objects

LDADD = \
    ../src/libsqlparser.la

check_SCRIPTS = \
    visualize.sh

check_PROGRAMS = \
    tst-sql-parser

tst_sql_parser_SOURCES = \
    tst-sql-parser.cpp

TESTS = $(check_PROGRAMS) $(check_SCRIPTS)

EXTRA_DIST = \
    $(check_SCRIPTS)
