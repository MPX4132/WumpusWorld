//
//  WumpusWorld.hpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/10/16.
//

#ifndef WumpusWorld_hpp
#define WumpusWorld_hpp

#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
#include <cmath>


// The WumpusWorld class represents a virtual turn-based sandbox for AI agents.
// Agents wishing to interact with the world must subclass Agent and implement
// relevant virtual methods to successfully interact with the world.
class WumpusWorld {
public:

    // The following are the valid orientations of the world, enumerated
    // clock-wise, 0 - 7 (8 total), North being 0, and NorthWest being 7.
    enum Orientation {
        North,
        NorthEast,
        East,
        SouthEast,
        South,
        SouthWest,
        West,
        NorthWest
    };

    class Chamber;

    // The Coordinates struct defines a coordinate pair in the world,
    // defining the x and y axis specifying a "Chamber" of the world.
    struct Coordinates {
        typedef long Domain;

        Domain x, y;
        
        friend std::istream &operator>>(std::istream& is, Coordinates &coordinates);
        friend std::ostream &operator<<(std::ostream& os, Coordinates const &coordinates);
        
        Domain xDistanceTo(Coordinates const &coordinates) const;
        Domain yDistanceTo(Coordinates const &coordinates) const;
        
        Coordinates operator+(Coordinates const &coordinates) const;
        Coordinates operator-(Coordinates const &coordinates) const;

        bool operator==(Coordinates const &coordinates) const;
        bool operator!=(Coordinates const &coordinates) const;
        
        float distanceTo(Coordinates const &coordinates) const;
        
        static Coordinates Dereference(Domain const size, Domain const coordinates);
        static Domain Reference(Domain const size, Coordinates const coordinates);
        
        Coordinates(Domain const x = 0, Domain const y = 0);
    };


    // The following struct represents an initial configuration of the world.
    // A configuration is taken by the constructor and loads values per line.
    // The configuration file must follow the following format (omit line #):
    // Line 1: <world size (squared)>
    // Line 2: <agent entry X> <agent entry Y>
    // Line 3: <agent orientation>
    // Line 4: [ <pit1 X> <pit1 Y> [ <pit2 X> <pit2 Y> [ ... ] ] ]
    // Line 5: <gold location X> <gold location Y>
    // Line 6: <Wumpus location X> <Wumpus location Y>
    struct Configuration {
        Coordinates::Domain size;               // Size of the board (will be squared)
        Coordinates::Domain entry;              // Starting point of the agent
        Orientation orientation;                // Starting orientation of the agent
        std::vector<Coordinates::Domain> pits;  // Coordinates of chambers containing a pit
        Coordinates::Domain gold;               // Coordinates of the chamber that contains gold
        Coordinates::Domain wumpus;             // Coordinates of the chamber where the wumpus resides

        bool valid() const;
        
        Configuration(std::fstream &file);
    };


    // The Inventory class serves as the inventory for agents and world chambers.
    // The class represents a type of storage for both, agents and chambers.
    // Anything that can be had/held/hold must be stored in an inventory.
    class Inventory {
    public:
        // The following are valid items in the world, including a special "item",
        // Air, that is automatically ignored upon storage or withdrawl.
        enum Item {
            Air, // Ignored by the inventory by default.
            Bow,
            Gold
        };

        static std::vector<std::string> const ItemIdentiy;

        friend std::istream& operator>>(std::istream& is, Inventory &inventory);
        friend std::ostream& operator<<(std::ostream& os, Inventory const &inventory);
        
        bool push(Item const item);
        Item pop(Item const item);
        Item pop();
        
        bool contains(Item const item) const;
        
        int size() const;
        int capacity() const;
        
        bool empty() const;
        
        Inventory(int const capacity = 1);
        
        
    protected:
        std::map<Item, unsigned long> _items; // Item type : Item Count
        int const _itemCapacity;
        int _itemCount;
    };



//    // The class below represents a Zone navigatable by agents within the world.
//    // These zones may have a multitude of Features, such as a Wumpus, or a Pit.
//    // Their environment depends on neighboring zones which generate "Percepts"
//    // that are detectable by agents.
//    class Zone {
//    public:
//        // The following enumerations are using bit-shifted values to make boolean
//        // operations easy, simplifying the usage of Features and Percepts, since
//        // multiple may be passed from a single Feature/Percept value.
//        enum Feature {
//            Empty           = 0,
//            Pit             = 1 << 0,
//            LivingWumpus    = 1 << 1,
//            DeadWumpus      = 1 << 2,
//            Exit            = 1 << 3
//        };
//
//        enum Percept {
//            Nothing     = 0,
//            Stench      = 1 << 0,
//            Breeze      = 1 << 1,
//            Glitter     = 1 << 2,
//            Bump        = 1 << 3,
//            Scream      = 1 << 4
//        };
//
//        Zone();
//        ~Zone();
//
//    protected:
//        void _addFeature(Feature const feature);
//        Feature _removeFeature(Feature const feature);
//
//    private:
//        Feature _feature;
//    };

    // The class below represents a Chamber navigatable by agents within the world.
    // These chambers may have a multitude of Features, such as a Wumpus, or a Pit.
    // Their environment depends on neighboring chambers which generate "Percepts"
    // that are detectable by agents.
    class Chamber {
    public:
        // The following enumerations are using bit-shifted values to make boolean
        // operations easy, simplifying the usage of Features and Percepts, since
        // multiple may be passed from a single Feature/Percept value.
        enum Feature {
            Empty           = 0,
            Pit             = 1 << 0,
            LivingWumpus    = 1 << 1,
            DeadWumpus      = 1 << 2,
            Exit            = 1 << 3
        };
        
        enum Percept {
            Nothing     = 0,
            Stench      = 1 << 0,
            Breeze      = 1 << 1,
            Glitter     = 1 << 2,
            Bump        = 1 << 3,
            Scream      = 1 << 4
        };
        
        Inventory inventory;

        friend std::istream& operator>>(std::istream& is, Chamber &chamber);
        friend std::ostream& operator<<(std::ostream& os, Chamber const &chamber);

        Chamber* passage(Orientation const orientation) const;
        
        int location() const;
        Coordinates coordinates() const;
        float distanceTo(Chamber const *chamber) const;
        Coordinates::Domain eightDistanceTo(Chamber const *chamber) const;
        
        Feature features() const;
        bool contains(Feature const feature) const;
        Percept environment() const;
        bool contains(Percept const percept) const;

        std::vector<Chamber *> neighbors() const;

        bool hit();
        
        
        static int Count(Feature features);
        static int Count(Percept percepts);
        
        Chamber(WumpusWorld const *_world, Feature const features = Empty);
        ~Chamber();

    protected:
        Feature _features;
        WumpusWorld const *_world;
        
        Feature removeFeature(Feature const features);
    };

    
    // This class represents a being (hero), within the world.
    // The class defines all possible actions and abilities a agent is capable
    // of performing within the world.
    class Agent {
    public:
        
        enum Side {
            Left,
            Right
        };
        
        class Configuration {
        public:
            static int const DefaultActionCost  = 1;
            static int const DefaultSpace       = 1;
            
            
            std::string identification; // A string used to identify the agent.
            int space;                  // The inventory limit space.
            int turnCost;               // Cost for making the agent change orientation by 45 degrees.
            int cost;                   // A generic cost value applied to every other action.
            
            Orientation orientation;    // The heading the agent faces in the world.
            Coordinates::Domain location;               // A location in the world.
            
            Configuration(std::string const identification = "",
                          int const space = DefaultSpace,
                          int const cost = DefaultActionCost,
                          int const turnCost = DefaultActionCost,
                          Orientation const orientation = Orientation::North,
                          Coordinates::Domain const location = 0
                          );
        };
        
        friend std::istream& operator>>(std::istream& is, Agent &agent);
        friend std::ostream& operator<<(std::ostream& os, Agent const &agent);
        
        static std::string Sensory(Chamber::Percept percept);
        
        
        std::string identification() const;
        Chamber::Percept sense() const;
        
        Inventory inventory() const;
        
        virtual int score() const;
        
        Coordinates::Domain location() const;
        
        Orientation orientation() const;
        void orient(Orientation const orientation);
        Orientation turn(Side const side);
        
        void forward();
        
        void shoot();
        
        void grab();
        
        void drop();
        
        void climb();
        
        Configuration configuration() const;
        
        Chamber* chamber() const;
        
        bool inChamber() const;
        bool enter(Chamber* chamber);
        
        bool alive() const;
        
        virtual bool finished() const;
        
        virtual Configuration nextMove();
        
        Agent(Configuration const configuration = Configuration());
        virtual ~Agent();
        
        
    protected:
        Configuration _configuration;
        
        Inventory _inventory;
        Chamber* _chamber;
        Inventory::Item _dropped;
        
        int _health;
        int _score;
        
        int _ammo;
        
        bool _bump;
        bool _scream;
        
        bool _finished;
        
        int _damage(int const damage);
        
        bool _isNeighbor(Chamber const *chamber) const;
        int _deductPoints(int const points);
        int _addPoints(int const points);
        void _clearPercepts();
        
        bool _prepareForActionWithCost(int const tax = Configuration::DefaultActionCost);
    };
    
    Chamber* passage(Chamber const *chamber, Orientation const orientation) const;
    
    Chamber& chamber(std::vector<Chamber*>::size_type i) const;
    Chamber* goldChamber() const;
    
    virtual bool playable() const; // Checks if the game is not over
    
    virtual void addAgent(Agent * const agent);
    
    int size() const;
    
    WumpusWorld(Configuration const configuration);
    WumpusWorld(int size);
    ~WumpusWorld();
    
    
protected:
    typedef unsigned char Edge;
    static Edge const Top       = 1;
    static Edge const Right     = 2;
    static Edge const Bottom    = 4;
    static Edge const Left      = 8;
    
    Orientation _defaultOrientation;
    WumpusWorld::Coordinates::Domain _defaultChamber;
    
    std::vector<Agent*> _agent;
    std::vector<Chamber*> _chamber;
    
    int _locate(Chamber const *chamber) const;
    Edge _edge(int const position) const;
    
    virtual void _processRound();
    virtual void _processPlayer(std::vector<Agent*>::size_type const i);
};

#endif
