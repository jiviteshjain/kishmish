CFLAGS = -g

kishmish: shell.o utils.o prompt.o parse.o pwd.o cd.o ls.o echo.o pinfo.o external.o history.o nightswatch.o exit.o
	gcc -o kishmish $(CFLAGS) shell.o utils.o prompt.o parse.o pwd.o cd.o ls.o echo.o pinfo.o external.o history.o nightswatch.o exit.o

clean:
	rm -f *.o kishmish

# DONOT USE THESE RULES DIRECTLY
utils.o: utils.c utils.h
	gcc $(CFLAGS) -c utils.c

prompt.o: prompt.c prompt.h
	gcc $(CFLAGS) -c prompt.c

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