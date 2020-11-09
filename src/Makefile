# Name of the project
PROJ_NAME=signAndVerify

# Flags for compiler
CC_FLAGS=-c -W -Wextra -Wall -ansi -pedantic

#Compiler
CC=g++

# .cpp files
CXX_SOURCE=$(wildcard *.cpp)

# .h files
H_SOURCE=$(wildcard *.h)

# Object files
OBJ=$(CXX_SOURCE:.cpp=.o)

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	$(CC) -o $@ $^

%.o: %.cpp %.h
	$(CC) -o $@ $< $(CC_FLAGS)

main.o: main.cpp $(H_SOURCE)
	$(CC) -o $@ $< $(CC_FLAGS)

clean:
	rm -rf *.o $(PROJ_NAME) *~
