all: Program

compiler = g++
flags = -std=c++11 -g -Wall -Wno-unknown-pragmas

WumpusWorld.o: WumpusWorld.cpp WumpusWorld.hpp
	$(compiler) $(flags) -c WumpusWorld.cpp

CLIWumpusWorld.o: CLIWumpusWorld.cpp CLIWumpusWorld.hpp
	$(compiler) $(flags) -c CLIWumpusWorld.cpp

PTWumpusWorld.o: PTWumpusWorld.cpp PTWumpusWorld.hpp
	$(compiler) $(flags) -c PTWumpusWorld.cpp

GreedyPlayer.o: GreedyPlayer.cpp GreedyPlayer.hpp
	$(compiler) $(flags) -c GreedyPlayer.cpp

AStarPlayer.o: AStarPlayer.cpp AStarPlayer.hpp
	$(compiler) $(flags) -c AStarPlayer.cpp

ACOPlayer.o: ACOPlayer.cpp ACOPlayer.hpp
	$(compiler) $(flags) -c ACOPlayer.cpp

main.o: main.cpp
	$(compiler) $(flags) -c main.cpp

Program: WumpusWorld.o CLIWumpusWorld.o PTWumpusWorld.o AStarPlayer.o ACOPlayer.o main.o
	mkdir -p bin && cp Sample\ Configurations/* bin
	$(compiler) $(flags) WumpusWorld.o CLIWumpusWorld.o PTWumpusWorld.o AStarPlayer.o ACOPlayer.o main.o -o bin/WumpusWorld

clean:
	rm -rfv *o bin/*
