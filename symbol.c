#include <stdlib.h>
#include <string.h>

#include "hoc.h"
#include "y.tab.h"

static Symbol *symlist = 0;

Symbol *
lookup(char const *s)
{
	for (Symbol *sp = symlist; sp != NULL; sp = sp->next)
		if (strcmp(sp->name, s) == 0)
			return sp;
	return NULL;
}

static void *emalloc(size_t);

Symbol *
install(char const *s, int t, double d)
{
	Symbol *sp = emalloc(sizeof *sp);
	char *nbuf = emalloc(strlen(s)+1);
	strcpy(nbuf, s);
	*sp = (Symbol) {
		.name = nbuf,
		.type = t,
		.val  = d,
		.next = symlist,
	};
	symlist = sp;
	return sp;
}

void *
emalloc(size_t n)
{
	void *p = malloc(n);
	if (p == NULL)
		execerror("out of memory", NULL);
	return p;
}
