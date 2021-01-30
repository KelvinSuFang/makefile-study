
1: 基本规则
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
	
2：makefile包含文件
include <filename>
include foo.make *.mk $(bar)  
等价于:
include foo.make a.mk b.mk c.mk e.mk f.mk


-I 或者--include-dir参数， make就会在这个参数指定的目录下去寻找
-include <filename>

我们用gcc编译程序时，可能会用到“-I”（大写i），“-L”（大写l），“-l”（小写l）等参数，下面做个记录：
例：
gcc -o hello hello.c -I /home/hello/include -L /home/hello/lib -lworld
上面这句表示在编译hello.c时：
-I /home/hello/include表示将/home/hello/include目录作为第一个寻找头文件的目录，寻找的顺序是：/home/hello/include-->/usr/include-->/usr/local/include
-L /home/hello/lib表示将/home/hello/lib目录作为第一个寻找库文件的目录，寻找的顺序是：/home/hello/lib-->/lib-->/usr/lib-->/usr/local/lib
-lworld表示在上面的lib的路径中寻找libworld.so动态库文件（如果gcc编译选项中加入了“-static”表示寻找libworld.a静态库文件）

通配符wildcard 
objects := $(wildcard *.o)

VPATH = src:../headers

上面的的定义指定两个目录，“src”和“../headers”，





















