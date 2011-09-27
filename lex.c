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
		scanf("%lf", &yylval.val);
		return NUMBER;
	}
	if (isalpha(c)) {
		char sbuf[100+1];

		size_t i = 0;
		do {
			sbuf[i++] = c;
		} while ((c=getchar()) != EOF && isalnum(c) && i<100);
		while (isalnum(c))
			c=getchar();
		ungetc(c, stdin);
		sbuf[i] = '\0';

		Symbol *s = lookup(sbuf);
		if (s == NULL)
			s = install(sbuf, UNDEF, 0.0);
		yylval.sym = s;
		return s->type == UNDEF ? VAR : s->type;
	}
	if (c == '\n') {
		lineno++;
		return ';';
	}
	return c;
}
