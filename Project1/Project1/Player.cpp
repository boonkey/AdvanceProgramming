#include "IBattleshipGameAlgo.h"
#include "data_types.h"

class Player : public IBattleshipGameAlgo {
	Board PlayersBoard;
	
public:
	//constructor
	Player(int rows, int cols) :PlayersBoard(rows, cols) {}
	//destructor
	~Player(){}
	
	void setBoard(const char** board, int numRows, int numCols) override{
		for (int i = 0; i < numCols; ++i) {
			for (int j = 0; j < numRows; ++j) {
				PlayersBoard.set(j+1,i+1, board[i][j]);
			}
		}
	}
	//virtual std::pair<int, int> attack() = 0;
	pair<int, int> attack() override{
		return make_pair(1, 1);
	};
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) {}

	void printBoard(){ PlayersBoard.print(); }
};