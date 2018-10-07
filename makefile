# build
main.out: main.o token.o lexer.o interpreter.o node.o parser.o
	gcc -g main.o token.o lexer.o interpreter.o node.o parser.o -o main.out
	rm *.o

# main
main.o: main.c interpreter.h
	gcc -c main.c

# token
token.o: token.c token.h
	gcc -c token.c

# lexer
lexer.o: lexer.c lexer.h token.h
	gcc -c lexer.c

# interpreter
interpreter.o: interpreter.c interpreter.h node.h parser.h token.h
	gcc -c interpreter.c

# node
node.o: node.c node.h token.h
	gcc -c node.c

# parser
parser.o: parser.c parser.h node.h lexer.h token.h
	gcc -c parser.c

# clean
clean:
	rm main.out
