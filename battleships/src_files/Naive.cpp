#include "Player.h"

Configuration config;

class Naive : public Player {
	int row=1, col=0;
public:
	Naive():Player() {	}
	//destructor
	~Naive() {}

	void setBoard(int player, const char** board, int numRows, int numCols) {
		Player::setBoard(player, board, numRows, numCols);
	}
	bool init(const std::string & path) override {
		return true;
	}
	//return next attack in the vector and advance the iterator
	pair<int, int> attack() {
		col++;
		if (col > BOARD_SIZE) {
			col = 1;
			row++;
		}
		while (PlayersBoard.isShipThere(make_pair(row, col)) || 
			PlayersBoard.isShipThere(make_pair(row+1, col)) ||
			PlayersBoard.isShipThere(make_pair(row-1, col)) ||
			PlayersBoard.isShipThere(make_pair(row, col+1)) || 
			PlayersBoard.isShipThere(make_pair(row, col-1))) {
			
			col++;
			if (col > BOARD_SIZE) {
				col = 1;
				row++;
			}
		}
		if (row > BOARD_SIZE)
			return make_pair(-1, -1);
		return make_pair(row, col);
	}

	void notifyOnAttackResult(int player, int row, int col, AttackResult result) {
		return;
	}

};

IBattleshipGameAlgo* GetAlgorithm() {
	_instancesVec.push_back(new Naive());
	return _instancesVec[_instancesVec.size() - 1];
}