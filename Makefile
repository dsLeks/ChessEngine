all: Board.o Hashkeys.o BitBoard.o init.o data.o attack.o io.o movegen.o validate.o
	g++ main.cpp Board.o Hashkeys.o BitBoard.o init.o data.o attack.o io.o movegen.o validate.o -o main
Board.o:
	g++ -c Board.cpp -o Board.o
Hashkeys.o:
	g++ -c Hashkeys.cpp -o Hashkeys.o
BitBoard.o: 
	g++ -c BitBoard.cpp -o BitBoard.o
init.o: 
	g++ -c init.cpp -o init.o
data.o:
	g++ -c data.cpp -o data.o
attack.o:
	g++ -c attack.cpp -o attack.o
io.o:
	g++ -c io.cpp -o io.o
movegen.o:
	g++ -c movegen.cpp -o movegen.o
validate.o:
	g++ -c validate.cpp -o validate.o
clean:
	rm -rf *.o main
