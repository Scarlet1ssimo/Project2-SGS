cc = gcc
prom = main
deps = SGS.h
src = main.c init.c effect.c basic.c print.c
obj = $(src:%.c=%.o)

$(prom): $(src) $(deps)
	$(cc) -o $(prom) $(src) -Wall -g -Wextra -pedantic -lncurses

clean:
	rm -rf main.o init.o effect.o basic.o print.o

run:
	./main;echo;read;
