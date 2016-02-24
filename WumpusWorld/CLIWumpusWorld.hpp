//
//  CLIWumpusWorld.hpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/22/16.
//  Copyright © 2016 Matias Barcenas. All rights reserved.
//

#ifndef CLIWumpusWorld_hpp
#define CLIWumpusWorld_hpp

#include <iostream>
#include "WumpusWorld.hpp"

class CLIWumpusWorld : public WumpusWorld {
public:
    
    struct ChamberContent {
        std::vector<std::stringstream> data;
        int maxLineCount;
        
        ChamberContent();
    };
    
    
    // The following two constants control the size of the board displayed in ASCII
    // ChamberWidth specifies each square's individual width (each room), while
    // DefaultSize specifies the size of the default size of the board itself.
    static int const    DefaultSize     = 4;
    static long const   ChamberWidth    = 15;
    
    friend std::istream& operator>>(std::istream &is, CLIWumpusWorld& wumpusWorld);
    friend std::ostream& operator<<(std::ostream &os, CLIWumpusWorld const &wumpusWorld);
    
    virtual int score() const;
    
    virtual void run(); // Run the game loop (ends upon not playable)
    
    CLIWumpusWorld(Configuration const configuration);
    CLIWumpusWorld(int size = DefaultSize);
    
    
protected:
    virtual void get(ChamberContent &chamberContent) const;
    virtual void getPlayerOn(ChamberContent &chamberContent) const;
    virtual std::string getRowSeparator() const;
    virtual std::string getRowSpacer() const;
    virtual std::string getHeadsUpDisplay() const;
    virtual std::string getHeadsUpDisplayBorder() const;
    virtual std::string getResults() const;
    virtual int getFinalScore() const;
};

#endif /* CLIWumpusWorld_hpp */
