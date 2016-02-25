//
//  PTWumpusWorld.cpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/24/16.
//  Copyright © 2016 Matias Barcenas. All rights reserved.
//

#include "PTWumpusWorld.hpp"


void PTWumpusWorld::addPlayer(Player * const player)
{
    WumpusWorld::addPlayer(player);
    _coordinates.resize(_player.size());
    _coordinates[_player.size()-1].push_back(_player[_player.size()-1]->chamber()->coordinate());
}

void PTWumpusWorld::_processPlayer(int const i)
{
    _player[i]->nextMove();
    if (_movedPlayer(i))
        _coordinates[i].push_back(_player[i]->chamber()->coordinate());
}

void PTWumpusWorld::getPlayersOn(ChamberContent &chamberContent) const
{
    for (int i = 0; i < _player.size(); i++) {
        for (Coordinate const &coordinate : _coordinates[i]) {
            chamberContent.data[WumpusWorld::Coordinate::Reference(size(), coordinate)] << _player[i]->identification() << std::endl;
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
    
    for (int i = 0; i < _player.size(); i++) {
        output << "Path for " << _player[i]->identification() << ": ";
        for (Coordinate const &coordinate : _coordinates[i]) {
            output << coordinate << " ";
        }
        output << std::endl;
    }
    
    return output.str();
}

bool PTWumpusWorld::_movedPlayer(int const i) const
{
    return _coordinates[i].back() != _player[i]->chamber()->coordinate();
}

PTWumpusWorld::PTWumpusWorld(Configuration const configuration):
CLIWumpusWorld(configuration)
{
    _coordinates.resize(_player.size());
    for (int i = 0; i < _player.size(); i++) {
        _coordinates[i].push_back(_player[i]->chamber()->coordinate());
    }
}

PTWumpusWorld::~PTWumpusWorld()
{
    
}