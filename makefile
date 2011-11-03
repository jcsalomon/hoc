CFLAGS  = -Wall -Wextra -std=c1x
YFLAGS  = -Wall -d
LDLIBS  = -lm

OBJS    = lex.o symbol.o init.o code.o error.o
ALLOBJS = hoc.o $(OBJS)

hoc:     $(ALLOBJS)

hoc.o:   hoc.h

$(OBJS): hoc.h x.tab.h

x.tab.h: y.tab.h
	-cmp -s x.tab.h y.tab.h || cp y.tab.h x.tab.h

clean:
	rm -f $(ALLOBJS) [xy].tab.[ch] hoc
