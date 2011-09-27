typedef struct Symbol {
	char const *name;
	int type;
	union {
		double val;
		double (*func)(double);
	};
	struct Symbol *next;
} Symbol;
Symbol *install(char const *, int, double);
Symbol *lookup(char const *);

void init(void);

int yylex(void);
void yyerror(char const *);

extern char const *argv0;

void reset(void);
void execerror(char const *, char const *);
