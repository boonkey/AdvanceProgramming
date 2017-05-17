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
Board::Board()
{
	Board(BOARD_SIZE, BOARD_SIZE);
}

//destructor
Board::~Board() {
	for (int i = 0; i < numOfRows; ++i) {
		delete[] board[i];
	}
	delete[] board;
}


void Board::init(int rows, int cols) {
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
			if (IsCharUpperA(board[i][j]) != 0 && sideA)
				sidedBoard[i][j] = board[i][j];
			else if (IsCharLowerA(board[i][j]) != 0 && !sideA)
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
	if (config.quiet)
		return;
	SetTextColor(WHITE);
	gotoxy(50, 0);
	cout << char(176) << char(176);
	for (int i = 0; i < numOfRows; i++)
		cout << i + 1 << char(176);
	cout << endl;
	for (int i = 0; i < 2*numOfRows; i++) {
		if (i != 18)
			gotoxy(50, 1 + i);
		else
			gotoxy(49, i + 1);
		
		if (i % 2 == 0) {
			cout << (i + 2)/2 << char(176);
			for (int j = 0; j < numOfCols; j++) {
				if (IsCharLowerA(board[j][(i + 1) / 2])) {
					SetTextColor(AQUA);
					cout << board[j][(i + 1) / 2];
				}
				else if (IsCharUpperA(board[j][(i + 1) / 2])) {
					SetTextColor(RED);
					cout << board[j][(i + 1) / 2];
				}
				else {
					SetTextColor(BLUE);
					cout << char(247);
				}
				
				SetTextColor(WHITE);
				cout << char(176);
			}
			cout << endl;
		}
		else {
			for (int j = 0; j <= numOfCols; j++) {
				cout << char(176) << char(176);
			}
			cout << endl;
		}
	}
}

/*function checks if there is ship there
 return values:
	0 - no ship there
	1 - A's ship there, alive and well
	3 - A's ship there, dead
	2 - B's ship there, alive and well
	4 - B's ship there, dead
	*/
int Board::isShipThere(pair<int,int> location) {
	for (auto ship : ships) {
		if (ship.checkLocation(location)) {
			if (ship.isSideA() && ship.checkAlive())
				return 1;
			if (ship.isSideA() && !ship.checkAlive())
				return 3;
			if (!ship.isSideA() && ship.checkAlive())
				return 2;
			if (!ship.isSideA() && !ship.checkAlive())
				return 4;
		}
	}
	return 0;
}

void Board::kaboom(pair<int,int> location) {
	if (config.quiet)
		return;
	int ship;
	gotoxy(50 + 2 * location.second, -1 + 2 * location.first);
	SetTextColor(GREEN);
	cout << "@" << endl;
	HideCursor();
	Sleep(3*config.delay/4);
	gotoxy(50 + 2 * location.second, -1 + 2 * location.first);

	// Kill red ship
	if ((ship = isShipThere(location)) % 2 == 1 && ship > 0){
		SetTextColor(RED);
		cout << char(178);
		HideCursor();
	}

	// Kill blue ship
	else if (ship % 2 == 0 && ship > 0){
		gotoxy(50 + 2 * location.second, -1 + 2 * location.first);
		SetTextColor(AQUA);
		cout << char(178);
		HideCursor();
	}

	// Clear missed attack
	else {
		gotoxy(50 + 2 * location.second, -1 + 2 * location.first);
		SetTextColor(BLUE);
		cout << char(247);
		HideCursor();
	}
	Sleep(config.delay / 4);
}

void Board::copyBoard(Board &origin) {
	for (int i = 1; i <= numOfRows; ++i)
		for (int j = 1; j <= numOfCols; ++j)
			board[j - 1][i - 1] = origin.get(i, j);
}

//loads board from file. returns 0 on success. -1 on fail
int Board::loadFromFile(string filename) { 
	string line;
	int rows = 1;

	//check cwd
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	//cout << "Working Dir: " << string(buffer).substr(0, pos) << endl;      // PRINT CWD 
	ifstream boardFile(filename);
	if (boardFile.is_open()) {
		while (getline(boardFile, line) && rows <= BOARD_SIZE) {
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
				//set(i, rows, line.length() >= i ? line[i - 1] : ' ');
			}
			rows++;
		}
		return 0;
	}
	else {
		printf("Unable to open (file is close) %s\n ", filename.c_str());
		return -1;
	}
}

void Board::reverseboard(Board &origin) {
	for (int i = 1; i <= numOfRows; ++i)
		for (int j = 1; j <= numOfCols; ++j)
			board[i - 1][j - 1] = origin.get(i, j);
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
	return true;
}

//function returns 0 if path exists
int pathExist(string pathname) {
	struct stat info;
	if (stat(pathname.c_str(), &info) != 0)
		return ERR_PATH_DOESNT_EXIST;
	else if (info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows 
		return 0;
	else
		return ERR_PATH_NOT_FOLDER;
}

//returns vector of all files in folder
vector<string> get_all_files_names_within_folder(string folder) {
	vector<string> names;
	string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}



int initGame() {
	//get all files
	int err;

	//check that path exists
	if ((err = pathExist(config.workingDirectory)) != 0) {
		cout << "Wrong path: " << config.workingDirectory << endl;
		return err;
	}

	//get all files in folders
	vector<string> files = get_all_files_names_within_folder(config.workingDirectory);
	bool attackA = false, attackB = false, gameBoard = false;
	for (auto &file : files) {
		size_t index = file.find(".sboard");
		if (index != string::npos) {
			//load attack file for a
			if (!gameBoard) {
				config.pathBoard = config.workingDirectory + "\\" + file;
				gameBoard = true;
				return 0;
			} 
		}
	}
	if (!gameBoard) {
		cout << "Missing board file(*.sboard) looking in path : " << config.workingDirectory << endl;
		return ERR_MISSING_SBOARD;
	}
	return ERR_UNREACHABLE;
}

int validateBoard(Board &gameBoard) {
	//define intermediate board AND result board
	Board intermediateBoard(BOARD_SIZE, BOARD_SIZE);
	intermediateBoard.copyBoard(gameBoard);
	char temp;
	bool hasError = false;
	vector<char> letters = { 'B','M','P','D','b','m','p','d' }; //this order is needed for correct error printing order
	map<char, bool> warnings = { \
	{ 'B',false },
	{ 'M',false },
	{ 'P',false },
	{ 'D',false },
	{ 'b',false },
	{ 'm',false },
	{ 'p',false },
	{ 'd',false }
	};

	//Step 1: verify size and shape of ships on board
	for (int col = 1; col <= 10; col++) {
		for (int row = 1; row <= 10; row++) {
			switch ((temp = intermediateBoard.get(col, row))) {
			case 'b':
			case 'B':
			case 'M':
			case 'm':
			case 'p':
			case 'P':
			case 'd':
			case 'D':
				if (topLeftOfShip(gameBoard, intermediateBoard, col, row))
					warnings[temp] = true;
			default:
				break;
			}
		}
	}

	for (auto type : letters)
		if (warnings[type]) {
			hasError = true;
			if (IsCharUpperA(type))
				cout << "Wrong size or shape for ship " << type << " for player A" << endl;
			if (IsCharLowerA(type))
				cout << "Wrong size or shape for ship " << type << " for player B" << endl;

		}


	//Step 2: count ships
	int shipsA = 0, shipsB = 0;
	for (auto ship : gameBoard.ships) {
		if (ship.isSideA())
			shipsA++;
		else
			shipsB++;
	}

	if (shipsA > 5) {
		cout << "Too many ships for player A" << endl;
		hasError = true;
	}

	if (shipsA < 5) {
		cout << "Too few ships for player A" << endl;
		hasError = true;
	}

	if (shipsB > 5) {
		cout << "Too many ships for player B" << endl;
		hasError = true;
	}

	if (shipsB < 5) {
		cout << "Too few ships for player B" << endl;
		hasError = true;
	}


	//Step 3: check adjacent ships
	for (int col = 1; col <= 10; col++) {
		for (int row = 1; row <= 10; row++) {
			if (intermediateBoard.get(col, row) != ' ' && checkProximity(intermediateBoard, col, row)) {
				cout << "Adjacent Ships on Board" << endl;
				return ERR_BAD_BOARD;
			}
		}
	}

	if (hasError)
		return ERR_BAD_BOARD;
	return 0;
}

int checkProximity(Board &board, int col, int row) {
	// returns -1 if a surronding box contain something different than blank or same.
	char local = board.get(col, row);
	if (board.get(col + 1, row) != local &&  board.get(col + 1, row) != ' ' && board.get(col + 1, row) != 'o')
		return -1;
	if (board.get(col - 1, row) != local &&  board.get(col - 1, row) != ' ' && board.get(col - 1, row) != 'o')
		return -1;
	if (board.get(col, row + 1) != local &&  board.get(col, row + 1) != ' ' && board.get(col, row + 1) != 'o')
		return -1;
	if (board.get(col, row - 1) != local &&  board.get(col, row - 1) != ' ' && board.get(col, row - 1) != 'o')
		return -1;
	return 0;
}

void gotoxy(int x, int y) {
	COORD coord = { (short)x,(short)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetTextColor(COLOR color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void HideCursor() {
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = false;
	lpCursor.dwSize = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &lpCursor);
}



