//
//  GreedyPlayer.hpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/24/16.
//  Copyright © 2016 Matias Barcenas. All rights reserved.
//

#ifndef GreedyPlayer_hpp
#define GreedyPlayer_hpp

#include <iostream>
#include <string>
#include <vector>
#include "WumpusWorld.hpp"

class GreedyPlayer : public WumpusWorld::Player {
public:    
    int score() const;
    Configuration nextMove();
    bool finished() const;
    
    GreedyPlayer(Configuration const configuration, WumpusWorld::Chamber const *const destination);
    ~GreedyPlayer();
    
protected:
    WumpusWorld::Chamber const *const _destination;
};

#endif /* GreedyPlayer_hpp */
