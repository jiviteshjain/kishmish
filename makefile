CFLAGS = -g -Wall

kishmish: shell.o utils.o prompt.o process.o parse.o jobs.o kjob.o overkill.o fg.o pwd.o cd.o ls.o echo.o pinfo.o env.o external.o history.o nightswatch.o exit.o
	gcc -o kishmish $(CFLAGS) shell.o utils.o prompt.o process.o parse.o jobs.o kjob.o overkill.o fg.o pwd.o cd.o ls.o echo.o pinfo.o env.o external.o history.o nightswatch.o exit.o

clean:
	rm -f *.o kishmish

# DONOT USE THESE RULES DIRECTLY
utils.o: utils.c utils.h
	gcc $(CFLAGS) -c utils.c

prompt.o: prompt.c prompt.h
	gcc $(CFLAGS) -c prompt.c

process.o: process.c process.h
	gcc $(CFLAGS) -c process.c

jobs.o: jobs.c jobs.h
	gcc $(CFLAGS) -c jobs.c

kjob.o: kjob.c kjob.h
	gcc $(CFLAGS) -c kjob.c

overkill.o: overkill.c overkill.h
	gcc $(CFLAGS) -c overkill.c

fg.o: fg.c fg.h
	gcc $(CFLAGS) -c fg.c

pwd.o: pwd.c pwd.h
	gcc $(CFLAGS) -c pwd.c

cd.o: cd.c cd.h
	gcc $(CFLAGS) -c cd.c

ls.o: ls.c ls.h
	gcc $(CFLAGS) -c ls.c

echo.o: echo.c echo.h
	gcc $(CFLAGS) -c echo.c

pinfo.o: pinfo.c pinfo.h
	gcc $(CFLAGS) -c pinfo.c

env.o: env.c env.h
	gcc $(CFLAGS) -c env.c

external.o: external.c external.h
	gcc $(CFLAGS) -c external.c

nightswatch.o: nightswatch.c nightswatch.h
	gcc $(CFLAGS) -c nightswatch.c

history.o: history.c history.h
	gcc $(CFLAGS) -c history.c

exit.o: exit.c exit.h
	gcc $(CFLAGS) -c exit.c

parse.o: parse.c parse.h
	gcc $(CFLAGS) -c parse.c

shell.o: shell.c shell.h
	gcc $(CFLAGS) -c shell.c