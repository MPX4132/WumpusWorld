all: Program

flags = -std=c++0x -g

WumpusWorld.o: WumpusWorld.cpp
	g++ $(flags) -c WumpusWorld.cpp

main.o: main.cpp
	g++ $(flags) -c main.cpp

Program: WumpusWorld.o main.o
	g++ $(flags) WumpusWorld.o main.o -o WumpusWorld

clean:
	rm -rfv *o WumpusWorld
