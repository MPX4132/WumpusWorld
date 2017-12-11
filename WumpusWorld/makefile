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

AStarPlayer.o: AStarPlayer.cpp
	$(compiler) $(flags) -c AStarPlayer.cpp

ACOPlayer.o: ACOPlayer.cpp
	$(compiler) $(flags) -c ACOPlayer.cpp

main.o: main.cpp
	$(compiler) $(flags) -c main.cpp

Program: WumpusWorld.o CLIWumpusWorld.o PTWumpusWorld.o ACOPlayer.o main.o
	$(compiler) $(flags) WumpusWorld.o CLIWumpusWorld.o PTWumpusWorld.o ACOPlayer.o main.o -o WumpusWorld

clean:
	rm -rfv *o WumpusWorld
