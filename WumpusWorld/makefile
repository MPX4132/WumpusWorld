all: Program

compiler = g++
flags = -std=c++0x -g

WumpusWorld.o: WumpusWorld.cpp
	$(compiler) $(flags) -c WumpusWorld.cpp

CLIWumpusWorld.o: CLIWumpusWorld.cpp
	$(compiler) $(flags) -c CLIWumpusWorld.cpp

PTWumpusWorld.o: PTWumpusWorld.cpp
	$(compiler) $(flags) -c PTWumpusWorld.cpp

GreedyPlayer.o: GreedyPlayer.cpp
	$(compiler) $(flags) -c GreedyPlayer.cpp

main.o: main.cpp
	$(compiler) $(flags) -c main.cpp

Program: WumpusWorld.o CLIWumpusWorld.o PTWumpusWorld.o GreedyPlayer.o main.o
	$(compiler) $(flags) WumpusWorld.o CLIWumpusWorld.o PTWumpusWorld.o GreedyPlayer.o main.o -o WumpusWorld

clean:
	rm -rfv *o WumpusWorld
