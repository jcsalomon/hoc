%{
#include <stdio.h>
#include <math.h>

#include "hoc.h"
%}


%token NUMBER

%left  '%'
%left  '+' '-'
%left  '*' '/'
%left  UNAROP
%right '^'


%%
list
	: // nothing
	| list '\n'
	| list expr '\n'  { printf("\t%.8g\n", $2); }
	| list error '\n' { yyerrok; }
	;
expr
	: NUMBER        { $$ = $1; }
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
