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
void PTWumpusWorld::addPlayer(Player * const player)
{
    WumpusWorld::addPlayer(player);
    _coordinates.resize(_player.size());
    _coordinates[_player.size()-1].push_back(_player[_player.size()-1]->chamber()->coordinate());
}

void PTWumpusWorld::_processPlayer(std::vector<Player*>::size_type const i)
{
    _player[i]->nextMove();
    if (_movedPlayer(i))
        _coordinates[i].push_back(_player[i]->chamber()->coordinate());
}

void PTWumpusWorld::getPlayersOn(ChamberContent &chamberContent) const
{
	if (playable()) {
		CLIWumpusWorld::getPlayersOn(chamberContent);
		return;
	}

	for (std::vector<WumpusWorld::Player*>::size_type i = 0; i < _player.size(); i++) {
		int count = 0;
		for (Coordinate const &coordinate : _coordinates[i]) {
			std::stringstream &content = chamberContent.data[WumpusWorld::Coordinate::Reference(size(), coordinate)];
			content << _player[i]->identification();
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
    
    for (std::vector<WumpusWorld::Player*>::size_type i = 0; i < _player.size(); i++) {
        output << "Path for " << _player[i]->identification() << ": ";
        for (Coordinate const &coordinate : _coordinates[i]) {
            output << coordinate << " ";
        }
        output << std::endl;
    }
    
    return output.str();
}

bool PTWumpusWorld::_movedPlayer(std::vector<Player*>::size_type const i) const
{
    return _coordinates[i].back() != _player[i]->chamber()->coordinate();
}

PTWumpusWorld::PTWumpusWorld(Configuration const configuration):
CLIWumpusWorld(configuration)
{
    _coordinates.resize(_player.size());
    for (std::vector<WumpusWorld::Player*>::size_type i = 0; i < _player.size(); i++) {
        _coordinates[i].push_back(_player[i]->chamber()->coordinate());
    }
}

PTWumpusWorld::~PTWumpusWorld()
{
    
}
