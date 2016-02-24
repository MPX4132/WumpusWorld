//
//  main.cpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 1/31/16.
//  Copyright © 2016 Matias Barcenas. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "GreedyWumpusWorld.hpp"

int main(int argc, const char * argv[]) {
    std::fstream file((argc > 1)? argv[1] : "wumpus_2.txt");
    
    WumpusWorld::Player player1;
    
    GreedyWumpusWorld world((WumpusWorld::Configuration(file)));
    world.run();
    
    std::cout << "Thank you for playing!" << std::endl;
    
    return 0;
}
