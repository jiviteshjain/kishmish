CFLAGS = -g -Wall

kishmish: shell.o utils.o pwd.o cd.o
	gcc $(CFLAGS) shell.o utils.o pwd.o cd.o

clean:
	rm -f *.o a.out

# DONOT USE THESE RULES DIRECTLY
shell.o: shell.c shell.h
	gcc $(CFLAGS) -c shell.c

utils.o: utils.c utils.h
	gcc $(CFLAGS) -c utils.c

pwd.o: pwd.c pwd.h
	gcc $(CFLAGS) -c pwd.c

cd.o: cd.c cd.h
	gcc $(CFLAGS) -c cd.c