#pragma once
#include "Game.h"
#include "IBattleshipGameAlgo.h"

static std::vector<IBattleshipGameAlgo *> _instancesVec; //Our Algo collection

class Player : public IBattleshipGameAlgo {
public:
	Board PlayersBoard;
	bool haveMoreAttacks = true;
	bool isPlayerA;


	vector<pair<int, int>> listOfAttacks;    // declares a vector of integers
	vector<pair<int, int>>::iterator nextAttack = listOfAttacks.begin();

	//destructor and constructor
	~Player();
	Player();
	
	//ibattleship to implement
	void setBoard(int player, const char** board, int numRows, int numCols) override;
	pair<int, int> attack() override;
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
	bool init(const std::string& path) override;

	//other methods
	void setSide(bool sideA);
	void printBoard();
	void setlistOfAttacks(vector<pair<int, int>> copy);
};
