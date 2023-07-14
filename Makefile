all: Board.o BitBoard.o init.o
	g++ main.cpp Board.o BitBoard.o init.o -o main
Board.o:
	g++ -c Board.cpp -o Board.o
BitBoard.o: 
	g++ -c BitBoard.cpp -o BitBoard.o
init.o: 
	g++ -c init.cpp -o init.o
clean:
	rm -rf *.o main
