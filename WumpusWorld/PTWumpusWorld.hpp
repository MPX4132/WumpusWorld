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
// extends CLIWumpusWorld by keeping a log of the agent's movment
// on the world and outputs the final path upon the simulation ending
// of each agent.
class PTWumpusWorld : public CLIWumpusWorld {
public:
    static const bool ShowStep = true;

    void addAgent(Agent * const agent);
    
    PTWumpusWorld(Configuration const configuration);
    ~PTWumpusWorld();

protected:
    std::vector<std::vector<Coordinates>> _coordinates;
    
    void getPlayersOn(ChamberContent &chamberContent) const;
    std::string getHeadsUpDisplay() const;
    void _processPlayer(std::vector<Agent*>::size_type const i);
    std::string getResults() const;
    
    bool _movedPlayer(std::vector<Agent*>::size_type const i) const;
};

#endif /* PTWumpusWorld_hpp */
