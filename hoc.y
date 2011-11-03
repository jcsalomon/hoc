%{
#include <stdio.h>

#include "hoc.h"
%}


%union {
	double val;
	Symbol *sym;
	Inst *inst;
}
%token <val> NUMBER
%token <sym> VAR BLTIN UNDEF

%right '='
%left  '%'
%left  '+' '-'
%left  '*' '/'
%left  UNAROP
%right '^'

/* expect one shift/reduce conflict:  shall a standalone assignment
 * (e.g., “x = 1;”) reduce to an expr and print its value,
 * or shift directly to “list asgn ';'” and not print?
 */
%expect 1

%%
list
	: // nothing
	| list ';'
	| list asgn ';'  { oprcode(pop); oprcode(STOP); return 1; }
	| list expr ';'  { oprcode(print); oprcode(STOP); return 1; }
	| list error ';' { yyerrok; }
	;
asgn
	: VAR  '=' expr { oprcode(varpush); symcode($1); oprcode(assign); }
	;
expr
	: NUMBER        { oprcode(constpush); valcode($1); }
	| '@'           { oprcode(constpush); valcode(lastval); }
	| VAR           { oprcode(varpush); symcode($1); oprcode(eval); }
	| asgn
	| expr '+' expr { oprcode(add); }
	| expr '-' expr { oprcode(sub); }
	| expr '*' expr { oprcode(mul); }
	| expr '/' expr { oprcode(divd); }
	| expr '%' expr { oprcode(mod); }
	| expr '^' expr { oprcode(power); }
	| '(' expr ')'
	| '+' expr %prec UNAROP
	| '-' expr %prec UNAROP { oprcode(negate); }
	| BLTIN '(' expr ')'    { oprcode(bltin); funcode($1->func); }
	;
%%

#include <stdlib.h>
#include <setjmp.h>

char const *argv0;
static jmp_buf begin;

int
main(int argc, char *argv[static argc+1])
{
	argv0 = argc > 0 ? argv[0] : "hoc";
	init();
	setjmp(begin);

	for (initcode(); yyparse(); initcode())
		execute(prog);

	exit(EXIT_SUCCESS);
}

void
reset(void)
{
	longjmp(begin, 0);
}
