CFLAGS = -g -Wall

kishmish: shell.o utils.o prompt.o parse.o pwd.o cd.o ls.o echo.o
	gcc $(CFLAGS) shell.o utils.o prompt.o parse.o pwd.o cd.o ls.o echo.o

clean:
	rm -f *.o a.out

# DONOT USE THESE RULES DIRECTLY
shell.o: shell.c shell.h
	gcc $(CFLAGS) -c shell.c

utils.o: utils.c utils.h
	gcc $(CFLAGS) -c utils.c

prompt.o: prompt.c prompt.h
	gcc $(CFLAGS) -c prompt.c

parse.o: parse.c parse.h
	gcc $(CFLAGS) -c parse.c

pwd.o: pwd.c pwd.h
	gcc $(CFLAGS) -c pwd.c

cd.o: cd.c cd.h
	gcc $(CFLAGS) -c cd.c

ls.o: ls.c ls.h
	gcc $(CFLAGS) -c ls.c

echo.o: echo.c echo.h
	gcc $(CFLAGS) -c echo.c