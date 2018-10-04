# build
main.out: main.o token.o interpreter.o
	gcc -g main.o token.o interpreter.o -o main.out
	rm *.o

# main
main.o: main.c token.h interpreter.h
	gcc -c main.c

# token
token.o: token.c token.h
	gcc -c token.c

# interpreter
interpreter.o: interpreter.c interpreter.h
	gcc -c interpreter.c

# clean
clean:
	rm main.out
