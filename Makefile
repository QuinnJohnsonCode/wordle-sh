CXX=g++
CXXFLAGS=-std=c++23 -Wall -Werror -Wextra
RM=rm -f
LDFLAGS=-g $(root-config --ldflags)
LDLIBS=$(root-config --libs)

SRCS=main.cpp LetterColour.cpp Solver.cpp
OBJS=$(SRCS:.cpp=.o)
HEADERS=LetterColour.h Solver.h

.PHONY: all clean distclean

all: main

main: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) main
