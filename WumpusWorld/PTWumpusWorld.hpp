//
//  PTWumpusWorld.hpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/24/16.
//

#ifndef PTWumpusWorld_hpp
#define PTWumpusWorld_hpp

#include <iostream>
#include <string>
#include <sstream>
#include "CLIWumpusWorld.hpp"


// The PTWumpusWorld class, or Player Tracking Wumpus World class
// extends CLIWumpusWorld by keeping a log of the player's movment
// on the world and outputs the final path upon the simulation ending
// of each player.
class PTWumpusWorld : public CLIWumpusWorld {
public:
	static const bool ShowStep = true;

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
