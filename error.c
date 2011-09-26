#include <stdio.h>

#include "hoc.h"

extern size_t lineno;

static void warning(char const *, char const *);

void
yyerror(char const *s)
{
	warning(s, NULL);
}

void
execerror(char const *s, char const *t)
{
	warning(s, t);
	reset();
}

void
warning(char const *s, char const *t)
{
	fprintf(stderr, "%s: %s", argv0, s);
	if (t && *t)
		fprintf(stderr, " %s", t);
	fprintf(stderr, " near line %zu\n", lineno);
}
