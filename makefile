# build
main.out: main.o token.o lexer.o interpreter.o node.o parser.o vec.o map.o bucket.o
	gcc -g main.o token.o lexer.o interpreter.o node.o parser.o vec.o map.o bucket.o -o main.out
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
interpreter.o: interpreter.c interpreter.h node.h parser.h token.h map.h
	gcc -c interpreter.c

# node
node.o: node.c node.h token.h
	gcc -c node.c

# parser
parser.o: parser.c parser.h node.h lexer.h token.h vec.h
	gcc -c parser.c

# vec
vec.o: vec.c vec.h
	gcc -c vec.c

# map
map.o: map.c map.h bucket.h
	gcc -c map.c

# bucket
bucket.o: bucket.c bucket.h
	gcc -c bucket.c

# clean
clean:
	rm main.out
