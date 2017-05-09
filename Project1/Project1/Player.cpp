#include "Player.h"


	//destructor
	Player::~Player(){}
	Player::Player(){}
	pair<vector<pair<int, int>>, int> Player::parseAttackFile(string filename) {
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

//TODO what player argument is for?
	void Player::setBoard(int player, const char** board, int numRows, int numCols) {
		isPlayerA = (player == 0);
		if (player = 0) {
			for (int i = 0; i < numCols; ++i) {
				for (int j = 0; j < numRows; ++j) {
					if (IsCharUpperA(board[i][j]))
						PlayersBoard.set(j + 1, i + 1, board[i][j]);
				}
			}
		}
		else if (player = 1) {
			for (int i = 0; i < numCols; ++i) {
				for (int j = 0; j < numRows; ++j) {
					if (IsCharLowerA(board[i][j]))
						PlayersBoard.set(j + 1, i + 1, board[i][j]);
				}
			}
		}
		Board intermediateBoard(BOARD_SIZE, BOARD_SIZE);
		intermediateBoard.copyBoard(PlayersBoard);
		for (int col = 1; col <= 10; col++) {
			for (int row = 1; row <= 10; row++) {
				switch (intermediateBoard.get(col, row)) {
				case 'b':
				case 'B':
				case 'M':
				case 'm':
				case 'p':
				case 'P':
				case 'd':
				case 'D':
					//cout << "found a ship of type " << temp << " at <" << col << "," << row << "> " << endl;
					topLeftOfShip(PlayersBoard, intermediateBoard, col, row);
					//	intermediateBoard.print();
				default:
					break;
				}
			}
		}
	}

	void Player::setSide(bool sideA) {
		isPlayerA = sideA;
	}

	//return next attack in the vector and advance the iterator
	pair<int, int> Player::attack(){
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

	void Player::notifyOnAttackResult(int player, int row, int col, AttackResult result) {
		//on this exercise notify the player doesnt need to do anythinng. 
		return;
	}

	bool Player::init(const std::string & path)
	{
		return false;
	}

	void Player::printBoard(){ PlayersBoard.print(); }
	void Player::setlistOfAttacks(vector<pair<int, int>> copy){
		listOfAttacks = copy;
	}

//TODO implement this method
