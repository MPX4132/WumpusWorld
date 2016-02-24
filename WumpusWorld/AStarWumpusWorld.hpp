//
//  AStarWumpusWorld.hpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/23/16.
//  Copyright © 2016 Matias Barcenas. All rights reserved.
//

#ifndef AStarWumpusWorld_hpp
#define AStarWumpusWorld_hpp

#include <iostream>
#include "CLIWumpusWorld.hpp"

class AStarWumpusWorld : public CLIWumpusWorld {
public:
    AStarWumpusWorld(Configuration const configuration);
};

#endif /* AStarWumpusWorld_hpp */
