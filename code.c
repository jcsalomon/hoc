#include <stdio.h>
#include <math.h>

#include "hoc.h"
#include "y.tab.h"

static Datum stack[256], *stackp;
Inst prog[2000], *progp, *pc;
double lastval;

void
initcode(void)
{
	stackp = stack;
	progp = prog;
}

static void
push(Datum d)
{
	if (stackp >= stack+nelem(stack))
		execerror("stack overflow", NULL);
	*stackp++ = d;
}

Datum
pop(void)
{
	if (stackp <= stack)
		execerror("stack underflow", NULL);
	return *--stackp;
}

Datum
constpush(void)
{
	push((Datum){.val = (*pc++).val});
	return (Datum){0};
}

Datum
varpush(void)
{
	push((Datum){.sym = (*pc++).sym});
	return (Datum){0};
}

Datum
bltin(void)
{
	Datum d = pop();
	d.val = (*pc++).func(d.val);
	push(d);
	return (Datum){0};
}

Datum
eval(void)
{
	Datum d = pop();
	if (d.sym->type == UNDEF)
		execerror("undefined variable", d.sym->name);
	d.val = d.sym->val;
	push(d);
	return (Datum){0};
}

Datum
add(void)
{
	Datum d2 = pop();
	Datum d1 = pop();
	d1.val += d2.val;
	push(d1);
	return (Datum){0};
}

Datum
sub(void)
{
	Datum d2 = pop();
	Datum d1 = pop();
	d1.val -= d2.val;
	push(d1);
	return (Datum){0};
}

Datum
mul(void)
{
	Datum d2 = pop();
	Datum d1 = pop();
	d1.val *= d2.val;
	push(d1);
	return (Datum){0};
}

Datum
divd(void)
{
	Datum d2 = pop();
	Datum d1 = pop();
	d1.val /= d2.val;
	push(d1);
	return (Datum){0};
}

Datum
mod(void)
{
	Datum d2 = pop();
	Datum d1 = pop();
	d1.val = fmod(d1.val, d2.val);
	push(d1);
	return (Datum){0};
}

Datum
negate(void)
{
	Datum d = pop();
	d.val = -d.val;
	push(d);
	return (Datum){0};
}

Datum
power(void)
{
	Datum d2 = pop();
	Datum d1 = pop();
	d1.val = pow(d1.val, d2.val);
	push(d1);
	return (Datum){0};
}

Datum
assign(void)
{
	Datum d1 = pop();
	Datum d2 = pop();
	if (d1.sym->type != VAR && d1.sym->type != UNDEF)
		execerror("assignment to non-variable", d1.sym->name);
	d1.sym->val = d2.val;
	d1.sym->type = VAR;
	push(d2);
	return (Datum){0};
}

Datum
print(void)
{
	Datum d = pop();
	printf("\t%.8g\n", d.val);
	return d;
}

Inst *
code(Inst f)
{
	Inst *oprogp = progp;
	if (progp >= prog+nelem(prog))
		execerror("program too big", NULL);
	*progp++ = f;
	return oprogp;
}

void
execute(Inst *p)
{
	Datum d;
	pc = p;
	while (pc->opr != STOP)
		d = (*pc++).opr();
	lastval = d.val;
}
