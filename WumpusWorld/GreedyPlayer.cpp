//
//  GreedyPlayer.cpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/24/16.
//  Copyright © 2016 Matias Barcenas. All rights reserved.
//

#include "GreedyPlayer.hpp"


int GreedyPlayer::score() const
{
    return 0;
}

WumpusWorld::Player::Configuration GreedyPlayer::nextMove()
{
    // Check if the chamber contains gold
    if (chamber()->contains(WumpusWorld::Chamber::Glitter)) {
        drop();
        grab();
        return _configuration;
    }
    
    WumpusWorld::Chamber *nextChamber = chamber()->passage(WumpusWorld::Orientation::North);
    
    for (int heading = WumpusWorld::Orientation::North; heading <= WumpusWorld::Orientation::NorthWest; heading++) {
        WumpusWorld::Chamber *aChamber = chamber()->passage(static_cast<WumpusWorld::Orientation>(heading));
        
        if (!aChamber) continue;
        
        if (!nextChamber || nextChamber->distanceTo(_destination) > aChamber->distanceTo(_destination)) {
            nextChamber = aChamber;
        }
    }
    
    enter(nextChamber);
    return _configuration;
}

bool GreedyPlayer::finished() const
{
    return _inventory.contains(WumpusWorld::Item::Gold);
}

GreedyPlayer::GreedyPlayer(WumpusWorld::Player::Configuration const configuration, WumpusWorld::Chamber const *const destination):
WumpusWorld::Player(configuration),
_destination(destination)
{
}

GreedyPlayer::~GreedyPlayer()
{
    
}