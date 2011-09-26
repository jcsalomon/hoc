int yylex(void);
void yyerror(char const *);

#define YYSTYPE double

extern char const *argv0;

void reset(void);
void execerror(char const *, char const *);
