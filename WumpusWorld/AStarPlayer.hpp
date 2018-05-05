//
//  AStarPlayer.hpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/24/16.
//

#ifndef AStarPlayer_hpp
#define AStarPlayer_hpp

#include <iostream>
#include <vector>
#include "WumpusWorld.hpp"


// The AStarPlayer class extends the WumpusWorld::Player by taking
// an optimized approach to getting to a destination. It essentially
// overwrites a virtual method in the base class that's called
// when it's the player's time to make its next move.
class AStarPlayer : public WumpusWorld::Player {
protected:

    // This class serves as a special queue designed specifically to deal with
    // an AStarPlayer in WumpusWorld. This queue should NOT BE USED FOR ANYTHING ELSE.
    class Queue {
    public:
        class Node {
        public:
            // Overloading this operator for the std::priority_queue
            bool operator<(Node const &that) const;
            bool operator==(Node const &that) const;
            Node &operator=(Node const &that);

            WumpusWorld::Chamber *chamber;

            int gCost(Node const *parent = nullptr) const;
            int hCost() const;
            int fCost() const;

            Node const *parent() const;
            void setParent(Node *node);

            bool invalid() const;

            Node(WumpusWorld::Chamber * const chamber = nullptr,
                 WumpusWorld::Chamber const * const destination = nullptr,
                 Node const *parent = nullptr);
            ~Node();

        protected:
            WumpusWorld::Chamber const *_destination;
            Node const *_parent;
        };

        void push(Node *node);
        Node* pop();
        Node* peek() const;

        bool empty() const;
        bool contains(Node const &node) const;

        void destroy();

        Node* seek(Node const &node) const;

    protected:
        std::vector<Node*> _data;
    };



public:

    // The following two methods are being overridden to have
    // polymorphism help out with player abstraction. This should
    // make it easier to write different players that can do different things!
    Configuration nextMove();
    bool finished() const;

    AStarPlayer(Configuration const configuration, WumpusWorld::Chamber const * const destination);
    ~AStarPlayer();


protected:

    std::vector<WumpusWorld::Chamber *> _solution;
    WumpusWorld::Chamber const * const _destination;

    void _resolvePath();
};

#endif /* AStarPlayer_hpp */
