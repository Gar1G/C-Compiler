CPPFLAGS += -g

all: exec

bisontest.tab.cc : bisontest.y
	bison -d bisontest.y -o bisontest.tab.cc 

lex.yy.cc : flextest.l
	flex -o lex.yy.cc flextest.l

exec :	lex.yy.cc bisontest.tab.cc ast.hpp
	g++ $(CPPFLAGS) bisontest.tab.cc lex.yy.cc -o exec

