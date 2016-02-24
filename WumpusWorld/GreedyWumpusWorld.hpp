//
//  GreedyWumpusWorld.hpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/21/16.
//

#ifndef GreedyWumpusWorld_hpp
#define GreedyWumpusWorld_hpp

#include <iostream>
#include <vector>
#include "CLIWumpusWorld.hpp"

class GreedyWumpusWorld : public CLIWumpusWorld {
public:
    
    Chamber const *goldChamber;
    
    void takeTurn();
    bool playable() const;
    
    GreedyWumpusWorld(Configuration const configuration);
    
protected:
    std::vector<Coordinate> _path;
    
    void getPlayerOn(GreedyWumpusWorld::ChamberContent &chamberContent) const;
    std::string getHeadsUpDisplay() const;
    std::string getResults() const;
};

#endif /* GreedyWumpusWorld_hpp */
