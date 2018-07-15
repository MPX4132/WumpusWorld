//
//  PTWumpusWorld.cpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/24/16.
//

#include "PTWumpusWorld.hpp"


// ================================================================
#pragma mark - PTWumpusWorld Implementation
// ================================================================
void PTWumpusWorld::addAgent(Agent * const agent)
{
    WumpusWorld::addAgent(agent);
    _coordinates.resize(_agent.size());
    _coordinates[_agent.size()-1].push_back(_agent[_agent.size()-1]->chamber()->coordinates());
}

void PTWumpusWorld::_processPlayer(std::vector<Agent*>::size_type const i)
{
    _agent[i]->nextMove();
    if (_movedPlayer(i))
        _coordinates[i].push_back(_agent[i]->chamber()->coordinates());
}

void PTWumpusWorld::getPlayersOn(ChamberContent &chamberContent) const
{
    if (playable()) {
        CLIWumpusWorld::getPlayersOn(chamberContent);
        return;
    }

    for (std::vector<WumpusWorld::Agent*>::size_type i = 0; i < _agent.size(); i++) {
        int count = 0;
        for (Coordinates const &coordinate : _coordinates[i]) {
            std::stringstream &content = chamberContent.data[WumpusWorld::Coordinates::Reference(size(), coordinate)];
            content << _agent[i]->identification();
            if (PTWumpusWorld::ShowStep) content << "-" << count++;
            content << std::endl;
        }
    }

}

std::string PTWumpusWorld::getHeadsUpDisplay() const
{
    std::stringstream output;
    output << getHeadsUpDisplayPercepts();
    output << getHeadsUpDisplayInventories();
    return output.str();
}

std::string PTWumpusWorld::getResults() const
{
    std::stringstream output;
    
    for (std::vector<WumpusWorld::Agent*>::size_type i = 0; i < _agent.size(); i++) {
        output << "Path for " << _agent[i]->identification() << ": ";
        for (Coordinates const &coordinate : _coordinates[i]) {
            output << coordinate << " ";
        }
        output << std::endl;
    }
    
    return output.str();
}

bool PTWumpusWorld::_movedPlayer(std::vector<Agent*>::size_type const i) const
{
    return _coordinates[i].back() != _agent[i]->chamber()->coordinates();
}

PTWumpusWorld::PTWumpusWorld(Configuration const configuration):
CLIWumpusWorld(configuration)
{
    _coordinates.resize(_agent.size());
    for (std::vector<WumpusWorld::Agent*>::size_type i = 0; i < _agent.size(); i++) {
        _coordinates[i].push_back(_agent[i]->chamber()->coordinates());
    }
}

PTWumpusWorld::~PTWumpusWorld()
{
    
}
