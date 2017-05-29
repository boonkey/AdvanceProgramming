#include "Player.h"


	// Destructor
	Player::~Player(){}

	// Constructor
	Player::Player(): PlayersBoard() {}

	void Player::setBoard(int player, const char** board, int numRows, int numCols) {
		PlayersBoard.init(numRows, numCols);
		isPlayerA = (player == 0);
		if (player == 0) {
			for (int i = 0; i < numCols; ++i) {
				for (int j = 0; j < numRows; ++j) {
					if (IsCharUpperA(board[i][j]))
						PlayersBoard.set(j + 1, i + 1, board[i][j]);
					else
						PlayersBoard.set(j + 1, i + 1, ' ');
				}
				

			}
		}
		else if (player == 1) {
			for (int i = 0; i < numCols; ++i) {
				for (int j = 0; j < numRows; ++j) {
					if (IsCharLowerA(board[i][j]))
						PlayersBoard.set(j + 1, i + 1, board[i][j]);
					else
						PlayersBoard.set(j + 1, i + 1, ' ');
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
					topLeftOfShip(PlayersBoard, intermediateBoard, col, row);
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
			}
		}
		haveMoreAttacks = false;
		return  make_pair(-1, -1);
	}

	void Player::notifyOnAttackResult(int player, int row, int col, AttackResult result) {
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


