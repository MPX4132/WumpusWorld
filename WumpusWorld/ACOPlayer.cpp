//
//  ACOPlayer.cpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 4/19/16.
//  Copyright © 2016 Matias Barcenas. All rights reserved.
//

#include "ACOPlayer.hpp"


std::default_random_engine ACOPlayer::Ant::Randomizer = std::default_random_engine(0);

// ================================================================
#pragma mark - Player Overwritten Implementation
// ================================================================
WumpusWorld::Player::Configuration ACOPlayer::nextMove()
{
    // If the algorithm does not have a solution yet, get one.
    if (_solution.empty()) _resolvePath();

    if (chamber()->contains(WumpusWorld::Chamber::Percept::Glitter)) {
        drop();
        grab();
    }

    // For each chamber in the solution, step into it every move.
    WumpusWorld::Chamber const * const nextChamber = _solution.back();
    _solution.pop_back(); // Remove the chamber from the solution

    for (WumpusWorld::Chamber *aNeighbor : chamber()->neighbors()) {
        if (aNeighbor == nextChamber) {
            enter(aNeighbor);
            break;
        }
    }

    return _configuration;
}

bool ACOPlayer::finished() const
{
    return _inventory.contains(WumpusWorld::Inventory::Item::Gold);
}

void ACOPlayer::_resolvePath()
{
    Edges edges;

    for (int generation = 0; generation < ACOPlayer::Generations; generation++) {
        Ants ants(10, Ant(chamber(), _destination, edges));

        for (Ant& ant : ants) {
            ant.tour();
        }

        // Global evaporation
        for (Edge& edge : edges) {
            edge.weight -= edge.weight * ACOPlayer::Rho;
        }

        // Update pheromone (weights)
        for (Ant& ant : ants) {
            ant.stigmergy(); // Lay down pheromone
        }
    }

    Ant ant(chamber(), _destination, edges);
    _solution = ant.tour();
    std::reverse(_solution.begin(), _solution.end());
}


// ================================================================
#pragma mark - Edge Implementation
// ================================================================
bool ACOPlayer::Edge::operator==(const ACOPlayer::Edge& edge) const
{
    return (this->nodeA == edge.nodeA && this->nodeB == edge.nodeB) ||
    (this->nodeA == edge.nodeB && this->nodeB == edge.nodeA);
}

bool ACOPlayer::Edge::operator>(const ACOPlayer::Edge& edge) const
{
    return weight > edge.weight;
}

bool ACOPlayer::Edge::operator<(const ACOPlayer::Edge& edge) const
{
    return weight < edge.weight;
}

ACOPlayer::Edge::Edge(
                      WumpusWorld::Chamber const *nodeA,
                      WumpusWorld::Chamber const *nodeB,
                      const float weigth):
weight(weigth), nodeA(nodeA), nodeB(nodeB)
{
}

// ================================================================
#pragma mark - Ant Implementation
// ================================================================
ACOPlayer::Chambers ACOPlayer::Ant::tour()
{
    _current = _start;

    while (!done()) {
        _path.push_back(_current);
        WumpusWorld::Chamber *last = _current;

        if ((_current = _next())) {
            _edgePath.push_back(Edge(last, _current));
            continue;
        }

        // If path blocked, backtrack...
        // Restart for now
        _path.clear();
        _edgePath.clear();
        _current = _start;
    }

    _path.push_back(_current);

    return _path;
}

void ACOPlayer::Ant::stigmergy()
{
    for (Edge &edge : _edgePath) {
        Edges::iterator it = std::find(_edges.begin(), _edges.end(), edge);

        if (it == _edges.end()) continue;

        it->weight +=  1.0 / _path.size();
    }
}

bool ACOPlayer::Ant::done() const
{
    return _current == _end;
}

ACOPlayer::Chambers ACOPlayer::Ant::path() const
{
    return _path;
}

WumpusWorld::Chamber *ACOPlayer::Ant::Ant::_next() const
{
    // Candidates are unvisited neighbors
    Chambers candidate(_getUnvisitedNeighbors());
    std::vector<float> probability(candidate.size());

    // Check if path is blocked!
    if (candidate.empty()) return nullptr;

    // Calculate the probability of each candidate in order.
    for (Chambers::size_type i = 0; i < candidate.size(); i++) {
        probability[i] = _t(candidate[i]) * (static_cast<float>(1) / std::pow(_h(candidate[i]), ACOPlayer::Beta));
    }

    float total = std::accumulate(probability.begin(), probability.end(), 0.0);

    for (float &p : probability) {
        p /= total;
    }

    float chosen = static_cast<float>(Ant::Randomizer()) / std::default_random_engine::max();

    int i = 0;
    float sum = 0;

    while (sum <= chosen) {
        sum += probability[i++];
    }

    return candidate[i-1];
}

std::vector<WumpusWorld::Chamber*> ACOPlayer::Ant::_getUnvisitedNeighbors() const
{
    std::vector<WumpusWorld::Chamber*> candidates;
    for (int i = 0; i <= WumpusWorld::Orientation::NorthWest; i++) {
        WumpusWorld::Chamber *target = _current->passage(static_cast<WumpusWorld::Orientation>(i));
        if (!target || std::find(_path.begin(), _path.end(), target) != _path.end()) continue;
        candidates.push_back(target);
    }
    return candidates;
}

float ACOPlayer::Ant::_p(WumpusWorld::Chamber const * const target) const
{
    return 0;
}

float ACOPlayer::Ant::_t(WumpusWorld::Chamber const * const target) const
{
    Edges::iterator it = std::find(_edges.begin(), _edges.end(), Edge(_current, target));

    // If the edge was found, return its weight
    if (it != _edges.end()) return it->weight;

    // If it wasn't found, create an edge to it.
    Edge edge(_current, target);
    _edges.push_back(edge);
    return edge.weight;
}

float ACOPlayer::Ant::_h(WumpusWorld::Chamber const * const target) const
{
    // A score of 5 for moving to adjacent squares.
    int cost = 5;

    WumpusWorld::Chamber::Percept environment = target->environment();

    cost += environment & WumpusWorld::Chamber::Percept::Stench? 25 : 0;
    cost += environment & WumpusWorld::Chamber::Percept::Breeze? 25 : 0;

    cost += target->contains(WumpusWorld::Chamber::Feature::Pit)? 500 : 0;
    cost += target->contains(WumpusWorld::Chamber::Feature::LivingWumpus)? 500 : 0;

    cost += target->eightDistanceTo(_end) * 5;

    return cost;
}

ACOPlayer::Ant::Ant(WumpusWorld::Chamber * const start,
                    WumpusWorld::Chamber * const end,
                    ACOPlayer::Edges &edges):
_edges(edges), _start(start), _end(end)
{
    static bool seeded = false;
    if (!seeded) {
        ACOPlayer::Ant::Randomizer = std::default_random_engine(static_cast<unsigned int>(std::clock()));
        seeded = true;
    }
}

ACOPlayer::Ant::Ant(const ACOPlayer::Ant &ant):
_edges(ant._edges), _start(ant._start), _end(ant._end)
{
    static bool seeded = false;
    if (!seeded) {
        ACOPlayer::Ant::Randomizer = std::default_random_engine(static_cast<unsigned int>(std::clock()));
        seeded = true;
    }
}


// ================================================================
#pragma mark - ACOPlayer Implementation
// ================================================================
ACOPlayer::ACOPlayer(Configuration const configuration, WumpusWorld::Chamber *const destination):
WumpusWorld::Player(configuration),
_destination(destination)
{}

ACOPlayer::~ACOPlayer()
{}
