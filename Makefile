CC = g++
CFLAGS = --std=c++17 -Wall -Werror -pedantic -g
LIB = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -lboost_unit_test_framework
# Your .hpp files
DEPS = Sudoku.hpp
# Your compiled .o files
OBJECTS = Sudoku.o
# The name of your program
PROGRAM = Sudoku

.PHONY: all clean lint

all: $(PROGRAM) $(PROGRAM).a test

# Wildcard recipe to make .o files from corresponding .cpp file
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

$(PROGRAM).a: main.o $(OBJECTS)
	ar rcs Sudoku.a $(OBJECTS)

test: test.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

clean:
	rm *.o test $(PROGRAM) $(PROGRAM).a

lint:
	cpplint *.cpp *.hpp