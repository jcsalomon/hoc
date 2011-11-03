#include <stddef.h>
#include <math.h>

#include "hoc.h"
#include "y.tab.h"

static const struct {
	char const *name;
	double val;
} consts[] = {
	{"PI",    3.14159265358979323846},
	{"E",     2.71828182845904523536},
	{"GAMMA", 0.57721566490153286060}, // Euler’s constant
	{"DEG",  57.29577951308232087680}, // degrees per radian
	{"PHI",   1.61803398874989484820}, // golden ratio
};

static const struct {
	char const *name;
	double (*func)(double);
} builtins[] = {
	{"sin",   sin},
	{"cos",   cos},
	{"atan",  atan},
	{"log",   log},
	{"log10", log10},
	{"exp",   exp},
	{"sqrt",  sqrt},
	{"abs",   fabs},
};

void
init(void)
{
	for (size_t c = 0; c < nelem(consts); c++)
		install(consts[c].name, VAR, consts[c].val);
	for (size_t b = 0; b < nelem(builtins); b++) {
		Symbol *s = install(builtins[b].name, BLTIN, 0);
		s->func = builtins[b].func;
	}
}
