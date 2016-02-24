//
//  CLIWumpusWorld.cpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/22/16.
//  Copyright © 2016 Matias Barcenas. All rights reserved.
//

#include "CLIWumpusWorld.hpp"


// ================================================================
#pragma mark - Chamber Content Implementation
// ================================================================
CLIWumpusWorld::ChamberContent::ChamberContent():
maxLineCount(0)
{}


// ================================================================
#pragma mark - CLIWumpusWorld Implementation
// ================================================================
std::istream& operator>>(std::istream& is, CLIWumpusWorld& wumpusWorld)
{
    // Implement later
    return is;
}

std::ostream& operator<<(std::ostream& os, CLIWumpusWorld const &wumpusWorld)
{
    CLIWumpusWorld::ChamberContent chamberContent;
    wumpusWorld.get(chamberContent);
    wumpusWorld.getPlayerOn(chamberContent);
    
    int const columnCount = wumpusWorld.size();
    int const rowCount = wumpusWorld.size();
    
    // Reset the output stream
    os << std::left << std::setw(0) << std::setfill(' ');
    
    for (int row = 0; row < rowCount; row++) {
        os << wumpusWorld.getRowSeparator();
        os << wumpusWorld.getRowSpacer();
        
        for (int x = 0; x < chamberContent.maxLineCount+2; x++) {
            for (int y = 0; y < columnCount; y++) {
                int const index = columnCount * (row+1) - columnCount + y;
                
                std::string line;
                
                if (chamberContent.data[index].good()) {
                    std::getline(chamberContent.data[index], line);
                }
                
                os << std::left << "| " << std::setw(CLIWumpusWorld::ChamberWidth-2) << line << " ";
            }
            os << '|' << std::endl;
        }
        
        os << wumpusWorld.getRowSpacer();
    }
    
    os << wumpusWorld.getRowSeparator();
    
    // Reset the output stream
    os << std::left << std::setw(0) << std::setfill(' ');
    
    return os;
}

int CLIWumpusWorld::score() const
{
    return player.score();
}

void CLIWumpusWorld::run()
{
    while (playable()) {
        std::cout << *this;
        
        std::cout << getHeadsUpDisplayBorder();
        std::cout << getHeadsUpDisplay();
        takeTurn();
        std::cout << getHeadsUpDisplayBorder();
        std::cout << std::endl << std::endl;
    }
    
    std::cout << *this;
    std::cout << getHeadsUpDisplayBorder();
    std::cout << getHeadsUpDisplay();
    std::cout << getHeadsUpDisplayBorder();
    std::cout << getResults();
}

void CLIWumpusWorld::get(CLIWumpusWorld::ChamberContent &chamberContent) const
{
    chamberContent.data.resize(_chamber.size());
    int maxiumHeight  = 0;
    
    // I'm treating every row of squares as a line that is outputted to the stringstream
    // I'll later use this string stream to fetch a row from each depending on what row of the
    // board I'm about to print. I'm keeping a "height" variable to make all squares have the
    // same height.
    for (int i = 0; i < _chamber.size(); i++) {
        chamberContent.data[i] << chamber(i);
        int featureCount = WumpusWorld::Chamber::Count(chamber(i).features())
                         + WumpusWorld::Chamber::Count(chamber(i).environment());
        maxiumHeight = featureCount > maxiumHeight? featureCount : maxiumHeight;
    }
    
    chamberContent.maxLineCount = maxiumHeight;
}

void CLIWumpusWorld::getPlayerOn(CLIWumpusWorld::ChamberContent &chamberContent) const
{
    chamberContent.data[player.location()] << player;
}

std::string CLIWumpusWorld::getRowSeparator() const
{
    std::stringstream output;
    output << std::setfill('-') << std::left;
    for (int column = 0; column < size(); column++) {
        output << std::setw(CLIWumpusWorld::ChamberWidth+1) << '+';
    }
    output << '+' << std::endl;
    return output.str();
}

std::string CLIWumpusWorld::getRowSpacer() const
{
    std::stringstream output;
    output << std::setfill(' ') << std::left;
    for (int column = 0; column < size(); column++) {
        output << std::setw(CLIWumpusWorld::ChamberWidth+1) << '|';
    }
    output << '|' << std::endl;
    return output.str();
}

std::string CLIWumpusWorld::getHeadsUpDisplay() const
{
    std::stringstream output;
    output << " Percepts: " << WumpusWorld::Player::Sensory(player.sense()) << std::endl;
    output << "Inventory: " << player.inventory() << std::endl;
    output << "    Score: " << score() << std::endl;
    return output.str();
}

std::string CLIWumpusWorld::getHeadsUpDisplayBorder() const
{
    std::stringstream output;
    output << std::setfill('=');
    output << std::setw((CLIWumpusWorld::ChamberWidth) * size() + size() + 1);
    output << '=' << std::endl;
    return output.str();
}

std::string CLIWumpusWorld::getResults() const
{
    std::stringstream output;
    if (!player.alive()) {
        if (_chamber[player.location()]->contains(WumpusWorld::Chamber::Feature::LivingWumpus))
            std::cout << "[ YOU WERE EATEN BY THE WUMPUS! ]" << std::endl;
        
        if (_chamber[player.location()]->contains(WumpusWorld::Chamber::Feature::Pit))
            std::cout << "[ YOU FELL TO YOUR DEATH! ]" << std::endl;
    }
    
    std::cout << "Final Score: " << getFinalScore() << std::endl;
    return output.str();
}

int CLIWumpusWorld::getFinalScore() const
{
    return player.score();
}

CLIWumpusWorld::CLIWumpusWorld(Configuration const configuration):
WumpusWorld(configuration)
{
    
}

CLIWumpusWorld::CLIWumpusWorld(int size):
WumpusWorld(size)
{
    
}