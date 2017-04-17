#pragma once
#include "IBattleshipGameAlgo.h"
#include "data_types.h"

class Player : public IBattleshipGameAlgo {
	Board PlayersBoard;
	//for ex1
	vector<pair<int, int>> listOfAttacks;    // declares a vector of integers
	vector<pair<int, int>>::iterator nextAttack = listOfAttacks.begin();
	bool haveMoreAttacks = true;
	bool isPlayerA;
public:
	//constructor
	Player(int rows, int cols) :PlayersBoard(rows, cols) {}
	//destructor
	~Player(){}
	
	void setBoard(const char** board, int numRows, int numCols) override{
		cout << "setting board for player: (A?)" << isPlayerA << endl;
		for (int i = 0; i < numCols; ++i) {
			for (int j = 0; j < numRows; ++j) {
				if (isupper(board[i][j]) != 0 && isPlayerA)
					PlayersBoard.set(j + 1, i + 1, board[i][j]);
				else if (islower(board[i][j]) != 0 && !isPlayerA)
					PlayersBoard.set(j + 1, i + 1, board[i][j]);
				else
					PlayersBoard.set(j + 1, i + 1, ' ');
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
				return *nextAttack;	//return last pair
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

