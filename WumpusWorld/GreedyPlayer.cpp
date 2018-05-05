//
//  GreedyPlayer.cpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/24/16.
//

#include "GreedyPlayer.hpp"


// ================================================================
#pragma mark - GreedyPlayer Implementation
// ================================================================
WumpusWorld::Player::Configuration GreedyPlayer::nextMove()
{
    // Check if the chamber contains gold
    if (chamber()->contains(WumpusWorld::Chamber::Glitter)) {
        drop();
        grab();
        return _configuration;
    }

    // Assume we're moving north as default
    WumpusWorld::Chamber *nextChamber = chamber()->passage(WumpusWorld::Orientation::North);

    // Make a greedy choice and pick the one with the shortest eight-distance
    for (int heading = WumpusWorld::Orientation::North; heading <= WumpusWorld::Orientation::NorthWest; heading++) {
        WumpusWorld::Chamber *aChamber = chamber()->passage(static_cast<WumpusWorld::Orientation>(heading));
        
        if (!aChamber) continue;

        // This could potentially be randomized, when they have the same eight distance but I don't think it HAS TO be.
        if (!nextChamber || nextChamber->eightDistanceTo(_destination) > aChamber->eightDistanceTo(_destination)) {
            nextChamber = aChamber;
        }
    }
    
    enter(nextChamber);
    return _configuration;
}

bool GreedyPlayer::finished() const
{
    return _inventory.contains(WumpusWorld::Inventory::Item::Gold);
}

GreedyPlayer::GreedyPlayer(WumpusWorld::Player::Configuration const configuration, WumpusWorld::Chamber const *const destination):
WumpusWorld::Player(configuration),
_destination(destination)
{
}

GreedyPlayer::~GreedyPlayer()
{
    
}
