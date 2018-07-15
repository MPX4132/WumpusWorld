//
//  GreedyPlayer.hpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/24/16.
//

#ifndef GreedyPlayer_hpp
#define GreedyPlayer_hpp

#include <iostream>
#include <string>
#include <vector>
#include "WumpusWorld.hpp"


// The GreedyPlayer class extends the WumpusWorld::Player by taking
// the greedy approach to getting to a destination. It essentially
// overwrites a virtual method in the base class that's called
// when it's the player's time to make its next move.
class GreedyPlayer : public WumpusWorld::Agent {
public:
    
    Configuration nextMove();
    bool finished() const;
    
    GreedyPlayer(Configuration const configuration, WumpusWorld::Chamber const *const destination);
    ~GreedyPlayer();
    
protected:
    WumpusWorld::Chamber const *const _destination;
};

#endif /* GreedyPlayer_hpp */
