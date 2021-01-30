
object =  main.o command.o shell.o
out: $(object) 
	cc -o out main.o command.o shell.o

main.o: main.c
	cc -c main.c

command.o: command.c command.h
	cc -c command.c

shell.o: shell.c shell.h
	cc -c shell.c

clean:
	rm -rf *.o
	rm a.out
	rm out

