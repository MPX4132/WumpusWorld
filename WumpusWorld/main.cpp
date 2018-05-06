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
#include "AStarPlayer.hpp"

int main(int argc, const char * argv[]) {
    std::fstream file((argc > 1)? argv[1] : "wumpus_3.txt");

    // Load configuration file from path.
    WumpusWorld::Configuration layout(file);

    // Check if the loaded configuration is valid. If invalid, abort.
    if (!layout.valid())
    {
        std::cout << "Aborting due to invalid configuration!" << std::endl;
        return 1;
    }

    // Prompt user for starting and ending coordinates (overwrite configuration entry & exit).
    //int start[2], end[2];
    //std::cout << "Enter starting coordinates (0-" << layout.size - 1 << ") separated by space: ";
    //std::cin >> start[0] >> start[1];

    //std::cout << "Enter ending coordinates (0-" << layout.size - 1 << ") separated by space: ";
    //std::cin >> end[0] >> end[1];

    // Set entry and gold locations to configuration.
    //layout.entry = WumpusWorld::Coordinate::Reference(layout.size, WumpusWorld::Coordinate(start[0], start[1]));
    //layout.gold = WumpusWorld::Coordinate::Reference(layout.size, WumpusWorld::Coordinate(end[0], end[1]));

    // Generate the world with loaded configuration.
    PTWumpusWorld world(layout);

    // Add second gold piece for the second player.
    world.goldChamber()->inventory.push(WumpusWorld::Inventory::Item::Gold);

    // Create and insert the player into the world.
    AStarPlayer player1(WumpusWorld::Player::Configuration("A-*"), world.goldChamber());
    ACOPlayer player2(WumpusWorld::Player::Configuration("ACO"), world.goldChamber());

    world.addPlayer(&player1);
    world.addPlayer(&player2);

    world.run();

    std::cout << "Agents finished playing." << std::endl;

    return 0;
}
