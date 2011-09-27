CFLAGS  = -Wall -Wextra -std=c1x
YFLAGS  = -Wall -d
LDLIBS  = -lm

OBJS    = lex.o symbol.o init.o error.o
ALLOBJS = hoc.o $(OBJS)

hoc:     $(ALLOBJS)

hoc.o:   hoc.h

$(OBJS): hoc.h y.tab.h

clean:
	rm -f $(ALLOBJS) y.tab.[ch] hoc
