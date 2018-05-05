all: Program

compiler = g++
flags = -std=c++11 -g -Wall -Wno-unknown-pragmas

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

Program: WumpusWorld.o CLIWumpusWorld.o PTWumpusWorld.o AStarPlayer.o ACOPlayer.o main.o
	mkdir -p bin && cp Sample\ Configurations/* bin
	$(compiler) $(flags) WumpusWorld.o CLIWumpusWorld.o PTWumpusWorld.o AStarPlayer.o ACOPlayer.o main.o -o bin/WumpusWorld

clean:
	rm -rfv *o bin/*
