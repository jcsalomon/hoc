%{
#include <stdio.h>
#include <math.h>

#include "hoc.h"

double lastval = 0;
%}


%union {
	double val;
	Symbol *sym;
}
%token <val> NUMBER
%token <sym> VAR UNDEF
%type  <val> expr asgn

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
	| list asgn ';'  { lastval=$2; }
	| list expr ';'  { printf("\t%.8g\n", lastval=$2); }
	| list error ';' { yyerrok; }
	;
asgn
	: VAR  '=' expr { $$ = $1->val = $3; $1->type = VAR; }
	;
expr
	: NUMBER
	| '@'           { $$ = lastval; }
	| VAR           {
		if ($1->type == UNDEF)
			execerror("undefined variable", $1->name);
		$$ = $1->val;
	}
	| asgn
	| expr '+' expr { $$ = $1 + $3; }
	| expr '-' expr { $$ = $1 - $3; }
	| expr '*' expr { $$ = $1 * $3; }
	| expr '/' expr { $$ = $1 / $3; }
	| expr '%' expr { $$ = fmod($1, $3); }
	| expr '^' expr { $$ = pow($1, $3); }
	| '(' expr ')'  { $$ = $2; }
	| '+' expr %prec UNAROP { $$ = +$2; }
	| '-' expr %prec UNAROP { $$ = -$2; }
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
	setjmp(begin);

	if (yyparse() != 0)
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}

void
reset(void)
{
	longjmp(begin, 0);
}
