//
//  ACOPlayer.hpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 4/19/16.
//  Copyright © 2016 Matias Barcenas. All rights reserved.
//

#ifndef ACOPlayer_hpp
#define ACOPlayer_hpp

#include <ctime>
#include <random>
#include <vector>
#include <numeric>
#include <algorithm>
#include "WumpusWorld.hpp"

//#define ACO_GENERATIONS_CONVERGENCE_LOG

// The ACOPlayer class extends the WumpusWorld::Player.
// The class aims to represent a player capable of following
// the ant colony optimization algorithm for a valid path to the gold.
// The class overwrites a virtual method in the base class that is
// called when it's the player's time to make its next move.
class ACOPlayer : public WumpusWorld::Agent {
public:

    static const int Generations    = 1000;
    static constexpr float Alpha    = 2.00;
    static constexpr float Beta     = 0.95;
    static constexpr float Rho      = 0.33;

    // The following two methods are being overridden to have
    // polymorphism help out with player abstraction. This should
    // make it easier to write different players that can do different things!
    Configuration nextMove();
    bool finished() const;

    ACOPlayer(Configuration const configuration, WumpusWorld::Chamber *const destination);
    ~ACOPlayer();


protected:
    typedef std::vector<WumpusWorld::Chamber *> Chambers;

    class Edge {
    public:
        static constexpr float DefaultWeight = 0.1;

        float weight;

        WumpusWorld::Chamber const *nodeA, *nodeB;

        bool operator==(const Edge& edge) const;
        bool operator>(const Edge& edge) const;
        bool operator<(const Edge& edge) const;

        Edge(WumpusWorld::Chamber const *nodeA,
             WumpusWorld::Chamber const *nodeB,
             const float weight = DefaultWeight);
    };

    typedef std::vector<Edge> Edges;

    class Ant {
    public:
        static std::default_random_engine Randomizer;

        Chambers tour();
        void stigmergy();

        bool done() const;

        Chambers path() const;

        Ant(WumpusWorld::Chamber * const start, WumpusWorld::Chamber * const end, Edges &edges);
        Ant(const Ant& ant);

    protected:
        Chambers _path;
        Edges &_edges;
        Edges _edgePath;

        WumpusWorld::Chamber * const _start;
        WumpusWorld::Chamber * const _end;
        WumpusWorld::Chamber * _current;

        WumpusWorld::Chamber * _next() const;

        std::vector<WumpusWorld::Chamber*> _getUnvisitedNeighbors() const;

        // Calculates the probability to move to "target" chamber.
        float _p(WumpusWorld::Chamber const * const target) const;

        // Calculates the amount of pheromone for the edge leading to "target."
        float _t(WumpusWorld::Chamber const * const target) const;

        // Calculates the heuristic value for the "target."
        float _h(WumpusWorld::Chamber const * const target) const;
    };

    typedef std::vector<Ant> Ants;

    std::vector<WumpusWorld::Chamber *> _solution;
    WumpusWorld::Chamber * const _destination;
    
    void _resolvePath();
};


#endif /* ACOPlayer_hpp */
