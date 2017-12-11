//
//  main.cpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 1/31/16.
//  Copyright © 2016 Matias Barcenas. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "PTWumpusWorld.hpp"
#include "ACOPlayer.hpp"

int main(int argc, const char * argv[]) {
    std::fstream file((argc > 1)? argv[1] : "wumpus_2.txt");

	// Load configuration file
	WumpusWorld::Configuration layout(file);

	// Get user defined start and end coordinates
//	int start[2] = {0, 0};
//	int end[2] = {7, 7};

	int start[2];
	int end[2];
	std::cout << "Enter starting coordinates separated by space: ";
	std::cin >> start[0] >> start[1];
	std::cout << "Enter ending coordinates separated by space: ";
	std::cin >> end[0] >> end[1];

	layout.entry = WumpusWorld::Coordinate::Reference(layout.size, WumpusWorld::Coordinate(start[0], start[1]));
	layout.gold = WumpusWorld::Coordinate::Reference(layout.size, WumpusWorld::Coordinate(end[0], end[1]));


	// Generate the world with the configuration
	PTWumpusWorld world(layout);

	// Create the player and run the world
	ACOPlayer player(WumpusWorld::Player::Configuration("ACO"), world.goldChamber());
	world.addPlayer(&player);

    world.run();
    
    std::cout << "Thank you for playing!" << std::endl;
    
    return 0;
}
