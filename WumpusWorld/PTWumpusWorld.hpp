//
//  PTWumpusWorld.hpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/24/16.
//  Copyright © 2016 Matias Barcenas. All rights reserved.
//

#ifndef PTWumpusWorld_hpp
#define PTWumpusWorld_hpp

#include <iostream>
#include <string>
#include <sstream>
#include "CLIWumpusWorld.hpp"

class PTWumpusWorld : public CLIWumpusWorld {
public:
    
    void addPlayer(Player * const player);
    
    PTWumpusWorld(Configuration const configuration);
    ~PTWumpusWorld();

protected:
    std::vector<std::vector<Coordinate>> _coordinates;
    
    void getPlayersOn(ChamberContent &chamberContent) const;
    std::string getHeadsUpDisplay() const;
    void _processPlayer(int const i);
    std::string getResults() const;
    
    bool _movedPlayer(int const i) const;
};

#endif /* PTWumpusWorld_hpp */
