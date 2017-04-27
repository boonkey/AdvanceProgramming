#include "Game.h"

Ship::Ship(char t, bool hor) :
	type(t),
	alive(true),
	horizontal(hor)
	{}

// returns true belongs to player A
bool Ship::isSideA() { return isupper(type) != 0; }

void Ship::print() {
	cout << "type: " << type << " alive: " << alive << " Side A: " << isSideA() << endl;
	for (auto p : position) {
		cout << p.first.first << " , " << p.first.second << " # status: " << p.second << endl;
	}
}

//puts a ship in place (position is set to true position on board. initialized to all parts alive
int Ship::putInPlace(vector<pair<int, int>> location) {
	for (auto p : location) {
		pair <pair<int, int>, bool> temp = make_pair(p, true); //make the pair of position and alive
		position.insert(position.end(), 1, temp);				//insert this into the position
	}
	return 0;
}

//returns true iff ship is alive (has at least one more life)
bool Ship::checkAlive() {
	for (auto p : position)
		if (p.second == true)
			return true;
	alive = false;
	return false;
}

//returns true iff attack hit the ship
bool Ship::checkAttack(pair<int, int> location) {
	for (int i = 0; i < position.size(); i++) {
		if (location.first == position[i].first.first && location.second == position[i].first.second && position[i].second == true) {
			//cout << "Ship was hit on " << location.first << "," << location.second << endl;
			position[i].second = false;
			if (checkAlive() == false) {
				//	cout << "ship is dead" << endl;
			}
			return true;
		}
	}
	return false;
}

int Ship::getShipScore() {
	print();
	if ((type == 'b') || (type == 'B')) {
		return 2;
	}
	if ((type == 'p') || (type == 'P')) {
		return 3;
	}
	if ((type == 'm') || (type == 'M')) {
		return 7;
	}
	if ((type == 'd') || (type == 'D')) {
		return 8;
	}
	return 0;
}