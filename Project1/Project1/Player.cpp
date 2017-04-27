#include "Player.h"


class Player : public IBattleshipGameAlgo {
	Board PlayersBoard;
	//for ex1
	bool haveMoreAttacks = true;
	bool isPlayerA;

public:
	vector<pair<int, int>> listOfAttacks;    // declares a vector of integers
	vector<pair<int, int>>::iterator nextAttack = listOfAttacks.begin();
	//constructor
	Player(int rows, int cols,bool side) :PlayersBoard(rows, cols) {
		int err;
		if (side)
			make_pair(listOfAttacks, err) = parseAttackFile(config.attackA);
		else 
			make_pair(listOfAttacks, err) = parseAttackFile(config.attackB);

	}
	//destructor
	~Player(){}
	
	pair<vector<pair<int, int>>, int> parseAttackFile(string filename) {
		vector<pair<int, int>> results;
		string line;
		smatch m;
		int numCounter = 0;
		//cout << "loading file: " << filename << endl;
		ifstream myfile(filename);
		regex exp("\\s*(\\d*)\\s*,\\s*(\\d*)");
		if (myfile.is_open()) {
			while (getline(myfile, line)) {
				if (regex_search(line, m, exp)) {
					if (m.size() == 3) {
						pair<int, int> temp = make_pair(atoi(m[1].str().c_str()), atoi(m[2].str().c_str()));
						results.insert(results.end(), 1, temp);
					}
				}
			}
			myfile.close();
			//cout << "File loaded Successfully " << filename << endl;
			//	cout << "results size: " << results.size() << endl;
			return make_pair(results, 0);
		}
		else {
			cout << "Error: Unable to open file" << endl;
			return  make_pair(results, -1);
		}
	}


	void setBoard(const char** board, int numRows, int numCols) override{
		for (int i = 0; i < numCols; ++i) {
			for (int j = 0; j < numRows; ++j) {
				PlayersBoard.set(j + 1, i + 1, board[i][j]);
			}
		}
	
	}

	void setSide(bool sideA) {
		isPlayerA = sideA;
	}

	//return next attack in the vector and advance the iterator
	pair<int, int> attack() override{
		if (haveMoreAttacks) {
			if (nextAttack != listOfAttacks.end()) {
				pair<int, int> attack = *nextAttack;
				++nextAttack;
				return attack;
			} else {
				haveMoreAttacks = false;
				return make_pair(-1, -1);	//return last pair
			}
		} else{
			return  make_pair(-1, -1);
		}
	}

	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override {
		//on this exercise notify the player doesnt need to do anythinng. 
		return;
	}

	void printBoard(){ PlayersBoard.print(); }
	void setlistOfAttacks(vector<pair<int, int>> copy){
		listOfAttacks = copy;
	}
};

