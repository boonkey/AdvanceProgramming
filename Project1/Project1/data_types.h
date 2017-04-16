#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cerrno>
#include <Windows.h>
#define BOARD_SIZE 10

using namespace std;

class Board{
	char** board;
	int numOfRows;
	int numOfCols;

public:
	//constructor
	Board(int rows, int cols) {
		numOfRows = rows;
		numOfCols = cols;
		board = new char*[numOfCols];
		for (int i = 0; i < numOfCols; ++i) {
			board[i] = new char[numOfRows];
		}
		//init to clear board
		for (int i = 0; i < numOfCols; ++i) {
			for (int j = 0; j < numOfRows; ++j) {
				board[i][j] = ' ';
			}
		}
	}
	//destructor
	~Board() {
		for (int i = 0; i < numOfCols; ++i) {
			delete[] board[i];
		}
		delete[] board;	
	}

	char get(int col, int row) { return board[col - 1][row - 1]; }
	const char** getFullBoard(){ return const_cast<const char **>(board) ; }
	void set(int col, int row, char c) { board[col - 1][row - 1] = c; }
	void Board::print(){
		for (int i = 0; i < numOfCols; i++) {
			for (int j = 0; j < numOfRows; j++) {
				cout << board[i][j] << ' ';
			}
			cout << endl;
		}
	}

	int Board::loadFromFile(string filename) { //loads board from file. returns 0 on success. -1 on fail
		string line;
		int rows = 1;
		char buffer[MAX_PATH];
		GetModuleFileName(NULL, buffer, MAX_PATH);
		string::size_type pos = string(buffer).find_last_of("\\/");
		cout << "Working Dir: " << string(buffer).substr(0, pos) << endl; 
		ifstream boardFile(filename);
		if (boardFile.is_open()) {
			while (getline(boardFile, line) && rows < BOARD_SIZE) {
				for (int i = 1; i <= BOARD_SIZE; i++) {
					set(i, rows, line.length() >= i ? line[i] : ' ');
				}
				rows++;
			}
			print();
			return 0;
		}
		else {
			printf("Unable to open (file is close) %s\n ", filename.c_str());
			return -1;
		}
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
