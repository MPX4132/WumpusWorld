//
//  WumpusWorld.cpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/10/16.
//

#include "WumpusWorld.hpp"


// ================================================================
#pragma mark - Coordinates Implementation
// ================================================================
std::istream &operator>>(std::istream& is, WumpusWorld::Coordinates &coordinates)
{
    is >> coordinates.x;
    is >> coordinates.y;
    return is;
}

std::ostream &operator<<(std::ostream& os, WumpusWorld::Coordinates const &coordinates)
{
    os << "(" << coordinates.x << ", " << coordinates.y << ")";
    return os;
}

WumpusWorld::Coordinates::Domain WumpusWorld::Coordinates::xDistanceTo(WumpusWorld::Coordinates const &coordinates) const
{
    return x - coordinates.x;
}

WumpusWorld::Coordinates::Domain WumpusWorld::Coordinates::yDistanceTo(WumpusWorld::Coordinates const &coordinates) const
{
    return y - coordinates.y;
}

float WumpusWorld::Coordinates::distanceTo(WumpusWorld::Coordinates const &coordinates) const
{
    return sqrt(std::pow(xDistanceTo(coordinates), 2) +
                std::pow(yDistanceTo(coordinates), 2));
}

WumpusWorld::Coordinates WumpusWorld::Coordinates::operator+(WumpusWorld::Coordinates const &coordinates) const
{
    return WumpusWorld::Coordinates(x + coordinates.x, y + coordinates.y);
}

WumpusWorld::Coordinates WumpusWorld::Coordinates::operator-(WumpusWorld::Coordinates const &coordinates) const
{
    return WumpusWorld::Coordinates(x - coordinates.x, y - coordinates.y);
}

bool WumpusWorld::Coordinates::operator==(Coordinates const &coordinates) const
{
    return x == coordinates.x && y == coordinates.y;
}

bool WumpusWorld::Coordinates::operator!=(Coordinates const &coordinates) const
{
    return x != coordinates.x || y != coordinates.y;
}

WumpusWorld::Coordinates WumpusWorld::Coordinates::Dereference(WumpusWorld::Coordinates::Domain const size, WumpusWorld::Coordinates::Domain const coordinates)
{
    return WumpusWorld::Coordinates(coordinates % size, coordinates / size);
}

WumpusWorld::Coordinates::Domain WumpusWorld::Coordinates::Reference(WumpusWorld::Coordinates::Domain const size, WumpusWorld::Coordinates const coordinates)
{
    return coordinates.x + (size * coordinates.y);
}

WumpusWorld::Coordinates::Coordinates(WumpusWorld::Coordinates::Domain const x, WumpusWorld::Coordinates::Domain const y):
x(x), y(y)
{}


// ================================================================
#pragma mark - Configuration Implementation
// ================================================================
bool WumpusWorld::Configuration::valid() const
{
    if (!size) return false;

    int locationLimit = std::pow(size, 2);

    if (entry >= locationLimit) return false;

    if (orientation < North || orientation > NorthWest) return false;

    for (WumpusWorld::Coordinates::Domain const pitLocation : pits)
    {
        if (pitLocation >= locationLimit) return false;
    }

    if (gold >= locationLimit) return false;

    if (wumpus >= locationLimit) return false;

    return true;
}

WumpusWorld::Configuration::Configuration(std::fstream &file)
{
    if (!file.good())
    {
        std::cout << "Bad input file or not found..." << std::endl;
        return;
    }
    
    std::string line;
    
    
    // Configuration file's first line is world size.
    std::getline(file, line);
    std::stringstream boardSize(line);
    boardSize >> size;
    
    
    // Configuration file's second line is agent entry point.
    std::getline(file, line);
    std::stringstream startInfo(line);
    WumpusWorld::Coordinates::Domain startX, startY;
    startInfo >> startX >> startY;

    entry = WumpusWorld::Coordinates::Reference(size, WumpusWorld::Coordinates(startX, startY));
    
    
    // Configuration file's third line is agent orientation.
    std::getline(file, line);
    std::stringstream orientationInfo(line);
    int orientation;
    orientationInfo >> orientation;

    this->orientation = static_cast<WumpusWorld::Orientation>(orientation);
    
    
    // Configuration file's fourth line is pits' location.
    std::getline(file, line);
    std::stringstream pitInfo(line);
    
    while (pitInfo.good() && pitInfo.peek() != '\r')
    {
        WumpusWorld::Coordinates::Domain pitX, pitY;
        pitInfo >> pitX >> pitY;

        pits.push_back(WumpusWorld::Coordinates::Reference(size, WumpusWorld::Coordinates(pitX, pitY)));
    }


    // Fifth line is gold's location.
    std::getline(file, line);
    std::stringstream goldInfo(line);
    WumpusWorld::Coordinates::Domain goldX, goldY;
    goldInfo >> goldX >> goldY;

    gold = WumpusWorld::Coordinates::Reference(size, WumpusWorld::Coordinates(goldX, goldY));


    // Sixth line is wumpus' location.
    std::getline(file, line);
    std::stringstream wumpusInfo(line);
    WumpusWorld::Coordinates::Domain wumpusX, wumpusY;
    wumpusInfo >> wumpusX >> wumpusY;

    wumpus = WumpusWorld::Coordinates::Reference(size, WumpusWorld::Coordinates(wumpusX, wumpusY));
}


// ================================================================
#pragma mark - Inventory Implementation
// ================================================================
// The following constant avoids a hideous switch statement.
std::vector<std::string> const WumpusWorld::Inventory::ItemIdentiy({
    "Air",
    "Bow",
    "Gold"
});

std::istream& operator>>(std::istream& is, WumpusWorld::Inventory &inventory)
{
    // Implement later...
    return is;
}

std::ostream& operator<<(std::ostream& os, WumpusWorld::Inventory const &inventory)
{
    for (std::pair<const WumpusWorld::Inventory::Item, unsigned long> const &items : inventory._items)
    {
        for (unsigned long i = 0; i < items.second; i++)
        {
            os << WumpusWorld::Inventory::ItemIdentiy[items.first] << " ";
        }
    }
    return os;
}

bool WumpusWorld::Inventory::push(WumpusWorld::Inventory::Item const item)
{
    if (item == Air || _itemCount >= _itemCapacity) return false;

    _items[item]++; // Automatically created if key does not exist.
    _itemCount++;

    return true;
}

WumpusWorld::Inventory::Item WumpusWorld::Inventory::pop(WumpusWorld::Inventory::Item const item)
{
    if (item == Air || !contains(item)) return Air;

    _items[item]--;
    _itemCount--;

    if (_items[item] == 0) _items.erase(item);

    return item;
}

WumpusWorld::Inventory::Item WumpusWorld::Inventory::pop()
{
    if (empty()) return Air;
    return pop(_items.begin()->first);
}

bool WumpusWorld::Inventory::contains(WumpusWorld::Inventory::Item const item) const
{
    return _items.find(item) != _items.end();
}

int WumpusWorld::Inventory::size() const
{
    return _itemCount;
}

int WumpusWorld::Inventory::capacity() const
{
    return _itemCapacity;
}

bool WumpusWorld::Inventory::empty() const
{
    return _items.empty();
}

WumpusWorld::Inventory::Inventory(int const capacity):
_itemCapacity(capacity),
_itemCount(0)
{}


// ================================================================
#pragma mark - Chamber Implementation
// ================================================================
std::istream& operator>>(std::istream& is, WumpusWorld::Chamber &chamber)
{
    
    return is;
}

std::ostream& operator<<(std::ostream& os, WumpusWorld::Chamber const &chamber)
{
    // Show all features first
    if (chamber.features() != WumpusWorld::Chamber::Feature::Empty) {
        if (chamber.contains(WumpusWorld::Chamber::Feature::LivingWumpus)) {
            os << "Wumpus" << std::endl;
            return os;
        }
        
        if (chamber.contains(WumpusWorld::Chamber::Feature::DeadWumpus)) {
            os << "Dead Wumpus" << std::endl;
            return os;
        }
        
        if (chamber.contains(WumpusWorld::Chamber::Feature::Pit)) {
            os << "Pit" << std::endl;
            return os;
        }
    }
    
    if (chamber.contains(WumpusWorld::Chamber::Feature::Exit)) os << "Start" << std::endl;
    
    // Insert spacer
    os << ' ' << std::endl;
    
    // Show all percepts next
    if (chamber.contains(WumpusWorld::Chamber::Percept::Stench)) os << "(stench)" << std::endl;
    if (chamber.contains(WumpusWorld::Chamber::Percept::Breeze)) os << "(breeze)" << std::endl;
    if (chamber.contains(WumpusWorld::Chamber::Percept::Glitter)) os << "(glitter)" << std::endl;
    if (chamber.contains(WumpusWorld::Chamber::Percept::Bump)) os << "(bump)" << std::endl;
    if (chamber.contains(WumpusWorld::Chamber::Percept::Scream)) os << "(scream)" << std::endl;
    
    return os;
}

WumpusWorld::Chamber* WumpusWorld::Chamber::passage(WumpusWorld::Orientation const orientation) const
{
    return _world->passage(this, orientation);
}

WumpusWorld::Coordinates WumpusWorld::Chamber::coordinates() const
{
    return WumpusWorld::Coordinates::Dereference(_world->size(), location());
}

int WumpusWorld::Chamber::location() const
{
    return _world->_locate(this);
}

float WumpusWorld::Chamber::distanceTo(WumpusWorld::Chamber const *chamber) const
{
    WumpusWorld::Coordinates thisCoordinate(WumpusWorld::Coordinates::Dereference(_world->size(), this->location()));
    WumpusWorld::Coordinates thatCoordinate(WumpusWorld::Coordinates::Dereference(_world->size(), chamber->location()));
    return thisCoordinate.distanceTo(thatCoordinate);
}

WumpusWorld::Coordinates::Domain WumpusWorld::Chamber::eightDistanceTo(Chamber const *chamber) const
{
    WumpusWorld::Coordinates thisCoordinate(WumpusWorld::Coordinates::Dereference(_world->size(), this->location()));
    WumpusWorld::Coordinates thatCoordinate(WumpusWorld::Coordinates::Dereference(_world->size(), chamber->location()));
    WumpusWorld::Coordinates change = thisCoordinate - thatCoordinate;

    return std::max(std::abs(change.x), std::abs(change.y));
}

WumpusWorld::Chamber::Feature WumpusWorld::Chamber::features() const
{
    return _features;
}

bool WumpusWorld::Chamber::contains(WumpusWorld::Chamber::Feature const feature) const
{
    return feature & features();
}

bool WumpusWorld::Chamber::contains(WumpusWorld::Chamber::Percept const percept) const
{
    return percept & environment();
}

WumpusWorld::Chamber::Percept WumpusWorld::Chamber::environment() const
{
    WumpusWorld::Chamber::Percept percepts = Nothing;
    
    // Set the local percepts
    if (inventory.contains(WumpusWorld::Inventory::Item::Gold)) percepts = static_cast<WumpusWorld::Chamber::Percept>(percepts | Glitter);
    
    // Set the side-effect percepts
    for (int orientation = North; orientation <= NorthWest; orientation++) {
        WumpusWorld::Orientation degree = static_cast<WumpusWorld::Orientation>(orientation);
        WumpusWorld::Chamber const *chamber = passage(degree);
        
        if (!chamber) continue;
        
        if (chamber->contains(static_cast<WumpusWorld::Chamber::Feature>(LivingWumpus | DeadWumpus)))
            percepts = static_cast<WumpusWorld::Chamber::Percept>(percepts | Stench);
        
        if (chamber->contains(Pit))
            percepts = static_cast<WumpusWorld::Chamber::Percept>(percepts | Breeze);
    }
    
    return percepts;
}

std::vector<WumpusWorld::Chamber *> WumpusWorld::Chamber::neighbors() const
{
    std::vector<WumpusWorld::Chamber *> neighbors;

    for (int orientation = North; orientation <= NorthWest; orientation++) {
        WumpusWorld::Orientation degree = static_cast<WumpusWorld::Orientation>(orientation);
        WumpusWorld::Chamber *neighbor = passage(degree);
        if (!neighbor) continue;
        neighbors.push_back(neighbor);
    }

    return neighbors;
}

bool WumpusWorld::Chamber::hit() {
    if (!contains(LivingWumpus)) return false;
    
    _features = static_cast<WumpusWorld::Chamber::Feature>(removeFeature(LivingWumpus) | DeadWumpus);
    
    return true;
}

int WumpusWorld::Chamber::Count(WumpusWorld::Chamber::Feature features)
{
    int count = 0;
    
    for (int i = 0; i < static_cast<int>(sizeof(features)); i++) {
        if (features & 1) count++;
        features = static_cast<WumpusWorld::Chamber::Feature>(features >> 1);
    }
    
    return count;
}

int WumpusWorld::Chamber::Count(WumpusWorld::Chamber::Percept percepts)
{
    int count = 0;
    
    for (int i = 0; i < static_cast<int>(sizeof(percepts)); i++) {
        if (percepts & 1) count++;
        percepts = static_cast<WumpusWorld::Chamber::Percept>(percepts >> 1);
    }
    
    return count;
}

WumpusWorld::Chamber::Feature WumpusWorld::Chamber::removeFeature(WumpusWorld::Chamber::Feature const features)
{
    return _features = static_cast<WumpusWorld::Chamber::Feature>((~features) & _features);
}

WumpusWorld::Chamber::Chamber(WumpusWorld const *world, WumpusWorld::Chamber::Feature const features):
inventory(10), _features(features), _world(world)
{}

WumpusWorld::Chamber::~Chamber()
{}


// ================================================================
#pragma mark - Player Configuration Implementation
// ================================================================
WumpusWorld::Agent::Configuration::Configuration(std::string const identification,
                                                  int const space,
                                                  int const cost,
                                                  int const turnCost,
                                                  WumpusWorld::Orientation const orientation,
                                                  WumpusWorld::Coordinates::Domain const location
                                                  ):
identification(identification),
space(space),
turnCost(turnCost),
cost(cost),
orientation(orientation),
location(location)
{
}


// ================================================================
#pragma mark - Player Implementation
// ================================================================
std::istream& operator>>(std::istream& is, WumpusWorld::Agent &agent)
{
//    // If cin is being used for input, output some help text
//    if (&is == &std::cin) {
//        std::string const nav = "R - Turn Right | F - Forward | L - Turn Left";
//        std::string const aux = "D - Drop | G - Grab | S - Shoot | C - Climb Out";
//        
//        std::cout << nav << std::endl;
//        std::cout << aux << std::endl;
//        std::cout << "Action: ";
//    }
    
    // No error checking for now...
    std::string action;
    is >> action;
    
    switch (action.c_str()[0]) {
        case 'F':
            agent.forward();
            break;
            
        case 'R':
            agent.turn(WumpusWorld::Agent::Right);
            break;

        case 'L':
            agent.turn(WumpusWorld::Agent::Left);
            break;
            
        case 'D':
            agent.drop();
            break;
            
        case 'G':
            agent.grab();
            break;
            
        case 'S':
            agent.shoot();
            break;
        
        case 'C':
            agent.climb();
            break;
            
        default:
            std::cout << "Received invalid operation...\nIgnoring..." << std::endl;
            break;
    }
    
    return is;
}

std::ostream& operator<<(std::ostream& os, WumpusWorld::Agent const &agent)
{
    char pointer[8] = {'^', '/', '>', '\\', 'v', '/', '<', '\\'};
    os << pointer[agent.orientation()];
    
    std::string label[8] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
    os << " (" << label[agent.orientation()] << ")";
    
    return os;
}

std::string WumpusWorld::Agent::Sensory(WumpusWorld::Chamber::Percept percept)
{
    std::string output;
    
    if (percept & WumpusWorld::Chamber::Percept::Stench) {
        output.append("stench ");
    }
    
    if (percept & WumpusWorld::Chamber::Percept::Breeze) {
        output.append("breeze ");
    }
    
    if (percept & WumpusWorld::Chamber::Percept::Glitter) {
        output.append("glitter ");
    }
    
    if (percept & WumpusWorld::Chamber::Percept::Bump) {
        output.append("bump ");
    }
    
    if (percept & WumpusWorld::Chamber::Percept::Scream) {
        output.append("scream");
    }
    
    if (output.empty()) output = "None";
    
    return output;
}

std::string WumpusWorld::Agent::identification() const
{
    return _configuration.identification;
}

WumpusWorld::Chamber::Percept WumpusWorld::Agent::sense() const
{
    WumpusWorld::Chamber::Percept bump = _bump? WumpusWorld::Chamber::Bump : WumpusWorld::Chamber::Nothing;
    WumpusWorld::Chamber::Percept scream = _scream? WumpusWorld::Chamber::Scream : WumpusWorld::Chamber::Nothing;
    
    return static_cast<WumpusWorld::Chamber::Percept>((inChamber()? _chamber->environment() : 0) | bump | scream);
}

WumpusWorld::Inventory WumpusWorld::Agent::inventory() const
{
    return _inventory;
}

int WumpusWorld::Agent::score() const
{
    return _score;
}

WumpusWorld::Coordinates::Domain WumpusWorld::Agent::location() const
{
    return _chamber->location();
}

WumpusWorld::Orientation WumpusWorld::Agent::orientation() const
{
    return _configuration.orientation;
}

WumpusWorld::Orientation WumpusWorld::Agent::turn(WumpusWorld::Agent::Side const side)
{
    // If can't afford action, skip it
    if (!_prepareForActionWithCost(_configuration.turnCost))
        return orientation();
    
    // If left, decrement by 1, if right, increment by 1
    switch (side) {
        case Left:
            _configuration.orientation = orientation() != North? static_cast<WumpusWorld::Orientation>((orientation()-1) % (NorthWest+1)) : NorthWest;
            break;
        case Right:
            _configuration.orientation = orientation() != NorthWest? static_cast<WumpusWorld::Orientation>((orientation()+1) % (NorthWest+1)) : North;
            break;
            
        default:
            // Whelp.. shit... that ain't right...
            break;
    }
    
    return orientation();
}

void WumpusWorld::Agent::forward()
{
    // If can't afford action, skip it
    if (!_prepareForActionWithCost()) return;
    
    _bump = !enter(_chamber->passage(orientation()));
}

void WumpusWorld::Agent::shoot()
{
    // Deduct 10 points, reguardless of whether the arrow hit or not,
    // but skip the actual action if it can't be afforded...
    if (!_prepareForActionWithCost(10)) return;
    
    // If you're not holding the bow, you can't shoot!
    if (!_inventory.contains(WumpusWorld::Inventory::Item::Bow)) return;
    
    
    // Reduce ammo (arrows) upon usage... (have only one by default)
    if (!_ammo) return;
    _ammo -= 1;
    
    for (WumpusWorld::Chamber* chamber = _chamber->passage(orientation()); chamber; chamber = chamber->passage(orientation())) {
        if ((_scream = chamber->hit())) break;
    }
}

void WumpusWorld::Agent::grab()
{
    // If can't afford action, skip it
    if (!_prepareForActionWithCost()) return;
    
    if (!_inventory.empty()) return; // Can only hold one thing
    
    // Imply which item to pick up
    Inventory::Item item = (_dropped == WumpusWorld::Inventory::Item::Bow)? WumpusWorld::Inventory::Item::Gold : WumpusWorld::Inventory::Item::Bow;
    
    // The inventory class automatically generates "Air",
    // if the chamber has no items, and "Air" is never stored.
    _inventory.push(_chamber->inventory.pop(item));
    
    // If it couldn't pick up the target item (if empty),
    // attempt to pick up whatever's there anyway.
    if (_inventory.empty())
        _inventory.push(_chamber->inventory.pop());
}

void WumpusWorld::Agent::drop()
{
    // If can't afford action, skip it
    if (!_prepareForActionWithCost()) return;

    // Drop an item from agent's inventory to the chamber's inventory.
    // NOTICE: "Air" is returned if inventory's empty, and it's not stored.
    _dropped = _inventory.pop();
    _chamber->inventory.push(_dropped);
}

void WumpusWorld::Agent::climb()
{
    // If can't afford action, skip it
    if (!_prepareForActionWithCost()) return;
    
    // Check that the agent can exit the cave
    if (_chamber->contains(Chamber::Feature::Exit)) {
        // If the agent is holding the gold, add 1000 points
        _addPoints(_inventory.contains(Inventory::Item::Gold)? 1000 : 0);
        _finished = true;
    }
}

void WumpusWorld::Agent::orient(Orientation const orientation)
{
    _prepareForActionWithCost((orientation - this->orientation()) * _configuration.turnCost);
    _configuration.orientation = orientation;
}

WumpusWorld::Chamber *WumpusWorld::Agent::chamber() const
{
    return _chamber;
}

bool WumpusWorld::Agent::inChamber() const
{
    return chamber() != nullptr; //&& !_chamber->contains(WumpusWorld::Chamber::Feature::Exit);
}

bool WumpusWorld::Agent::enter(WumpusWorld::Chamber* chamber)
{
    if (!chamber || !_isNeighbor(chamber)) return false;
    
    bool deathByWump = chamber->contains(WumpusWorld::Chamber::Feature::LivingWumpus);
    bool deathByPit  = chamber->contains(WumpusWorld::Chamber::Feature::Pit);

    if (deathByPit || deathByWump) {
        _damage(100);
        _deductPoints(1000);
    }
    
    return static_cast<bool>((_chamber = chamber));
}

bool WumpusWorld::Agent::alive() const
{
    return _health > 0;
}

bool WumpusWorld::Agent::finished() const
{
    return !alive() || !inChamber() || _finished;
}

WumpusWorld::Agent::Configuration WumpusWorld::Agent::nextMove()
{
    std::cin >> *this;
    return _configuration;
}

int WumpusWorld::Agent::_damage(int const damage)
{
    return _health += damage > 0? -damage : damage;
}

bool WumpusWorld::Agent::_isNeighbor(WumpusWorld::Chamber const *chamber) const
{
    // If it's not in a chamber, assume it is a neighbor.
    if (!inChamber()) return true;
    
    for (int orientation = WumpusWorld::Orientation::North; orientation <= WumpusWorld::Orientation::NorthWest; orientation++) {
        if (_chamber->passage(static_cast<WumpusWorld::Orientation>(orientation)) == chamber) {
            return true;
        }
    }
    
    return false;
}

int WumpusWorld::Agent::_deductPoints(int const points)
{
    return _score -= points;
}

int WumpusWorld::Agent::_addPoints(int const points)
{
    return _score += points;
}

void WumpusWorld::Agent::_clearPercepts()
{
    _scream = _bump = false;
}

bool WumpusWorld::Agent::_prepareForActionWithCost(int const tax)
{
    if (!inChamber()) return false;
    
    _clearPercepts(); // Clear past percepts
    _deductPoints(tax); // Deduct from every action taken
    
    return true;
}

WumpusWorld::Agent::Agent(WumpusWorld::Agent::Configuration const configuration):
_configuration(configuration),
_inventory(configuration.space),
_chamber(nullptr),
_dropped(Inventory::Item::Air),
_health(100),
_score(0),
_ammo(1),
_bump(false),
_scream(false),
_finished(false)
{
    // We'll start out with a bow
    _inventory.push(WumpusWorld::Inventory::Item::Bow);
}

WumpusWorld::Agent::~Agent()
{
    
}


// ================================================================
#pragma mark - WumpusWorld Implementation
// ================================================================
WumpusWorld::Chamber* WumpusWorld::passage(WumpusWorld::Chamber const* chamber, WumpusWorld::Orientation const orientation) const
{
    int position = _locate(chamber);
    int location = -1;
    
    
    switch (orientation) {
        case North:
            if (!(_edge(position) & Top))
                location = position - size();
            break;
            
        case NorthEast:
            if (!(_edge(position) & (Top | Right)))
                location = position - size() + 1;
            break;
            
        case East:
            if (!(_edge(position) & Right))
                location = position + 1;
            break;
            
        case SouthEast:
            if (!(_edge(position) & (Bottom | Right)))
                location = position + size() + 1;
            break;
            
        case South:
            if (!(_edge(position) & Bottom))
                location = position + size();
            break;
            
        case SouthWest:
            if (!(_edge(position) & (Bottom | Left)))
                location = position + size() - 1;
            break;
            
        case West:
            if (!(_edge(position) & Left))
                location = position - 1;
            break;
            
        case NorthWest:
            if (!(_edge(position) & (Top | Left)))
                location = position - size() - 1;
            break;
            
        default:
            break;
    }
    
    
    if (location < 0 || location >= std::pow(size(), 2)) return nullptr;
    
    return _chamber[location];
}

WumpusWorld::Chamber& WumpusWorld::chamber(std::vector<Chamber*>::size_type i) const
{
    return *_chamber[i];
}

WumpusWorld::Chamber* WumpusWorld::goldChamber() const
{
    for (WumpusWorld::Chamber *aChamber : _chamber) {
        if (aChamber->contains(WumpusWorld::Chamber::Percept::Glitter)) {
            return aChamber;
        }
    }
    return nullptr;
}



bool WumpusWorld::playable() const
{
    // If at least one agent can play, then keep the world turning!
    for (WumpusWorld::Agent const * const aPlayer : _agent) {
        if (!aPlayer->finished() && aPlayer->alive()) return true;
    }
    return false;
}

void WumpusWorld::addAgent(Agent * const agent)
{
    _agent.push_back(agent);
    
    // Note: The agent must be oriented BEFORE entering the world,
    // otherwise his actions will begin to count against him/her.
    agent->orient(_defaultOrientation);
    agent->enter(_chamber[_defaultChamber]);
}

int WumpusWorld::size() const
{
    return std::sqrt(_chamber.size());
}

int WumpusWorld::_locate(Chamber const *chamber) const
{
    // Find the position of the chamber (can use pointer arithmetic, but simplifiy it for now)
    for (std::vector<Chamber*>::size_type i = 0; i < _chamber.size(); i++) {
        if (_chamber[i] == chamber) return static_cast<int>(i);
    }
    
    return -1;
}

WumpusWorld::Edge WumpusWorld::_edge(int const position) const
{
    Edge edge = 0;
    
    if (position < size())
        edge = Top;
    
    if (position % size() == size() - 1)
        edge = edge | Right;
    
    if (position >= size() * size() - size())
        edge = edge | Bottom;
    
    if (!(position % size()))
        edge = edge | Left;
    
    return edge;
}

void WumpusWorld::_processRound() {
    for (std::vector<Agent*>::size_type i = 0; i < _agent.size(); i++) {
        if (!_agent[i]->finished()) _processPlayer(i);
    }
}

void WumpusWorld::_processPlayer(std::vector<Agent*>::size_type const i)
{
    _agent[i]->nextMove();
}

WumpusWorld::WumpusWorld(WumpusWorld::Configuration const configuration):
_defaultOrientation(WumpusWorld::Orientation::North),
_defaultChamber(0)
{
    // Generate enough features for each chamber in the world (size^2).
    std::vector<WumpusWorld::Chamber::Feature> feature(std::pow(configuration.size, 2));

    // Set entry and wumpus feature to corresponding chambers' feature set.
    feature[configuration.entry]    = WumpusWorld::Chamber::Feature::Exit;
    feature[configuration.wumpus]   = WumpusWorld::Chamber::Feature::LivingWumpus;

    // Set pit feature for all specified chambers, adding to existing features.
    for (WumpusWorld::Coordinates::Domain const &pit : configuration.pits)
    {
        feature[pit] = static_cast<WumpusWorld::Chamber::Feature>(feature[pit] | WumpusWorld::Chamber::Feature::Pit);
    }
    
    for (std::vector<WumpusWorld::Chamber::Feature>::size_type i = 0; i < feature.size(); i++)
    {
        _chamber.push_back(new WumpusWorld::Chamber(this, feature[i]));
    }
    
    _chamber[configuration.gold]->inventory.push(WumpusWorld::Inventory::Item::Gold);
    
//    for (WumpusWorld::Player *aPlayer : _agent) {
//        // Note: The agent must be oriented BEFORE entering the world,
//        // otherwise his actions will begin to count against him/her.
//        aPlayer->orient(configuration.orientation);
//        aPlayer->enter(_chamber[configuration.entry]);
//    }
    
    _defaultOrientation = configuration.orientation;
    _defaultChamber = configuration.entry;
}

WumpusWorld::WumpusWorld(int size)
{
    _chamber.resize((size < 4)? 4 : size);
}

WumpusWorld::~WumpusWorld()
{
    for (WumpusWorld::Chamber* aChamber : _chamber) {
        if (aChamber) delete aChamber;
    }
}
