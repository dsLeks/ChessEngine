all: Board.o Hashkeys.o BitBoard.o init.o data.o
	g++ main.cpp Board.o Hashkeys.o BitBoard.o init.o data.o -o main
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
clean:
	rm -rf *.o main
