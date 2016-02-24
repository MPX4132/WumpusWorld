//
//  GreedyWumpusWorld.cpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/21/16.
//

#include "GreedyWumpusWorld.hpp"

bool GreedyWumpusWorld::playable() const
{
    return !player.inventory().contains(WumpusWorld::Item::Gold);
}

void GreedyWumpusWorld::takeTurn()
{
    // Check if the chamber contains gold
    if (player.chamber()->contains(WumpusWorld::Chamber::Glitter)) {
        player.drop();
        player.grab();
        return;
    }
    
    WumpusWorld::Chamber *nextChamber = passage(player.chamber(), WumpusWorld::Orientation::North);
    
    for (int heading = WumpusWorld::Orientation::North; heading <= WumpusWorld::Orientation::NorthWest; heading++) {
        WumpusWorld::Chamber *aChamber = passage(player.chamber(), static_cast<WumpusWorld::Orientation>(heading));
        
        if (!aChamber) continue;
        
        if (nextChamber->distanceTo(goldChamber) > aChamber->distanceTo(goldChamber)) {
            nextChamber = aChamber;
        }
    }
    
    player.enter(nextChamber);
    _path.push_back(WumpusWorld::Coordinate::Dereference(size(), player.chamber()->location()));
}

void GreedyWumpusWorld::getPlayerOn(GreedyWumpusWorld::ChamberContent &chamberContent) const
{
    for (Coordinate const &coordinate : _path) {
        chamberContent.data[WumpusWorld::Coordinate::Reference(size(), coordinate)] << "[X]";//player;
    }
}

std::string GreedyWumpusWorld::getHeadsUpDisplay() const
{
    std::stringstream output;
    output << " Percepts: " << WumpusWorld::Player::Sensory(player.sense()) << std::endl;
    output << "Inventory: " << player.inventory() << std::endl;
    return output.str();
}

std::string GreedyWumpusWorld::getResults() const
{
    std::stringstream output;
    
    output << "Path: ";
    for (Coordinate const &coordinate : _path) {
        output << coordinate << ' ';
    }
    output << std::endl;
    
    // output << "Total Distance: " << (_path.size()-1) * 5 << " (" << _path.size()-1 << " moves)" << std::endl;
    return output.str();
}

GreedyWumpusWorld::GreedyWumpusWorld(WumpusWorld::Configuration const configuration):
CLIWumpusWorld(configuration),
goldChamber(&chamber(configuration.gold))
{
    _path.push_back(WumpusWorld::Coordinate::Dereference(size(), player.chamber()->location()));
}