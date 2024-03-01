CXXFLAGS := --std=c++17 -ggdb
LDLIBS   := $$(pkg-config --cflags --libs readline) -lboost_system

%.yy.c: %.l
	flex -o $@ --header-file=$*.yy.h $<

%.yy.o: %.yy.c
	${COMPILE.c} $< -o $@

%.o: %.cpp
	${COMPILE.cpp} $< -o $@

main: lisp_balance.yy.o main.o
	${LINK.cpp} -o fu-shell $+ ${LDLIBS}

clean:
	rm *.o
