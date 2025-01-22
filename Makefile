CXX=g++
CXXFLAGS=-std=c++23 -Wall -Werror -Wextra
RM=rm -f
LDFLAGS=-g $(root-config --ldflags)
LDLIBS=$(root-config --libs)

SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)

.PHONY: all clean distclean

all: main

main: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) main