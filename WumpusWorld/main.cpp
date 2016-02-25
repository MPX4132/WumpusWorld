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
#include "GreedyPlayer.hpp"

int main(int argc, const char * argv[]) {
    std::fstream file((argc > 1)? argv[1] : "wumpus_1.txt");
    
    
    
    PTWumpusWorld world((WumpusWorld::Configuration(file)));
    
    GreedyPlayer player1(WumpusWorld::Player::Configuration("G"), world.goldChamber());
    //WumpusWorld::Player player2(WumpusWorld::Player::Configuration("A"));
    
    world.addPlayer(&player1);
    //world.addPlayer(&player2);
    world.run();
    
    std::cout << "Thank you for playing!" << std::endl;
    
    return 0;
}
