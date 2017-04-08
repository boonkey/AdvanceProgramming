#pragma once
#include <iostream>
#include <vector>

#include <fstream>
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

	char get(int x, int y) { return board[y - 1][x - 1]; }
	const char** getFullBoard(){ return const_cast<const char **>(board) ; }
	void set(int x, int y, char c) { board[y-1][x-1] = c; }
	void print(){
		for (int i = 0; i < numOfCols; i++) {
			for (int j = 0; j < numOfRows; j++) {
				cout << board[i][j] << ' ';
			}
			cout << endl;
		}
	}
	



};




