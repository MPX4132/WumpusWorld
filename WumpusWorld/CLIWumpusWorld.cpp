//
//  CLIWumpusWorld.cpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/22/16.
//

#include "CLIWumpusWorld.hpp"


// ================================================================
#pragma mark - Chamber Content Implementation
// ================================================================
void CLIWumpusWorld::ChamberContent::resize(int const capacity) // THIS IS BULLSHIT, GET FREAKING STL SUPPORT
{
	std::stringstream *newData = new std::stringstream[capacity]();

	if (data) {
		for (int i = 0; i < this->capacity; i++) {
			newData[i] << data[i].str();
		}

		delete [] data;
	}

	data = newData;
	this->capacity = capacity;
}

CLIWumpusWorld::ChamberContent::ChamberContent():
data(nullptr),
capacity(0),
maxLineCount(0)
{}

CLIWumpusWorld::ChamberContent::~ChamberContent()
{
	delete [] data;
}


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
    wumpusWorld.getPlayersOn(chamberContent);
    
    int const columnCount = wumpusWorld.size();
    int const rowCount = wumpusWorld.size();
    
    // Reset the output stream
    os << std::left << std::setw(0) << std::setfill(' ');
    
    for (int row = 0; row < rowCount; row++) {
        os << wumpusWorld.getRowSeparator();
        os << wumpusWorld.getRowSpacer();
        
        for (std::vector<WumpusWorld::Player*>::size_type x = 0; x < chamberContent.maxLineCount + wumpusWorld._player.size() + 1; x++) {
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

void CLIWumpusWorld::run()
{
    while (playable()) {
        std::cout << getWorld();
        std::cout << getHeadsUpDisplayBorder();
        std::cout << getHeadsUpDisplay();
        _processRound();
        std::cout << getHeadsUpDisplayBorder();
        std::cout << std::endl << std::endl;
    }
    
    std::cout << getWorld();
    std::cout << getHeadsUpDisplayBorder();
    std::cout << getHeadsUpDisplay();
    std::cout << getHeadsUpDisplayBorder();
    std::cout << getResults();
}

void CLIWumpusWorld::_processPlayer(int const i)
{
    std::cout << "Player " << _player[i]->identification() << " Action: ";
    _player[i]->nextMove();
}

void CLIWumpusWorld::get(CLIWumpusWorld::ChamberContent &chamberContent) const
{
	//chamberContent.data.resize(_chamber.size());
	chamberContent.resize(static_cast<int>(_chamber.size()));
    int maxiumHeight  = 0;
    
    // I'm treating every row of squares as a line that is outputted to the stringstream
    // I'll later use this string stream to fetch a row from each depending on what row of the
    // board I'm about to print. I'm keeping a "height" variable to make all squares have the
    // same height.
    for (std::vector<WumpusWorld::Chamber*>::size_type i = 0; i < _chamber.size(); i++) {
        chamberContent.data[i] << chamber(i);
        int featureCount = WumpusWorld::Chamber::Count(chamber(i).features())
                         + WumpusWorld::Chamber::Count(chamber(i).environment());
        maxiumHeight = featureCount > maxiumHeight? featureCount : maxiumHeight;
    }
    
    chamberContent.maxLineCount = maxiumHeight;
}

void CLIWumpusWorld::getPlayersOn(CLIWumpusWorld::ChamberContent &chamberContent) const
{
    for (WumpusWorld::Player const * const aPlayer : _player) {
        chamberContent.data[aPlayer->location()] << "[" << aPlayer->identification() << "]";
        
        // If the player is dead, show it and skip its information.
        if (!aPlayer->alive()) {
            chamberContent.data[aPlayer->location()] << "-DEAD" << std::endl;
            continue;
        }
        
        chamberContent.data[aPlayer->location()] << *aPlayer << std::endl;
    }
}

std::string CLIWumpusWorld::getWorld() const
{
    std::stringstream output;
    output << *this;
    return output.str();
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

std::string CLIWumpusWorld::getHeadsUpDisplayPercepts() const
{
    std::stringstream output;
    output << " Percepts ";
    for (std::vector<WumpusWorld::Player*>::size_type i = 0; i < _player.size(); i++) {
        output << "[" << _player[i]->identification() << ": " << WumpusWorld::Player::Sensory(_player[i]->sense()) << "] ";
    }
    output << std::endl;
    return output.str();
}

std::string CLIWumpusWorld::getHeadsUpDisplayInventories() const
{
    std::stringstream output;
    output << "Inventory ";
    for (std::vector<WumpusWorld::Player*>::size_type i = 0; i < _player.size(); i++) {
        output << "[" << _player[i]->identification() << ": " << _player[i]->inventory() << "] ";
    }
    output << std::endl;
    return output.str();
}

std::string CLIWumpusWorld::getHeadsUpDisplayScores() const
{
    std::stringstream output;
    output << "    Score ";
    for (std::vector<WumpusWorld::Player*>::size_type i = 0; i < _player.size(); i++) {
        output << "[" << _player[i]->identification() << ": " << _player[i]->score() << "] ";
    }
    output << std::endl;
    return output.str();
}

std::string CLIWumpusWorld::getHeadsUpDisplayControls() const
{
    std::stringstream output;
    
    // Spacing...
    output << std::endl;
    
    std::string const nav = "R - Turn Right | F - Forward | L - Turn Left";
    std::string const aux = "D - Drop | G - Grab | S - Shoot | C - Climb Out";
    
    output << nav << std::endl;
    output << aux << std::endl;
    
    return output.str();
}

std::string CLIWumpusWorld::getHeadsUpDisplay() const
{
    std::stringstream output;
    output << getHeadsUpDisplayPercepts();
    output << getHeadsUpDisplayInventories();
    output << getHeadsUpDisplayScores();
    
    // Control display ...
    output << getHeadsUpDisplayControls();
    
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
    
    if (_player.size() == 1 && !_player[0]->alive()) {
        if (_chamber[_player[0]->location()]->contains(WumpusWorld::Chamber::Feature::LivingWumpus))
            output << "[ YOU WERE EATEN BY THE WUMPUS! ]" << std::endl;
        
        if (_chamber[_player[0]->location()]->contains(WumpusWorld::Chamber::Feature::Pit))
            output << "[ YOU FELL TO YOUR DEATH! ]" << std::endl;
    }
    
    std::cout << "Final Score ";
    std::vector<int> score = getFinalScores();
    
    for (std::vector<int>::size_type i = 0; i < score.size(); i++) {
        std::cout << '[' << _player[i]->identification() << ": " << score[i] << "] ";
    }
    
    std::cout << std::endl;
    return output.str();
}

std::vector<int> CLIWumpusWorld::getFinalScores() const
{
    std::vector<int> results;
    for (WumpusWorld::Player const * const aPlayer : _player) {
        results.push_back(aPlayer->score());
    }
    return results;
}

CLIWumpusWorld::CLIWumpusWorld(Configuration const configuration):
WumpusWorld(configuration)
{
    
}

CLIWumpusWorld::CLIWumpusWorld(int size):
WumpusWorld(size)
{
    
}

CLIWumpusWorld::~CLIWumpusWorld()
{
    
}
