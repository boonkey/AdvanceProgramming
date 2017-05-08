#pragma once
//#include "Utils.h"
#include "DataTypes.h"

//extern struct Configuration config;

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
	

class Board {
	char** board;
	int numOfRows;
	int numOfCols;
public:
	vector<Ship> ships;
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

//no need to utils.h
extern Configuration config;
int initGame();
int pathExist(string pathname);
vector<string> get_all_files_names_within_folder(string folder);
int topLeftOfShip(Board &gameBoard, Board& intermediateBoard, int i, int j);
int validateBoard(Board& gameBoard);
int verifyValidShape(Board &gameBoard, Board& intermediateBoard, char cmp_val, int shiplen, bool vert, int i, int j);
void burnShip(char cmp_val, int i, int j, Board &intermediateBoard, char temp = ' ');
Ship shipScan(char value, bool vert, pair<int, int> topLeft, int shipLen, Board& gameBoard);
int checkProximity(Board &board, int col, int row);