//
//  AStarPlayer.cpp
//  WumpusWorld
//
//  Created by Matias Barcenas on 2/24/16.
//

#include "AStarPlayer.hpp"


WumpusWorld::Player::Configuration AStarPlayer::nextMove()
{
	// If the algorithm does not have a solution yet, get one.
	if (_solution.empty()) _resolvePath();

	if (chamber()->contains(WumpusWorld::Chamber::Percept::Glitter)) {
		drop();
		grab();
	}

	// For each chamber in the solution, step into it every move.
	WumpusWorld::Chamber * const nextChamber = _solution.back();
	_solution.pop_back(); // Remove the chamber from the solution
	enter(nextChamber);
	return _configuration;
}

bool AStarPlayer::finished() const
{
    return _inventory.contains(WumpusWorld::Inventory::Item::Gold);
}

bool AStarPlayer::Queue::Node::operator<(AStarPlayer::Queue::Node const &that) const
{
	// A node is of lesser importance if its F score is higher, since it implies
	// the node is further and harder to transverse.
	return this->fCost() < that.fCost();
}

bool AStarPlayer::Queue::Node::operator==(AStarPlayer::Queue::Node const &that) const
{
	return this->chamber == that.chamber;
}

AStarPlayer::Queue::Node const *AStarPlayer::Queue::Node::parent() const
{
	return _parent;
}

void AStarPlayer::Queue::Node::setParent(AStarPlayer::Queue::Node *node)
{
	_parent = node;
}

AStarPlayer::Queue::Node &AStarPlayer::Queue::Node::operator=(AStarPlayer::Queue::Node const &that)
{
	this->chamber = that.chamber;
	this->_destination = that._destination;
	this->_parent = that._parent;
	return *this;
}

int AStarPlayer::Queue::Node::gCost(AStarPlayer::Queue::Node const *parent) const
{
	AStarPlayer::Queue::Node const *aParent = parent? parent : this->parent();
	if (!aParent) return 0;

	// A score of 5 for moving to adjacent squares.
	int myCost = 5;

	myCost += chamber->contains(WumpusWorld::Chamber::Percept::Stench)? 25 : 0;
	myCost += chamber->contains(WumpusWorld::Chamber::Percept::Breeze)? 25 : 0;
	myCost += chamber->contains(WumpusWorld::Chamber::Feature::Pit)? 500 : 0;
	myCost += chamber->contains(WumpusWorld::Chamber::Feature::LivingWumpus)? 500 : 0;

	return aParent->gCost() + myCost;
}

int AStarPlayer::Queue::Node::hCost() const
{
	return chamber->eightDistanceTo(_destination) * 5;
}

int AStarPlayer::Queue::Node::fCost() const
{
	return gCost() + hCost();
}

AStarPlayer::Queue::Node::Node(WumpusWorld::Chamber * const chamber,
						WumpusWorld::Chamber const * const destination,
						AStarPlayer::Queue::Node const *parent):
chamber(chamber),
_destination(destination),
_parent(parent)
{}

AStarPlayer::Queue::Node::~Node()
{}


void AStarPlayer::Queue::push(AStarPlayer::Queue::Node *node)
{
	_data.push_back(node);
}

AStarPlayer::Queue::Node* AStarPlayer::Queue::pop()
{
	if (empty()) return nullptr;
	std::vector<AStarPlayer::Queue::Node*>::iterator lowest = _data.begin();
	for (std::vector<AStarPlayer::Queue::Node*>::iterator i = _data.begin(); i != _data.end(); i++) {
		if (*(*lowest) < *(*i)) continue;
		lowest = i;
	}
	AStarPlayer::Queue::Node *output = *lowest;
	_data.erase(lowest);
	return output;
}

AStarPlayer::Queue::Node* AStarPlayer::Queue::peek() const
{
	if (empty()) return nullptr;
	std::vector<AStarPlayer::Queue::Node*>::const_iterator lowest = _data.begin();
	for (std::vector<AStarPlayer::Queue::Node*>::const_iterator i = _data.begin(); i != _data.end(); i++) {
		if (*(*lowest) < *(*i)) continue;
		lowest = i;
	}
	return *lowest;
}

bool AStarPlayer::Queue::empty() const
{
	return _data.empty();
}

bool AStarPlayer::Queue::contains(Node const &node) const
{
	return seek(node) != nullptr;
}

void AStarPlayer::Queue::destroy()
{
	for (AStarPlayer::Queue::Node *aNode : _data) {
		delete aNode;
	}
	_data.clear();
}

AStarPlayer::Queue::Node* AStarPlayer::Queue::seek(Node const &node) const
{
	for (std::vector<AStarPlayer::Queue::Node*>::const_iterator i = _data.begin(); i != _data.end(); i++) {
		if (*(*i) == node) {
			return *i;
		}
	}
	return nullptr;
}


void AStarPlayer::_resolvePath()
{
	Queue queue;	// To keep the nodes we'll be checking
	Queue checked;	// To keep the nodes we've checked and not recheck.

	// We begin with the first chamber
	queue.push(new AStarPlayer::Queue::Node(chamber(), _destination));

	// As long as we've got chambers, keep testing until we arrive at the destination.
	while (!queue.empty()) {

		// If we've got the solution, stop and check it.
		if (queue.peek()->chamber == _destination) {
			break;
		}

		// Get the next best chamber passage.
		AStarPlayer::Queue::Node *aNode = queue.pop();

		// Check evey adjacent chamber, in all directions...
		for (int i=WumpusWorld::Orientation::North; i<=WumpusWorld::Orientation::NorthWest; i++) {
			WumpusWorld::Chamber *aChamber = aNode->chamber->passage(static_cast<WumpusWorld::Orientation>(i));
			if (!aChamber) continue;

			AStarPlayer::Queue::Node target(aChamber);

			// If we've checked it, we're skipping it...
			if (checked.contains(target)) continue;

			// If we've got a new chamber, create a new node for it and set the adjacent node as it's parent.
			if (!queue.contains(target)) {
				queue.push(new AStarPlayer::Queue::Node(aChamber, _destination, aNode));
				continue;
			}

			// Find the neighbor and attempt to get a better cost for it.
			AStarPlayer::Queue::Node *neighbor = queue.seek(target);
			if (neighbor->gCost() > neighbor->gCost(aNode)) {
				neighbor->setParent(aNode);
			}
		}

		checked.push(aNode);
	}

	// Construct the solution based on the parent chain.
	for (AStarPlayer::Queue::Node const *aNode = queue.peek(); aNode; aNode = aNode->parent()) {
		_solution.push_back(aNode->chamber);
	}

	// Deallocate the memory we've allocated.
	queue.destroy();
	checked.destroy();

	// Remove the first node, there's no need to waste a turn with it.
	_solution.pop_back();
}




AStarPlayer::AStarPlayer(Configuration const configuration, WumpusWorld::Chamber const *const destination):
WumpusWorld::Player(configuration),
_destination(destination)
{}

AStarPlayer::~AStarPlayer()
{}
