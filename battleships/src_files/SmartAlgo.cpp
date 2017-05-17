#include "Player.h"

Configuration config;

//probabilty is 1/chance
#define CONFUSE_CHANCE    4				
#define LOW_PRIOR_CHANCE  20			

class SmartAlgo : public Player {
	
	bool targetLocked;						//can kill
	bool enemyFools;						//caught enemy bluffing
	vector<pair<int, int>> normProb;		//all the moves I can do
	vector<pair<int, int>> lowProb;			//moves with low priority
	vector<pair<int, int>> highProb;		//target locked moves list
	vector<pair<int, int>> conf;		//list of bad moves - attacked places
	pair<int, int> target,lastTarget;
	int misses;
	
	enum attackList {
		HIGH = 0,
		NORM = 1,
		LOW  = 2,
		CONF = 3
	};

	attackList list;

public:
	SmartAlgo() : Player() {}
	~SmartAlgo() {}

	void setBoard(int player, const char** board, int numRows, int numCols) {
		Player::setBoard(player, board, numRows, numCols);
		int count = 0;
		for (int i = 1; i <= numRows; i++) {
			for (int j = 1; j <= numCols; j++) {
				//Option 1: this space has my ship -> skip
				if (PlayersBoard.isShipThere(make_pair(i, j))) {
					count++;
					continue;
				}

				//Option 2: this space is ajacent to my ship -> add to other moves
				if (PlayersBoard.isShipThere(make_pair(i + 1, j)) ||
					PlayersBoard.isShipThere(make_pair(i - 1, j)) ||
					PlayersBoard.isShipThere(make_pair(i, j + 1)) ||
					PlayersBoard.isShipThere(make_pair(i, j - 1)))
					conf.push_back(make_pair(i, j));

				//Option 3: all by myself
				else
					normProb.push_back(make_pair(i, j));
			}
		}
	}

	bool init(const std::string & path) override {
		targetLocked = false;
		enemyFools = false;
		target = make_pair(-1, -1);
		lastTarget = make_pair(-1, -1);
		misses = 0;
		return true;
	}

	pair<int, int> attack() {
		int att;
		srand(time(NULL));
		if (highProb.size()) {
			target = highProb[0];
			highProb.erase(highProb.begin());
			list = HIGH;
			return target;
		}

		if (!enemyFools && lowProb.size() && rand() % LOW_PRIOR_CHANCE == 0) {
			att = rand() % lowProb.size();
			target = lowProb[att];
			lowProb.erase(lowProb.begin() + att);
			list = LOW;
			return target;
		}
		
		if (misses < 3 && conf.size() && (rand() % CONFUSE_CHANCE) == 0) {
			att = rand() % conf.size();
			target = conf[att];
			conf.erase(conf.begin() + att);
			list = CONF;
			return target;
		}
		if (!normProb.size())
			mergeLow();
		att = rand() % normProb.size();
		target = normProb[att];
		normProb.erase(normProb.begin() + att);
		list = NORM;
		return target;
	}

	void notifyOnAttackResult(int player, int row, int col, AttackResult result) {
		if (isPlayerA == (player == 0)) { // respond to my own attacks
			if (result == AttackResult::Miss)
				misses++;

			if (result == AttackResult::Sink) {
				if (list == LOW)
					mergeLow();
				while (highProb.size()) {
					if (highProb[0] == make_pair(-1, -1)) {
						highProb.erase(highProb.begin());
						break;
					}
					highProb.erase(highProb.begin());
				}
			}

			if (result == AttackResult::Hit) {
				if (list == HIGH) {
					int i = 0;
					while (i < highProb.size() || highProb[i] == make_pair(-1, -1)) {
						if (!pairInline(target, highProb[i]))
							highProb.erase(highProb.begin() + i);
						else
							i++;
					}
					if (target.first == lastTarget.first) { //ship is horizontal
						logAttack(target.first - 1, target.second);
						logAttack(target.first + 1, target.second);
						if (removeFromNorm(row, col + 1) || removeFromLow(row, col + 1))
							highProb.insert(highProb.begin(), make_pair(row, col + 1));
						else
							removeFromConf(row, col + 1);

						if (removeFromNorm(row, col - 1) || removeFromLow(row, col - 1))
							highProb.insert(highProb.begin(), make_pair(row, col - 1));
						else
							removeFromConf(row, col - 1);
					} else if (target.second == lastTarget.second) { //ship is vertical
						logAttack(target.first, target.second + 1);
						logAttack(target.first, target.second - 1);
						if (removeFromNorm(row + 1, col) || removeFromLow(row + 1, col))
							highProb.push_back(make_pair(row + 1, col));
						else
							removeFromConf(row + 1, col);

						if (removeFromNorm(row - 1, col) || removeFromLow(row - 1, col))
							highProb.push_back(make_pair(row - 1, col));
						else
							removeFromConf(row - 1, col);
					}
					return;
				}
				if (list == LOW)
					mergeLow();
				lockOnTarget(row, col);
			}
		} else { //enemy attacked
			if (result == AttackResult::Miss) {
				if (!enemyFools)
					updateLow(row, col);
				logAttack(row, col);
				return;
			}
			if (PlayersBoard.isShipThere(make_pair(row, col))) {
				logAttack(row, col);
				return;
			}

			//enemy hit his own ship! he might bluff
			mergeLow();

			if (result == AttackResult::Hit)
				lockOnTarget(row, col);

			if (result == AttackResult::Sink)
				clearSurrounding(row, col);

		}
	}

	bool removeFromHigh(int row, int col) {
		auto it = find(highProb.begin(), highProb.end(), make_pair(row, col));
		if (it != highProb.end()) {
			highProb.erase(it);
			return true;
		}
		return false;
	}
 
	bool removeFromNorm(int row, int col) {
		auto it = find(normProb.begin(), normProb.end(), make_pair(row, col));
		if (it != normProb.end()) {
			normProb.erase(it);
			return true;
		}
		return false;
	}

	bool removeFromLow(int row, int col) {
		auto it = find(lowProb.begin(), lowProb.end(), make_pair(row, col));
		if (it != lowProb.end()) {
			lowProb.erase(it);
			return true;
		}
		return false;
	}

	bool removeFromConf(int row, int col) {
		auto it = find(conf.begin(), conf.end(), make_pair(row, col));
		if (it != conf.end()) {
			conf.erase(it);
			return true;
		}
		return false;
	}

	void lockOnTarget(int row, int col) {
		lastTarget = make_pair(row, col);
		if (highProb.size())
			highProb.push_back(make_pair(-2, -2));

		if (removeFromNorm(row + 1, col) || removeFromLow(row + 1, col)) 
			highProb.push_back(make_pair(row + 1, col));
		else
			removeFromConf(row + 1, col);

		if (removeFromNorm(row - 1, col) || removeFromLow(row - 1, col))
			highProb.push_back(make_pair(row - 1, col));
		else
			removeFromConf(row - 1, col);

		if (removeFromNorm(row, col + 1) || removeFromLow(row, col + 1))
			highProb.push_back(make_pair(row, col + 1));
		else
			removeFromConf(row, col + 1);

		if (removeFromNorm(row, col - 1) || removeFromLow(row, col - 1))
			highProb.push_back(make_pair(row, col - 1));
		else
			removeFromConf(row, col - 1);
	}

	void updateLow(int row, int col) {
		if (removeFromNorm(row + 1, col))
			lowProb.push_back(make_pair(row + 1, col));
		else
			removeFromConf(row + 1, col);

		if (removeFromNorm(row - 1, col))
			lowProb.push_back(make_pair(row - 1, col));
		else
			removeFromConf(row - 1, col);

		if (removeFromNorm(row, col + 1))
			lowProb.push_back(make_pair(row, col + 1));
		else
			removeFromConf(row, col + 1);

		if (removeFromNorm(row, col - 1))
			lowProb.push_back(make_pair(row, col - 1));
		else
			removeFromConf(row, col - 1);
	}

	void logAttack(int row, int col) {
		if (removeFromHigh(row, col))
			return;
		if (removeFromNorm(row, col))
			return;
		if (removeFromLow(row, col))
			return;
		if (removeFromConf(row, col))
			return;
	}

	void mergeLow() {
		enemyFools = true;
		if (!lowProb.size())
			return;
		normProb.reserve(normProb.size() + lowProb.size());
		normProb.insert(normProb.end(), lowProb.begin(), lowProb.end());
		lowProb.erase(lowProb.begin(), lowProb.end());
	}

	void clearSurrounding(int row, int col) {
		logAttack(row, col);
		logAttack(row + 1, col);
		logAttack(row - 1, col);
		logAttack(row, col - 1);
		logAttack(row, col + 1);
		if (highProb.size() && highProb[0] == make_pair(-1, -1)) {
			highProb.erase(highProb.begin());
		}
	}

	bool pairInline(pair<int, int> a, pair<int, int> b) {
		return ((a.first == b.first) || (a.second == b.second));
	}

};

IBattleshipGameAlgo* GetAlgorithm() {
	_instancesVec.push_back(new SmartAlgo());
	return _instancesVec[_instancesVec.size() - 1];
}