#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cerrno>
#include <Windows.h>


#define BOARD_SIZE 10

//================== Error Code Definition ===========
#define ERR_WRONG_NUM_OF_ARGS  -1
#define ERR_TOO_MUCH_FILES     -2
#define ERR_PATH_DOESNT_EXIST  -3
#define ERR_PATH_NOT_FOLDER    -4
#define ERR_NUM_OF_ATTACKA     -5
#define ERR_NUM_OF_ATTACKB     -6
#define ERR_NUM_OF_BOARDS      -7
#define ERR_MISSING_ATTACKA    -8
#define ERR_MISSING_ATTACKB    -9
#define ERR_MISSING_SBOARD     -10
#define ERR_LOADING_BOARD      -11
#define ERR_BAD_BOARD          -12
#define ERR_UNREACHABLE        -13

using namespace std;

class Ship {
	char type;			// ship type (letter) - indicates both size and side
	vector<pair<pair<int, int>,bool>> position; //for each position we keep <<x,y>,alive>
	bool alive;			// true iff ship is alive (not dead)
	bool horizontal;	// true iff ship is horizontal


public:
	//constructor
	Ship(char t, bool hor) {
		type = t;
		alive = true;
		horizontal = hor;
	}

	//destructor
	~Ship() {}

	// returns true belongs to player A
	bool isSideA() { return isupper(type) != 0; }

	void print(){
		cout << "type: " << type << " alive: " << alive << " Side A: " << isSideA() << endl;
		for (auto p : position){
			cout << p.first.first << " , " << p.first.second << " # status: " << p.second << endl;
		}
	}

	//puts a ship in place (position is set to true position on board. initialized to all parts alive
	int putInPlace(vector<pair<int, int>> location) {
		for (auto p : location) {
			pair <pair<int, int>, bool> temp = make_pair(p, true); //make the pair of position and alive
			position.insert(position.end(), 1, temp);				//insert this into the position
		}
		return 0;
	}

	//returns true iff ship is alive (has at least one more life)
	bool checkAlive() {
		for (auto p : position)
			if (p.second == true)
				return true;
		alive = false;
		return false;
	}
	
	//returns true iff attack hit the ship
	bool checkAttack(pair<int, int> location) {
		for (auto pos : position) {
			if (location.first == pos.first.first && location.second == pos.first.second && pos.second == true) {
				cout << "Ship was hit on " << location.first << "," << location.second << endl;
				pos.second = false;
				if (checkAlive() == false)
					cout << "ship is dead" << endl;
				return true;
			}
		}
		return false;
	}

	int getShipScore(){
		if ((type == 'b') || (type == 'B')) {
			return 2;
		}
		if ((type == 'p') || (type == 'P')) {
			return 3;
		}
		if ((type == 'm') || (type == 'M')) {
			return 7;
		}
		if( (type == 'd') || (type == 'D') ){
			return 8;
		}
		return 0;
	}

};




class Board{
	char** board;
	int numOfRows;
	int numOfCols;
	
public:
	vector<Ship> ships;
	//constructor
	Board(int rows, int cols) {
		numOfRows = rows;
		numOfCols = cols;
		board = new char*[numOfRows];
		for (int i = 0; i < numOfCols; ++i) {
			board[i] = new char[numOfRows];
		}
		//init to clear board
		for (int i = 0; i < numOfRows; ++i) {
			for (int j = 0; j < numOfCols; ++j) {
				board[j][i] = ' ';
			}
		}
	}
	//destructor
	~Board() {
		for (int i = 0; i < numOfRows; ++i) {
			delete[] board[i];
		}
		delete[] board;	
	}

	char get(int col, int row) { 
		if (col < 1 || col > BOARD_SIZE || row < 1 || row > BOARD_SIZE)
			return 'o';
		return board[row - 1][col - 1]; 
	}
	const char** getFullBoard(){ return const_cast<const char **>(board) ; }

	const char** getSidedBoard(bool sideA) {
		char** sidedBoard = new char*[numOfCols];
		for (int i = 0; i < numOfCols; ++i) {
			sidedBoard[i] = new char[numOfRows];
		}
		//init to clear board
		for (int i = 0; i < numOfCols; ++i) {
			for (int j = 0; j < numOfRows; ++j) {
				if (isupper(board[i][j]) != 0 && sideA)
					sidedBoard[i][j] = board[i][j];
				else if (islower(board[i][j]) != 0 && !sideA)
					sidedBoard[i][j] = board[i][j];
				else
					sidedBoard[i][j] = ' ';
			}
		}
		return const_cast<const char **> (sidedBoard);
	}

	int set(int col, int row, char c) { 
		if (col < 1 || col > BOARD_SIZE || row < 1 || row > BOARD_SIZE)
			return -1;
		board[row - 1][col - 1] = c; 
		return 0;
	}

	void Board::print(){
		for (int i = 0; i < numOfRows; i++) {
			for (int j = 0; j < numOfCols; j++) {
				cout << board[j][i] << char(176);
			}
			cout << endl;
		}
	}

	void copyBoard(Board origin) {
		for (int i = 1; i <= numOfRows; ++i)
			for (int j = 1; j <= numOfCols; ++j)
				board[j - 1][i - 1] = origin.get(i, j);
	}

	int Board::loadFromFile(string filename) { //loads board from file. returns 0 on success. -1 on fail
		string line;
		int rows = 1;
		
		//check cwd
		char buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, MAX_PATH);
		string::size_type pos = string(buffer).find_last_of("\\/");
		//cout << "Working Dir: " << string(buffer).substr(0, pos) << endl; 

		ifstream boardFile(filename);
		if (boardFile.is_open()) {
			while (getline(boardFile, line) && rows <= BOARD_SIZE) {
				//cout << line << endl;
				for (int i = 1; i <= BOARD_SIZE; i++) {
					//check for input vaildity
					switch (line[i - 1]) {
					case 'm':
					case 'M':
					case 'D':
					case 'd':
					case 'p':
					case 'P':
					case 'b':
					case 'B':
						break;
					default:
						line[i - 1] = ' ';
						break;
					}

					//set the board
					set(rows, i, line.length() >= i ? line[i-1] : ' ');
				}
				rows++;
			}
		//	cout << "-------------------" << endl;
			//print();
			return 0;
		}
		else {
			printf("Unable to open (file is close) %s\n ", filename.c_str());
			return -1;
		}
	}
	void addShip(Ship ship) {
		ships.insert(ships.begin(), 1, ship);
	}
	
	//IN : bool sideA - true iff checking if player A has won
	//OUT: true if player has won
	bool gameOver(bool sideA) {
		for (auto ship : ships) {
			if ((ship.isSideA() == sideA) && ship.checkAlive())

				return false;
		}
		return true;
	}
};


#ifndef _CONFIGURATION
#define _CONFIGURATION


struct Configuration{
	string workingDirectory;
	string attackA, attackB, pathBoard;
	//Board gameBoard(BOARD_SIZE,BOARD_SIZE);
};

#endif // !_CONFIGURATION
