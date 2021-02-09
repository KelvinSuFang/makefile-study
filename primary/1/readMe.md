

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
-I /home/hello/include表示将/home/hello/include目录作为第一个寻找头文件的目录，
寻找的顺序是：/home/hello/include-->/usr/include-->/usr/local/include
-L /home/hello/lib表示将/home/hello/lib目录作为第一个寻找库文件的目录，
寻找的顺序是：/home/hello/lib-->/lib-->/usr/lib-->/usr/local/lib
-lworld表示在上面的lib的路径中寻找libworld.so动态库文件（如果gcc编译选项中加入了“-static”表示寻找libworld.a静态库文件）

通配符wildcard 
objects := $(wildcard *.o)

VPATH = src:../headers

上面的的定义指定两个目录，“src”和“../headers”，


3:嵌套执行makefile

如果你要传递变量到下级 Makefile 中，那么你可以使用这样的声明：
export <variable ...>
如果你不想让某些变量传递到下级 Makefile 中，那么你可以这样声明：
unexport <variable ...>

我们总控的 Makefile 可以这样书写：
subsystem:
cd subdir && $(MAKE)
其等价于：
subsystem:
$(MAKE) -C subdir
例如：
lvgl:
cd lvgl && (MAKE)
或
lvgl:
(MAKE) -C lvgl


命令包

如果 Makefile 中出现一些相同命令序列，那么我们可以为这些相同的命令序列定义一 个变量。
定义这种命令序列的语法以“define”开始，以“endef”结束，如：
define run-yacc yacc $(firstword $^) mv y.tab.c $@
endef

foo.c : foo.y
$(run-yacc)


FOO ?= bar
其含义是，如果 FOO 没有被定义过，那么变量 FOO 的值就是“bar”，
如果 FOO 先前 被定义过，那么这条语将什么也不做，其等价于：
ifeq ($(origin FOO), undefined)
FOO = bar
endif


变量的高级用法
我们可以替换变量中的共有的部分，其格式是“$(var:a=b)”或是“${var:a=b}”，
其意 思是，把变量“var”中所有以“a”字串“结尾”的“a”替换成“b”字串。
这里的“结尾 ”意思是“空格”或是“结束符”。

foo := a.o b.o c.o
bar := $(foo:.o=.c)

这个示例中，我们先定义了一个“$(foo)”变量，
而第二行的意思是把“$(foo)”中所有 以“.o”字串“结尾”全部替换成“.c”，
所以我们的“$(bar)”的值就是“a.c b.c c.c”。


另外一种变量替换的技术是以“静态模式”（参见前面章节）定义的，如：
foo := a.o b.o c.o
bar := $(foo:%.o=%.c) 这依赖于被替换字串中的有相同的模式，
模式中必须包含一个“%”字符，这个例子同 样让$(bar)变量的值为“a.c b.c c.c”。

第二种高级用法是——“把变量的值再当成变量”。先看一个例子：
x = y y = z
a := $($(x))

在这个例子中，$(x)的值是“y”，所以$($(x))就是$(y)，
于是$(a)的值就是“z”。（注意 是“x=y”，而不是“x=$(y)”）

我们可以使用“+=”操作符给变量追加值，如：
objects = main.o foo.o bar.o utils.o
objects += another.o 于是，
我们的$(objects)值变成：“main.o foo.o bar.o utils.o another.o”
（another.o 被追加 进去了）

objects := $(objects) another.o
如果变量之前没有定义过，那么，“+=”会自动变成“=”，如果前面有变量定义，那么
“+=”会继承于前次操作的赋值符。如果前一次的是“:=”，那么“+=”会以“:=”作为其
赋值符，如：

五、override 指示符
保护makefile中定义的变量的值;
提供一种在makefile中增加或者修改命令行参数的方式;
实际情况下,我们经常会有这种需求:通过命令行指定一些附加的参数选项,对于一些通用的参数选项在makefile中指定.

#使用override进行追加的变量的原来指定的值不会被命令行参数覆盖,而且会追加命令行指定的值
override CFLAGS += -g

all:
	@echo $(CFLAGS)

make CFLAGS=-wall
make
-Wall -gcc

六、多行变量 还有一种设置变量值的方法是使用 define 关键字。使用 define 关键字设置变量的值可以 有换行，
这有利于定义一系列的命令（前面我们讲过“命令包”的技术就是利用这个 关键字）。
define 指示符后面跟的是变量的名字，而重起一行定义变量的值，定义是以 endef 关键 字结束。
其工作方式和“=”操作符一样。变量的值可以包含函数、命令、文字，或是其它 变量。因为命令需要以[Tab]键开头，
所以如果你用 define 定义的命令变量中没有以 [Tab]键开头，那么 make 就不会把其认为是命令。 
下面的这个示例展示了 define 的用法：
define two-lines
echo foo
echo $(bar)
endef

七、环境变量

八、目标变量
当然，我样同样可以为某个目标设置局部变量，这种变量被称为“Target-specific Variable”，
它可以和“全局变量”同名，因为它的作用范围只在这条规则以及连带规 则中，所以其值也只在作用范围内有效。
而不会影响规则链以外的全局变量的值。

prog : CFLAGS = -g
prog : prog.o foo.o bar.o
$(CC) $(CFLAGS) prog.o foo.o bar.o
prog.o : prog.c
$(CC) $(CFLAGS) prog.c
foo.o : foo.c
$(CC) $(CFLAGS) foo.c
bar.o : bar.c
$(CC) $(CFLAGS) bar.c 在这个示例中，不管全局的$(CFLAGS)的值是什么，在 prog 目标，
以及其所引发的所 有规则中（prog.o foo.o bar.o 的规则），$(CFLAGS)的值都是“-g”


九、模式变量
我们知道，变量可以定义在某个目标上。模式变量的好处就是，我们可以给定一 种“模式”，
可以把变量定义在符合这种模式的所有目标上。
我们知道，make 的“模式”一般是至少含有一个“%”的，
所以，我们可以以如下方 式给所有以[.o]结尾的目标定义目标变量： 
%.o : CFLAGS = -O

<pattern ...> : <variable-assignment> 
<pattern ...> : override <variable-assignment>

var := xxx
%e : var := exxx

rule :
    @echo "var => $(var)"

normal :
    @echo "var => $(var)"

十、条件判断
ifeq、else、endif

二、语法 条件表达式的语法为：

<conditional-directive>
<text-if-true>
endif 以及： <conditional-directive>
<text-if-true>
else <text-if-false>
endif


ifeq (<arg1>, <arg2> )
ifeq '<arg1>' '<arg2>' 
ifeq "<arg1>" "<arg2>" 
ifeq "<arg1>" '<arg2>' 
ifeq '<arg1>' "<arg2>"



ifneq (<arg1>, <arg2> )
ifneq '<arg1>' '<arg2>' 
ifneq "<arg1>" "<arg2>"
ifneq "<arg1>" '<arg2>'
ifneq '<arg1>' "<arg2>"


ifdef <variable-name>
如果变量<variable-name>的值非空，那到表达式为真。
否则，表达式为假。

特别注意的是，make 是在读取 Makefile 时就计算条件表达式的值，并根据条件表达式 的值来选择语句，
所以，你最好不要把自动化变量（如“$@”等）放入条件表达式中， 因为自动化变量是在运行时才有的。 
而且，为了避免混乱，make 不允许把整个条件语句分成两部分放在不同的文件中。


三、makefile函数
语法
$(<function> <arguments> )或
${<function> <arguments>}

1、字符串处理函数
$(subst <from>,<to>,<text> )
名称：字符串替换函数——subst。 功能：把字串<text>中的<from>字符串替换成<to>。 返回：函数返回被替换过后的字符串。 
示例：
$(subst ee,EE,feet on the street)， 把“feet on the street”中的“ee”替换成“EE”，返回结果是“fEEt on the strEEt”。


$(patsubst <pattern>,<replacement>,<text> )
通配符“%”，表示任意长度的字串。如果<replacement>中也包含“%”，
那么， <replacement>中的这个“%”将是<pattern>中的那个“%”所代表的字串。
功能：查找<text>中的单词（单词以“空格”、“Tab”或“回车”“换行”分隔）是否 符合模式<pattern>，
如果匹配的话，则以<replacement>替换。
（可以用“\” 来转义，以“\%”来表示真实含义的“%”字符）
$(patsubst %.c,%.o,x.c.c bar.c)
把字串“x.c.c bar.c”符合模式[%.c]的单词替换成[%.o]，
返回结果是“x.c.o bar.o”


$(strip <string> ) 名称：去空格函数——strip。 功能：去掉<string>字串中开头和结尾的空字符。
返回：返回被去掉空格的字符串值。 
示例：
$(strip a b c ) 把字串“a b c ”去到开头和结尾的空格，结果是“a b c”。


$(findstring <find>,<in> ) 名称：查找字符串函数——findstring。
功能：在字串<in>中查找<find>字串。 返回：如果找到，那么返回<find>，
否则返回空字符串。 
示例：
$(findstring a,a b c)
$(findstring a,b c) 第一个函数返回“a”字符串，第二个返回“”字符串（空字符串）



$(filter <pattern...>,<text> )
名称：过滤函数——filter。 功能：以<pattern>模式过滤<text>字符串中的单词，
保留符合模式<pattern>的单词。可 以有多个模式。
返回：返回符合模式<pattern>的字串。
示例：
sources := foo.c bar.c baz.s ugh.h
foo: $(sources) cc $(filter %.c %.s,$(sources)) -o foo
$(filter %.c %.s,$(sources))返回的值是“foo.c bar.c baz.s”。

名称：反过滤函数——filter-out。 功能：以<pattern>模式过滤<text>字符串中的单词，
去除符合模式<pattern>的单词。可 以有多个模式。 返回：返回不符合模式<pattern>的字串。 
示例：
objects=main1.o foo.o main2.o bar.o
mains=main1.o main2.o
$(filter-out $(mains),$(objects)) 返回值是“foo.o bar.o”。

$(sort <list> )
名称：排序函数——sort。 功能：给字符串<list>中的单词排序（升序）。 
返回：返回排序后的字符串。 
示例：$(sort foo bar lose)返回“bar foo lose” 。 
备注：sort 函数会去掉<list>中相同的单词。

$(word <n>,<text> )
名称：取单词函数——word。 功能：取字符串<text>中第<n>个单词。（从一开始） 
返回：返回字符串<text>中第<n>个单词。如果<n>比<text>中的单词数要大，那么返回 空字符串。 
示例：
$(word 2, foo bar baz)返回值是“bar”。

$(wordlist <s>,<e>,<text> )
名称：取单词串函数——wordlist。 功能：从字符串<text>中取从<s>开始到<e>的单词串。<s>和<e>是一个数字。
返回：返回字符串<text>中从<s>到<e>的单词字串。如果<s>比<text>中的单词数要大，
那么返回空字符串。如果<e>大于<text>的单词数，那么返回从<s>开始，到<text>结束 的单词串。
示例： $(wordlist 2, 3, foo bar baz)返回值是“bar baz”。

$(words <text> )
名称：单词个数统计函数——words。 功能：统计<text>中字符串中的单词个数。 
返回：返回<text>中的单词数。 示例：$(words, foo bar baz)返回值是“3”。 
备注：如果我们要取<text>中最后的一个单词，
我们可以这样：$(word $(words <text> ),<text> )。

$(firstword <text> )
名称：首单词函数——firstword。 
功能：取字符串<text>中的第一个单词。 
返回：返回字符串<text>的第一个单词。 
示例：$(firstword foo bar)返回值是“foo”。 
备注：这个函数可以用 word 函数来实现：$(word 1,<text> )。

	
以上，是所有的字符串操作函数，如果搭配混合使用，可以完成比较复杂的功能。
这里， 举一个现实中应用的例子。我们知道，make 使用“VPATH”变量来指定“依赖文件”的 搜索路径。
于是，我们可以利用这个搜索路径来指定编译器对头文件的搜索路径参数
CFLAGS，如：
override CFLAGS += $(patsubst %,-I%,$(subst :, ,$(VPATH)))
如果我们的“$(VPATH)”值是“src:../headers”，那么“$(patsubst %,-I%,$(subst :, ,
$(VPATH)))”将返回“-Isrc -I../headers”，这正是 cc 或 gcc 搜索头文件路径的参数。


三、文件名操作函数
$(dir <names...> )
名称：取目录函数——dir。 功能：从文件名序列<names>中取出目录部分。
目录部分是指最后一个反斜杠（“/”） 之前的部分。
如果没有反斜杠，那么返回“./”。 返回：返回文件名序列<names>的目录部分。 
示例： $(dir src/foo.c hacks)返回值是“src/ ./”。

$(notdir <names...> )
名称：取文件函数——notdir。 功能：从文件名序列<names>中取出非目录部分。
非目录部分是指最后一个反斜杠（“/ ”）之后的部分。 
返回：返回文件名序列<names>的非目录部分。 
示例： $(notdir src/foo.c hacks)返回值是“foo.c hacks”。

$(suffix <names...> ) 
名称：取后缀函数——suffix。 
功能：从文件名序列<names>中取出各个文件名的后缀。 
返回：返回文件名序列<names>的后缀序列，如果文件没有后缀，则返回空字串。 
示例：
$(suffix src/foo.c src-1.0/bar.c hacks)返回值是“.c .c”。

$(basename <names...> ) 名称：取前缀函数——basename。 
功能：从文件名序列<names>中取出各个文件名的前缀部分。 
返回：返回文件名序列<names>的前缀序列，如果文件没有前缀，则返回空字串。 
示例：$(basename src/foo.c src-1.0/bar.c hacks)返回值是“src/foo src-1.0/bar hacks”。

$(addsuffix <suffix>,<names...> )
名称：加后缀函数——addsuffix。 功能：把后缀<suffix>加到<names>中的每个单词后面。 
返回：返回加过后缀的文件名序列。 
示例：$(addsuffix .c,foo bar)返回值是“foo.c bar.c”。

$(addprefix <prefix>,<names...> ) 名称：加前缀函数——addprefix。 
功能：把前缀<prefix>加到<names>中的每个单词后面。 返回：返回加过前缀的文件名序列。 
示例：
$(addprefix src/,foo bar)返回值是“src/foo src/bar”。

$(join <list1>,<list2> ) 
名称：连接函数——join。 
功能：把<list2>中的单词对应地加到<list1>的单词后面。
如果<list1>的单词个数要比 <list2>的多，
那么，<list1>中的多出来的单词将保持原样。
如果<list2>的单词个数要比 <list1>多，
那么，<list2>多出来的单词将被复制到<list2>中。 
返回：返回连接过后的字符串。 
示例：$(join aaa bbb , 111 222 333)
返回值是“aaa111 bbb222 333”。


四、foreach 函数
$(foreach <var>,<list>,<text> )

names := a b c d
files := $(foreach n,$(names),$(n).o)
上面的例子中，$(name)中的单词会被挨个取出，并存到变量“n”中，“$(n).o”每次根 据“$(n)”计算出一个值，
这些值以空格分隔，最后作为 foreach 函数的返回，所以，
$(files)的值是“a.o b.o c.o d.o”。

注意，foreach 中的<var>参数是一个临时的局部变量，foreach 函数执行完后，参数
<var>的变量将不在作用，其作用域只在 foreach 函数当中。


五、if 函数
$(if <condition>,<then-part> )	
或是
$(if <condition>,<then-part>,<else-part> )


有一个 make 的环境变量叫“MAKECMDGOALS”，这个变量中会存放你所指定的终极 目标的列表，
如果在命令行上，你没有指定目标，
那么，这个变量是空值。这个变量可 以让你使用在一些比较特殊的情形下。比如下面的例子：
sources = foo.c bar.c ifneq ( $(MAKECMDGOALS),clean)
include $(sources:.c=.d)
endif 基于上面的这个例子，只要我们输入的命令不是“make clean”，那么 makefile 会自动 包含“foo.d”和“bar.d”这两个 makefile。

“clean” 这个伪目标功能是删除所有被 make 创建的文件。 “install” 这个伪目标功能是安装已编译好的程序，其实就是把目标执行文件拷贝到指定的目标 中去。
“print” 这个伪目标的功能是例出改变过的源文件。 “tar” 这个伪目标功能是把源程序打包备份。也就是一个 tar 文件。
“dist”
这个伪目标功能是创建一个压缩文件，一般是把 tar 文件压成 Z 文件。或是 gz 文件。 “TAGS” 这个伪目标功能是更新所有的目标，以备完整地重编译使用。 “check”和“test” 这两个伪目标一般用来测试 makefile 的流程



四、检查规则 有时候，我们不想让我们的 makefile 中的规则执行起来，我们只想检查一下我们的命 令，或是执行的序列。于是我们可以使用 make 命令的下述参数：
“-n” “--just-print” “--dry-run”
“--recon” 不执行参数，这些参数只是打印命令，不管目标是否更新，把规则和连带规则下的命 令打印出来，但不执行，这些参数对于我们调试 makefile 很有用处。
“-t” “--touch” 这个参数的意思就是把目标文件的时间更新，但不更改目标文件。也就是说，make 假 装编译目标，但不是真正的编译目标，只是把目标变成已编译过的状态。 “-q” “--question”


这个参数的行为是找目标的意思，也就是说，如果目标存在，那么其什么也不会输出， 当然也不会执行编译，如果目标不存在，其会打印出一条出错信息。 
“-W <file>” “--what-if=<file>” “--assume-new=<file>” “--new-file=<file>”
这个参数需要指定一个文件。一般是是源文件（或依赖文件），Make 会根据规则推导 来运行依赖于这个文件的命令，一般来说，可以和“-n”参数一同使用，来查看这个依 赖文件所发生的规则命令。


“-C <dir>” “--directory=<dir>” 指定读取 makefile 的目录。如果有多个“-C”参数，make 的解释是后面的路径以前面的
作为相对路径，并以最后的目录作为被指定目录。如：“make –C ~hchen/test –C prog”
等价于“make –C ~hchen/test/prog”。

“-I <dir>” “--include-dir=<dir>” 指定一个被包含 makefile 的搜索目标。可以使用多个“-I”参数来指定多个目录。



