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


// This is the base class for the world and contains every subclass within it.
// I've created the classes that belong to WumpusWorld nested within it since
// they're used exclusively by the class itself and having nested classes
// creates an isolated namespace, which is considered good practice!
class WumpusWorld {
public:

    // These are the valid orientations of the world, and are enumerated from
    // 0 - 7 (8 total), going clock-wise (North being 0, and NorthWest being 7).
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

    
    // This class defines a coordinate pair, representing a point in the x axis,
    // and a point in the y axis of a two dimensional plane.
    struct Coordinate {
        int x, y;
        
        friend std::istream &operator>>(std::istream& is, Coordinate &coordinate);
        friend std::ostream &operator<<(std::ostream& os, Coordinate const &coordinate);
        
        int xDifference(Coordinate const &coordinate) const;
        int yDifference(Coordinate const &coordinate) const;
        
        //Coordinate operator+(Coordinate const &coordinate) const;
        Coordinate operator-(Coordinate const &coordinate) const;
        bool operator==(Coordinate const &coordinate) const;
        bool operator!=(Coordinate const &coordinate) const;
        
        float distanceTo(Coordinate const &coordinate) const;
        
        static Coordinate Dereference(int const size, int const coordinate);
        static int Reference(int const size, Coordinate const coordinate);
        
        Coordinate(int const x = 0, int const y = 0);
    };


    // This class is designed to represent an initial configuration of the world,
    // as a board confiuration. The constructor takes a file and derives numerical
    // values representing entities in the world, and initial values.
    class Configuration {
    public:
        int size;               // The size of the board (size by size)
        int entry;              // The starting point of the player
        Orientation orientation;// The starting orientation of the player
        std::vector<int> pits;    // The indexes of chambers containing a pit
        int gold;                // The index of the chamber that contains
        int wumpus;                // The inddx of the chamber where the wumpus resides
        
        bool valid() const;        // Checks whether the configuration is valid
        
        
        
        Configuration(std::fstream &file);
        ~Configuration();
    };


    // This class is designed to serve as the inventory for entities of the world, that is,
    // it acts as a storage containing all items the entity holds.
    class Inventory {
    public:
        // These are all the valid items in the world, including a special "item",
        // Air, that is automatically ignored upon storage or pickup.
        enum Item {
            Air, // Ignored, by default
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
        int limit() const;
        
        bool empty() const;
        
        Inventory(int const capacity = 1);
        
        
    protected:
        std::map<Item, int> _items; // Item type : Item Count
        int const _itemCapacity;
        int _itemCount;
    };

    
    // This class represents a "Room" within the cave (a chamber). These chambers may have
    // a multitude of Features, such as a wumpus or a pit. Their environment depends on
    // their surrounding chambers, which generate "Percepts", and are detetable by the player.
    class Chamber {
    public:


        // These enumerations are using bit-shifted values so they work with boolean operations.
        // This simplifies the usage Features and Percepts since the values passed can be multiple
        // on a single Feature/Percept value.
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
        
        friend std::istream& operator>>(std::istream& is, Chamber &chamber);
        friend std::ostream& operator<<(std::ostream& os, Chamber const &chamber);
        
        
        Inventory inventory;
        
        Chamber* passage(Orientation const orientation) const;
        
        int location() const;
        Coordinate coordinate() const;
        float distanceTo(Chamber const *chamber) const;
        int eightDistanceTo(Chamber const *chamber) const;
        
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
    // The class defines all possible actions and abilities a player is capable
    // of performing within the world.
    class Player {
    public:
        
        enum Side {
            Left,
            Right
        };
        
        class Configuration {
        public:
            static int const DefaultActionCost  = 1;
            static int const DefaultSpace       = 1;
            
            
            std::string identification; // A string used to identify the player.
            int space;                  // The inventory limit space.
            int turnCost;               // Cost for making the player change orientation by 45 degrees.
            int cost;                   // A generic cost value applied to every other action.
            
            Orientation orientation;    // The heading the player faces in the world.
            int location;               // A location in the world.
            
            Configuration(std::string const identification = "",
                          int const space = DefaultSpace,
                          int const cost = DefaultActionCost,
                          int const turnCost = DefaultActionCost,
                          Orientation const orientation = Orientation::North,
                          int const location = 0
                          );
        };
        
        friend std::istream& operator>>(std::istream& is, Player &player);
        friend std::ostream& operator<<(std::ostream& os, Player const &player);
        
        static std::string Sensory(Chamber::Percept percept);
        
        
        std::string identification() const;
        Chamber::Percept sense() const;
        
        Inventory inventory() const;
        
        virtual int score() const;
        
        int location() const;
        
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
        
        Player(Configuration const configuration = Configuration());
        virtual ~Player();
        
        
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
    
    virtual void addPlayer(Player * const player);
    
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
    int _defaultChamber;
    
    std::vector<Player*> _player;
    std::vector<Chamber*> _chamber;
    
    int _locate(Chamber const *chamber) const;
    Edge _edge(int const position) const;
    
    virtual void _processRound();
    virtual void _processPlayer(std::vector<Player*>::size_type const i);
};

#endif
