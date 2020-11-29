# Name of the project
PROJ_NAME=signAndVerify

# Flags for compiler
CC_FLAGS=-pedantic -std=c++11 -lcrypto -lgmp

#Compiler
CC=g++

# .cpp files
CXX_SOURCE=$(wildcard *.cpp)

# .h files
H_SOURCE=$(wildcard *.h)

# Object files
OBJ=base64.o isaac.o oaep.o rsa_.o str_utils.o

all: $(PROJ_NAME)

$(PROJ_NAME): main.o $(OBJ)
	$(CC) $(CC_FLAGS) -o $@ $^ 

test: base64.o str_utils.o rsa_.o oaep.o test.o
	$(CC) $(CC_FLAGS) -o $@ $^
	$(info Done building test, running test suite)
	./test

%.o: %.cpp %.h
	$(CC) -o $@ $< -c $(CC_FLAGS)

main.o: main.cpp $(H_SOURCE)
	$(CC) -o $@ $< -c $(CC_FLAGS)

clean:
	rm -rf *.o $(PROJ_NAME) test *~

test.o: teste.cpp
	$(CC) -o $@ $< -c $(CC_FLAGS)

isaac.o: isaac.cpp
	$(CC) -o $@ $< -c $(CC_FLAGS)
