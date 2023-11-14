CFLAGS = -W -Wall -g
PROGRAMS = suniq

all: $(PROGRAMS)

suniq: suniq.o
	gcc -o suniq $^

suniq.o: suniq.c

clean:
	rm *.o
