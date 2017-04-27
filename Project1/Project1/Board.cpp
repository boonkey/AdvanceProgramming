#include "Game.h"


Board::Board(int rows, int cols) {
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
Board::~Board() {
	for (int i = 0; i < numOfRows; ++i) {
		delete[] board[i];
	}
	delete[] board;
}


char Board::get(int col, int row) {
	if (col < 1 || col > BOARD_SIZE || row < 1 || row > BOARD_SIZE)
		return 'o';
	return board[row - 1][col - 1];
}

const char** Board::getFullBoard() { return const_cast<const char **>(board); }

const char** Board::getSidedBoard(bool sideA) {
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

int Board::set(int col, int row, char c) {
	if (col < 1 || col > BOARD_SIZE || row < 1 || row > BOARD_SIZE)
		return -1;
	board[row - 1][col - 1] = c;
	return 0;
}

void Board::print() {
	cout << char(176) << char(176);
	for (int i = 0; i < numOfRows; i++)
		cout << i + 1 << char(176);
	cout << endl;
	for (int i = 0; i < numOfRows; i++) {
		cout << i + 1 << char(176);
		for (int j = 0; j < numOfCols; j++) {
			cout << board[j][i] << char(176);
		}
		cout << endl;
	}
}

void Board::copyBoard(Board &origin) {
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
				set(rows, i, line.length() >= i ? line[i - 1] : ' ');
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

void Board::addShip(Ship ship) {
	ships.insert(ships.begin(), 1, ship);
}

//IN : bool sideA - true iff checking if player A has ships
//OUT: true if player have no ships left
bool Board::gameOver(bool sideA) {
	for (auto ship : ships) {
		if ((ship.isSideA() == sideA) && ship.checkAlive())
			return false;
	}
	//for (auto ship : ships)
	//	ship.print();
	return true;
}