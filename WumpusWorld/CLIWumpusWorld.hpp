//
//  CLIWumpusWorld.hpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/22/16.
//

#ifndef CLIWumpusWorld_hpp
#define CLIWumpusWorld_hpp

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "WumpusWorld.hpp"


// The CLI class, or Command Line Interface class, extends the base class
// for the world and contains utilities to properly display different items
// and take input from the command line.
class CLIWumpusWorld : public WumpusWorld {
public:
    
    struct ChamberContent {
        //std::vector<std::stringstream> data;

		// Must use dynamically allocated pointer due to the fact
		// THE LAB'S G++ IS SO FUCKING OUTDATED, it doesn't even have
		// proper STL support. Kirtley needs to fix that shit, IT'S ANNOYING.
		// Wasted TWO freaking hours reseraching what the hell was the compilation
		// problem when brining it from Mac OS (where it compiles fine) to Ubuntu.
		std::stringstream *data;

		int capacity;

        int maxLineCount;

		void resize(int const capacity);
        
        ChamberContent();
		~ChamberContent();
    };
    
    
    // The following two constants control the size of the board displayed in ASCII
    // ChamberWidth specifies each square's individual width (each room), while
    // DefaultSize specifies the size of the default size of the board itself.
    static int const    DefaultSize     = 4;
    static long const   ChamberWidth    = 15;
    
    friend std::istream& operator>>(std::istream &is, CLIWumpusWorld& wumpusWorld);
    friend std::ostream& operator<<(std::ostream &os, CLIWumpusWorld const &wumpusWorld);

    
    virtual void run(); // Run the game loop (ends upon not playable)
    
    CLIWumpusWorld(Configuration const configuration);
    CLIWumpusWorld(int size = DefaultSize);
    ~CLIWumpusWorld();
    
    
protected:
    virtual void get(ChamberContent &chamberContent) const;
    virtual void getPlayersOn(ChamberContent &chamberContent) const;
    virtual std::string getWorld() const;
    virtual std::string getRowSeparator() const;
    virtual std::string getRowSpacer() const;
    virtual std::string getHeadsUpDisplayPercepts() const;
    virtual std::string getHeadsUpDisplayInventories() const;
    virtual std::string getHeadsUpDisplayScores() const;
    virtual std::string getHeadsUpDisplayControls() const;
    virtual std::string getHeadsUpDisplay() const;
    virtual std::string getHeadsUpDisplayBorder() const;
    virtual std::string getResults() const;
    virtual std::vector<int> getFinalScores() const;
    
    virtual void _processPlayer(int const i);
};

#endif /* CLIWumpusWorld_hpp */
