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

typedef union Datum {
	double val;
	Symbol *sym;
} Datum;

typedef Datum (*Opr)(void);
typedef union Inst {
	Opr opr;
	double val;
	Symbol *sym;
	double (*func)(double);
} Inst;

extern Inst prog[];

#define STOP (Opr){0}

Datum pop(void), constpush(void), varpush(void), bltin(void), eval(void),
	add(void), sub(void), mul(void), divd(void),
	mod(void), negate(void), power(void),
	assign(void), print(void);

void initcode(void);

Inst *code(Inst);
#define oprcode(o) code((Inst){.opr = o})
#define valcode(v) code((Inst){.val = v})
#define symcode(s) code((Inst){.sym = s})
#define funcode(f) code((Inst){.func = f})

void execute(Inst *);
extern double lastval;

int yylex(void);
void yyerror(char const *);

extern char const *argv0;
#define nelem(a) (sizeof(a) / sizeof(a)[0])

void reset(void);
void execerror(char const *, char const *);
