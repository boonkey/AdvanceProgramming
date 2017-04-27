#pragma once
#include "Utils.h"

class Board {
	char** board;
	int numOfRows;
	int numOfCols;
	vector<Ship> ships;
public:
	//constructor
	Board(int rows, int cols);
	~Board();
	Board(Board const &) = delete;
	void operator=(Board const &x) = delete;
	char get(int col, int row);
	const char** getFullBoard();
	const char** getSidedBoard(bool sideA);
	int set(int col, int row, char c);
	void print();
	void copyBoard(Board &origin);
	int loadFromFile(string filename);
	void addShip(Ship ship);
	bool gameOver(bool sideA);
};



class Ship {
	char type;			// ship type (letter) - indicates both size and side
	vector<pair<pair<int, int>, bool>> position; //for each position we keep <<x,y>,alive>
	bool alive;			// true iff ship is alive (not dead)
	bool horizontal;	// true iff ship is horizontal

public:
	//constructor
	Ship(char t, bool hor);
	bool isSideA();
	void print();
	int putInPlace(vector<pair<int, int>> location);
	bool checkAlive();
	bool checkAttack(pair<int, int> location);
	int getShipScore();
};
	