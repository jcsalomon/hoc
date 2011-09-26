#include <stdio.h>

#include "hoc.h"

void
yyerror(char const *s)
{
	fprintf(stderr, "%s: %s\n", argv0, s);
}
