#include <stdio.h>

#include <ctype.h>

#include "hoc.h"
#include "y.tab.h"

size_t lineno = 1;

int
yylex(void)
{
	int c;

	while ((c=getchar()) == ' ' || c == '\t')
		;
	if (c == EOF)
		return 0;
	if (c == '.' || isdigit(c)) {
		ungetc(c, stdin);
		scanf("%lf", &yylval);
		return NUMBER;
	}
	if (c == '\n')
		lineno++;
	return c;
}
