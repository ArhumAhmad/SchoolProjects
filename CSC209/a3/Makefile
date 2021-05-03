CC = gcc
CFLAGS = -Wall -g

mysh: mysh.o execute.o parsepath.o parse.o error.o
	gcc -g -o mysh mysh.o execute.o parsepath.o parse.o error.o

testparse: testparse.o parse.o error.o
	gcc -g -o testparse testparse.o parse.o error.o

clean:
	rm -f mysh mysh.o execute.o parsepath.o parse.o error.o testparse testparse.o

error.o: error.c error.h
execute.o: execute.c mysh.h parse.h execute.h error.h
mysh.o: mysh.c mysh.h parsepath.h parse.h execute.h error.h
parse.o: parse.c parse.h error.h
parsepath.o: parsepath.c 
testparse.o: testparse.c parse.h error.h