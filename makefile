CPPFLAGS += -g

all: exec

bisontest.tab.cc : bisontest.y
	bison -d bisontest.y -o bisontest.tab.cc

lex.yy.cc : compiler.l
	flex -o compiler.yy.cc compiler.l

exec :	compiler.yy.cc bisontest.tab.cc ast.hpp
	g++ $(CPPFLAGS) bisontest.tab.cc compiler.yy.cc -o exec
