
object =  main.o command.o shell.o
target = out
$(target): $(object) 
	cc -o $(target) $(object) 

main.o: main.c
	cc -c main.c

command.o: command.c command.h
	cc -c command.c

shell.o: shell.c shell.h
	cc -c shell.c

clean:
	rm -rf *.o
	rm $(target) 

