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
std::istream &operator>>(std::istream& is, WumpusWorld::Coordinate &coordinates)
{
    is >> coordinates.x;
    is >> coordinates.y;
    return is;
}

std::ostream &operator<<(std::ostream& os, WumpusWorld::Coordinate const &coordinates)
{
    os << "(" << coordinates.x << ", " << coordinates.y << ")";
    return os;
}

int WumpusWorld::Coordinate::xDifference(WumpusWorld::Coordinate const &coordinate) const
{
    return x - coordinate.x;
}

int WumpusWorld::Coordinate::yDifference(WumpusWorld::Coordinate const &coordinate) const
{
    return y - coordinate.y;
}

float WumpusWorld::Coordinate::distanceTo(WumpusWorld::Coordinate const &coordinate) const
{
    return sqrt(std::pow(xDifference(coordinate), 2) +
                std::pow(yDifference(coordinate), 2));
}

WumpusWorld::Coordinate WumpusWorld::Coordinate::operator-(WumpusWorld::Coordinate const &coordinate) const
{
    return Coordinate(xDifference(coordinate), yDifference(coordinate));
}

WumpusWorld::Coordinate WumpusWorld::Coordinate::Dereference(int const size, int const coordinates)
{
    return WumpusWorld::Coordinate(coordinates % size, coordinates / size);
}

int WumpusWorld::Coordinate::Reference(int const size, WumpusWorld::Coordinate const coordinates)
{
    return coordinates.x + (size * coordinates.y);
}

WumpusWorld::Coordinate::Coordinate(int const x, int const y)
{
    this->x = x;
    this->y = y;
}


// ================================================================
#pragma mark - Configuration Implementation
// ================================================================
WumpusWorld::Configuration::Configuration(std::fstream &file)
{
    if (!file.good()) {
        std::cout << "Bad input file or not found..." << std::endl;
        return;
    }
    
    std::string line;
    
    
    // First line is size
    std::getline(file, line);
    std::stringstream boardSize(line);
    boardSize >> size;
    
    
    // Second is entry point
    std::getline(file, line);
    std::stringstream startInfo(line);
    int startX, startY;
    startInfo >> startX >> startY;
    entry = WumpusWorld::Coordinate::Reference(size, WumpusWorld::Coordinate(startX, startY));
    
    
    // Third is the orientation
    std::getline(file, line);
    std::stringstream orientationInfo(line);
    int orientation;
    orientationInfo >> orientation;
    this->orientation = static_cast<WumpusWorld::Orientation>(orientation);
    
    
    // Fourth is pits' location
    std::getline(file, line);
    std::stringstream pitInfo(line);
    
    while (pitInfo.good() && pitInfo.peek() != '\r') {
        int pitX, pitY;
        
        pitInfo >> pitX >> pitY;
        
        pits.push_back(WumpusWorld::Coordinate::Reference(size, WumpusWorld::Coordinate(pitX, pitY)));
    }
    
    // Fifth line is gold's location
    std::getline(file, line);
    std::stringstream goldInfo(line);
    int goldX, goldY;
    goldInfo >> goldX >> goldY;
    gold = WumpusWorld::Coordinate::Reference(size, WumpusWorld::Coordinate(goldX, goldY));
    
    // Sixth line is wumpus
    std::getline(file, line);
    std::stringstream wumpusInfo(line);
    int wumpusX, wumpusY;
    wumpusInfo >> wumpusX >> wumpusY;
    wumpus = WumpusWorld::Coordinate::Reference(size, WumpusWorld::Coordinate(wumpusX, wumpusY));
}

WumpusWorld::Configuration::~Configuration()
{
    
}


// ================================================================
#pragma mark - Inventory Implementation
// ================================================================
std::istream& operator>>(std::istream& is, WumpusWorld::Inventory &inventory)
{
    // Implement later
    return is;
}

std::ostream& operator<<(std::ostream& os, WumpusWorld::Inventory const &inventory)
{
    for (WumpusWorld::Item const item : inventory._item) {
        switch (item) {
            case WumpusWorld::Item::Air:
                os << "Air";
                break;
                
            case WumpusWorld::Item::Bow:
                os << "Bow";
                break;
                
            case WumpusWorld::Item::Gold:
                os << "Gold";
                break;
                
            default:
                break;
        }
        os << ' ';
    }
    return os;
}

bool WumpusWorld::Inventory::push(WumpusWorld::Item const item)
{
    if (item == Air || _item.size() == _capacity) return false;
    _item.push_back(item);
    return true;
}

WumpusWorld::Item WumpusWorld::Inventory::pop(WumpusWorld::Item const item)
{
    if (empty() || item == Air) return Air;
    
    std::vector<Item>::iterator aContainer = std::find(_item.begin(),_item.end(), item);
        
    if (aContainer == _item.end()) return Air;
    
    _item.erase(aContainer);
    
    return item;
}

WumpusWorld::Item WumpusWorld::Inventory::pop()
{
    if (empty()) return Air;
    
    Item output = _item.back();
    _item.pop_back();
    
    return output;
}

WumpusWorld::Item WumpusWorld::Inventory::peek(int const index) const
{
    if (index < 0 || index >= _item.size()) return WumpusWorld::Item::Air;
    
    return _item[index];
}

bool WumpusWorld::Inventory::contains(WumpusWorld::Item const item) const
{
    for (WumpusWorld::Item const anItem : _item) {
        if (anItem == item) return true;
    }
    
    return false;
}

int WumpusWorld::Inventory::size() const
{
    return static_cast<int>(_item.size());
}

int WumpusWorld::Inventory::limit() const
{
    return static_cast<int>(_item.capacity());
}

bool WumpusWorld::Inventory::empty() const
{
    return _item.empty();
}

WumpusWorld::Inventory::Inventory(int const capacity):
_capacity(capacity)
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

int WumpusWorld::Chamber::location() const
{
    return _world->_locate(this);
}

float WumpusWorld::Chamber::distanceTo(WumpusWorld::Chamber const *chamber) const
{
    WumpusWorld::Coordinate thisCoordinate(WumpusWorld::Coordinate::Dereference(_world->size(), this->location()));
    WumpusWorld::Coordinate thatCoordinate(WumpusWorld::Coordinate::Dereference(_world->size(), chamber->location()));
    return thisCoordinate.distanceTo(thatCoordinate);
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
    if (inventory.contains(WumpusWorld::Item::Gold)) percepts = static_cast<WumpusWorld::Chamber::Percept>(percepts | Glitter);
    
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

bool WumpusWorld::Chamber::hit() {
    if (!contains(LivingWumpus)) return false;
    
    _features = static_cast<WumpusWorld::Chamber::Feature>(removeFeature(LivingWumpus) | DeadWumpus);
    
    return true;
}

int WumpusWorld::Chamber::Count(WumpusWorld::Chamber::Feature features)
{
    int count = 0;
    
    for (int i = 0; i < sizeof(features); i++) {
        if (features & 1) count++;
        features = static_cast<WumpusWorld::Chamber::Feature>(features >> 1);
    }
    
    return count;
}

int WumpusWorld::Chamber::Count(WumpusWorld::Chamber::Percept percepts)
{
    int count = 0;
    
    for (int i = 0; i < sizeof(percepts); i++) {
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
WumpusWorld::Player::Configuration::Configuration(std::string const identification,
                                                  Orientation const orientation,
                                                  int const cost,
                                                  int const space,
                                                  int const turnCost,
                                                  int const location
                                                  ):
identification(identification),
orientation(orientation),
cost(cost),
space(space),
turnCost(turnCost),
location(location)
{
}


// ================================================================
#pragma mark - Player Implementation
// ================================================================
std::istream& operator>>(std::istream& is, WumpusWorld::Player &player)
{
    // If cin is being used for input, output some help text
    if (&is == &std::cin) {
        std::string const nav = "R - Turn Right | F - Forward | L - Turn Left";
        std::string const aux = "D - Drop | G - Grab | S - Shoot | C - Climb Out";
        
        std::cout << nav << std::endl;
        std::cout << aux << std::endl;
        std::cout << "Action: ";
    }
    
    // No error checking for now...
    std::string action;
    is >> action;
    
    switch (action.c_str()[0]) {
        case 'F':
            player.forward();
            break;
            
        case 'R':
            player.turn(WumpusWorld::Player::Right);
            break;

        case 'L':
            player.turn(WumpusWorld::Player::Left);
            break;
            
        case 'D':
            player.drop();
            break;
            
        case 'G':
            player.grab();
            break;
            
        case 'S':
            player.shoot();
            break;
        
        case 'C':
            player.climb();
            break;
            
        default:
            std::cout << "Received invalid operation...\nIgnoring..." << std::endl;
            break;
    }
    
    return is;
}

std::ostream& operator<<(std::ostream& os, WumpusWorld::Player const &player)
{
    char pointer[8] = {'^', '/', '>', '\\', 'v', '/', '<', '\\'};
    os << pointer[player.orientation()];
    
    std::string label[8] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
    os << " (" << label[player.orientation()] << ")";
    
    return os;
}

std::string WumpusWorld::Player::Sensory(WumpusWorld::Chamber::Percept percept)
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

std::string WumpusWorld::Player::identification() const
{
    return _configuration.identification;
}

WumpusWorld::Chamber::Percept WumpusWorld::Player::sense() const
{
    WumpusWorld::Chamber::Percept bump = _bump? WumpusWorld::Chamber::Bump : WumpusWorld::Chamber::Nothing;
    WumpusWorld::Chamber::Percept scream = _scream? WumpusWorld::Chamber::Scream : WumpusWorld::Chamber::Nothing;
    
    return static_cast<WumpusWorld::Chamber::Percept>((inChamber()? _chamber->environment() : 0) | bump | scream);
}

WumpusWorld::Inventory WumpusWorld::Player::inventory() const
{
    return _inventory;
}

int WumpusWorld::Player::score() const
{
    return _score;
}

int WumpusWorld::Player::location() const
{
    return _chamber->location();
}

WumpusWorld::Orientation WumpusWorld::Player::orientation() const
{
    return _configuration.orientation;
}

WumpusWorld::Orientation WumpusWorld::Player::turn(WumpusWorld::Player::Side const side)
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

void WumpusWorld::Player::forward()
{
    // If can't afford action, skip it
    if (!_prepareForActionWithCost()) return;
    
    _bump = !enter(_chamber->passage(orientation()));
}

void WumpusWorld::Player::shoot()
{
    // Deduct 10 points, reguardless of whether the arrow hit or not,
    // but skip the actual action if it can't be afforded...
    if (!_prepareForActionWithCost(10)) return;
    
    // If you're not holding the bow, you can't shoot!
    if (!_inventory.contains(WumpusWorld::Item::Bow)) return;
    
    
    // Reduce ammo (arrows) upon usage... (have only one by default)
    if (!_ammo) return;
    _ammo -= 1;
    
    for (WumpusWorld::Chamber* chamber = _chamber->passage(orientation()); chamber; chamber = chamber->passage(orientation())) {
        if ((_scream = chamber->hit())) break;
    }
}

void WumpusWorld::Player::grab()
{
    // If can't afford action, skip it
    if (!_prepareForActionWithCost()) return;
    
    if (!_inventory.empty()) return; // Can only hold one thing
    
    // Imply which item to pick up
    Item item = (_dropped == WumpusWorld::Item::Bow)? WumpusWorld::Item::Gold : WumpusWorld::Item::Bow;
    
    // The inventory class automatically generates "Air",
    // if the chamber has no items, and "Air" is never stored.
    _inventory.push(_chamber->inventory.pop(item));
    
    // If it couldn't pick up the target item (if empty),
    // attempt to pick up whatever's there anyway.
    if (_inventory.empty())
        _inventory.push(_chamber->inventory.pop());
}

void WumpusWorld::Player::drop()
{
    // If can't afford action, skip it
    if (!_prepareForActionWithCost()) return;
    
    // Store the first element as the last to be droppedL
    
    _dropped = _inventory.peek(0);
    
    // The inventory class automatically generates "Air",
    // if the player has no items, and "Air" is never stored.
    _chamber->inventory.push(_inventory.pop());
}

void WumpusWorld::Player::climb()
{
    // If can't afford action, skip it
    if (!_prepareForActionWithCost()) return;
    
    // Check that the player can exit the cave
    if (_chamber->contains(Chamber::Feature::Exit)) {
        // If the player is holding the gold, add 1000 points
        _addPoints(_inventory.contains(Item::Gold)? 1000 : 0);
        _finished = true;
    }
}

void WumpusWorld::Player::orient(Orientation const orientation)
{
    _prepareForActionWithCost((orientation - this->orientation()));
    _configuration.orientation = orientation;
}

WumpusWorld::Chamber *WumpusWorld::Player::chamber() const
{
    return _chamber;
}

bool WumpusWorld::Player::inChamber() const
{
    return chamber() != nullptr; //&& !_chamber->contains(WumpusWorld::Chamber::Feature::Exit);
}

bool WumpusWorld::Player::enter(WumpusWorld::Chamber* chamber)
{
    if (!chamber || !_isNeighbor(chamber)) return false;
    
    bool deathByWump = chamber->contains(WumpusWorld::Chamber::Feature::LivingWumpus);
    bool deathByPit  = chamber->contains(WumpusWorld::Chamber::Feature::Pit);

    if (deathByPit || deathByWump) {
        _damage(100);
        _deductPoints(1000);
    }
    
    return static_cast<bool>(_chamber = chamber);
}

bool WumpusWorld::Player::alive() const
{
    return _health > 0;
}

bool WumpusWorld::Player::finished() const
{
    return !alive() || !inChamber() || _finished;
}

WumpusWorld::Player::Configuration WumpusWorld::Player::nextMove()
{
    std::cin >> *this;
    return _configuration;
}

int WumpusWorld::Player::_damage(int const damage)
{
    return _health += damage > 0? -damage : damage;
}

bool WumpusWorld::Player::_isNeighbor(WumpusWorld::Chamber const *chamber) const
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

int WumpusWorld::Player::_deductPoints(int const points)
{
    return _score -= points;
}

int WumpusWorld::Player::_addPoints(int const points)
{
    return _score += points;
}

void WumpusWorld::Player::_clearPercepts()
{
    _scream = _bump = false;
}

bool WumpusWorld::Player::_prepareForActionWithCost(int const tax)
{
    if (!inChamber()) return false;
    
    _clearPercepts(); // Clear past percepts
    _deductPoints(tax); // Deduct from every action taken
    
    return true;
}

WumpusWorld::Player::Player(WumpusWorld::Player::Configuration const configuration):
_configuration(configuration),
_inventory(configuration.space),
_finished(false),
_chamber(nullptr),
_dropped(Air),
_scream(false),
_bump(false),
_health(100),
_score(0),
_ammo(1)
{
    // We'll start out with a bow
    _inventory.push(WumpusWorld::Item::Bow);
}

WumpusWorld::Player::~Player()
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

WumpusWorld::Chamber& WumpusWorld::chamber(int i) const
{
    return *_chamber[i];
}



bool WumpusWorld::playable() const
{
    // If at least one player can play, then keep the world turning!
    for (WumpusWorld::Player const * const aPlayer : _player) {
        if (!aPlayer->finished()) return true;
    }
    return false;
}

void WumpusWorld::addPlayer(Player * const player)
{
    _player.push_back(player);
    
    // Note: The player must be oriented BEFORE entering the world,
    // otherwise his actions will begin to count against him/her.
    player->orient(_defaultOrientation);
    player->enter(_chamber[_defaultChamber]);
}

int WumpusWorld::size() const
{
    return std::sqrt(_chamber.size());
}

int WumpusWorld::_locate(Chamber const *chamber) const
{
    // Find the position of the chamber (can use pointer arithmetic, but simplifiy it for now)
    for (int i = 0; i < _chamber.size(); i++) {
        if (_chamber[i] == chamber) return i;
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
    for (int i = 0; i < _player.size(); i++) {
        if (!_player[i]->finished()) _process(_player[i]);
    }
}

void WumpusWorld::_process(WumpusWorld::Player * const player)
{
    player->nextMove();
}

WumpusWorld::WumpusWorld(WumpusWorld::Configuration const configuration):
_defaultOrientation(WumpusWorld::Orientation::North),
_defaultChamber(0)
{
    std::vector<WumpusWorld::Chamber::Feature> feature(std::pow(configuration.size, 2));
    
    feature[configuration.entry]    = WumpusWorld::Chamber::Feature::Exit;
    feature[configuration.wumpus]   = WumpusWorld::Chamber::Feature::LivingWumpus;
    
    for (int const &pit : configuration.pits) {
        feature[pit] = static_cast<WumpusWorld::Chamber::Feature>(feature[pit] | WumpusWorld::Chamber::Feature::Pit);
    }
    
    for (int i = 0; i < feature.size(); i++) {
        _chamber.push_back(new WumpusWorld::Chamber(this, feature[i]));
    }
    
    _chamber[configuration.gold]->inventory.push(WumpusWorld::Item::Gold);
    
//    for (WumpusWorld::Player *aPlayer : _player) {
//        // Note: The player must be oriented BEFORE entering the world,
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