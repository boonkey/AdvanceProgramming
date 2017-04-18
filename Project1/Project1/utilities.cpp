#pragma once
#include "utilities.h"

int pathExist(string pathname) {
	struct stat info;
	if (stat(pathname.c_str(), &info) != 0)
		return ERR_PATH_DOESNT_EXIST;
	else if (info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows 
		return 0;
	else
		return ERR_PATH_NOT_FOLDER;
}


vector<string> get_all_files_names_within_folder(string folder)
{
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
		size_t index = file.find(".attack-a");
		if (index != string::npos) {
			//load attack file for a
			if (!attackA) {
				config.attackA = config.workingDirectory + "\\" + file;
				attackA = true;
				cout << "I found A's attack file: " << file << endl;
			}
			else {
				cout << "Error: more than one attack-a file in path" << endl;
				return ERR_NUM_OF_ATTACKA;
			}
		}
		index = file.find(".attack-b");
		if (index != string::npos) {
			//load attack file for b
			if (!attackB) {
				config.attackB = config.workingDirectory + "\\" + file;
				attackB = true;
				cout << "I found B's attack file: " << file << endl;
			}
			else {
				cout << "Error: more than one attack-b file in path" << endl;
				return ERR_NUM_OF_ATTACKB;
			}
		}
		index = file.find(".sboard");
		if (index != string::npos) {
			//load attack file for a
			if (!gameBoard) {
				config.pathBoard = config.workingDirectory + "\\" + file;
				gameBoard = true;
				cout << "I found game board's file: " << file << endl;
			}
			else {
				cout << "Error: more than one sboard file in path" << endl;
				return ERR_NUM_OF_BOARDS;
			}
		}
	}
	int result = 0;
	if (!gameBoard) {
		cout << "Missing board file(*.sboard) looking in path : " << config.workingDirectory << endl;
		result = ERR_MISSING_SBOARD;
	}
	if (!attackA) {
		cout << "Missing attack file for player A (*.attack-a) looking in path: " << config.workingDirectory << endl;
		result = ERR_MISSING_ATTACKA;
	}
	if (!attackB) {
		cout << "Missing attack file for player B (*.attack-b) looking in path: " << config.workingDirectory << endl;
		result = ERR_MISSING_ATTACKB;
	}
	return result;
}

int findShips(Board &gameBoard) {
	//define intermediate board AND result board
	Board intermediateBoard(BOARD_SIZE, BOARD_SIZE);
	intermediateBoard.copyBoard(gameBoard);
	char temp;
	bool hasError = false;
	vector<char> letters = { 'b','B','m','M','p','P','d','D' };
	map<char, bool> warnings = { \
	{ 'b',false },
	{ 'B',false },
	{ 'm',false },
	{ 'M',false },
	{ 'p',false },
	{ 'P',false },
	{ 'd',false },
	{ 'D',false },
	};
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
				cout << "found a ship of type " << temp << " at <" << col << "," << row << "> " << endl;
				if (topLeftOfShip(gameBoard, intermediateBoard, col, row))
					warnings[temp] = true;
				//intermediateBoard.print();
			default:
				break;
			}
		}
	}
	for (auto type : letters)
		if (warnings[type]) {
			hasError = true;
			if (isupper(type))
				cout << "Wrong size or shape for ship " << type << " for player A" << endl;
			if (islower(type))
				cout << "Wrong size or shape for ship " << type << " for player B" << endl;
		}
	//destroy intermediate board
	//intermediateBoard.~Board();
	return hasError;
	//destroy intermediate board
	intermediateBoard.~Board();
}




int topLeftOfShip(Board &gameBoard, Board& intermediateBoard, int col, int row) {
	bool vert = true;
	int shiplen;
	char cmp_val = intermediateBoard.get(col, row);
	switch (cmp_val) {
	case 'b':
	case 'B':
		shiplen = 1;
		break;
	case 'p':
	case 'P':
		shiplen = 2;
		break;
	case 'd':
	case 'D':
		shiplen = 4;
		break;
	case 'm':
	case 'M':
		shiplen = 3;
		break;
	}
	cout << "char: " << intermediateBoard.get(col, row) << "next char: " << intermediateBoard.get(col + 1, row) << endl;
	if (intermediateBoard.get(col + 1 , row) == cmp_val)
		vert = false;

	if (!verifyValidShape(gameBoard, intermediateBoard, cmp_val, shiplen - 1, vert, col, row)) {  //verifyValidShape needs shiplen-1 to check
		cout << "ship is cool" << endl;
		gameBoard.addShip(shipScan(cmp_val, vert, make_pair(col, row), shiplen,intermediateBoard));
		return 0;
	}
	cout << "ship isn't cool" << endl;
	return -1;
}


int verifyValidShape(Board &gameBoard, Board& intermediateBoard, char cmp_val, int shiplen, bool vert, int col, int row) {
	//valid ship=0, invalid =-1 and burn it
	cout << "verifyValidShape: ship type: " << cmp_val << " location: <" << col << "," << row << "> shiplen: " << shiplen << " vert? " << vert << endl;
	if (shiplen == 0)
	{
		cout << "line: " << __LINE__ << endl;
		if (intermediateBoard.get(col + 1, row) == cmp_val || \
			(vert && intermediateBoard.get(col - 1, row) == cmp_val) || \
			(!vert && intermediateBoard.get(col, row - 1) == cmp_val) || \
			intermediateBoard.get(col, row + 1) == cmp_val)	//this needs a quick fix, figure out not to check both top and left, just one
		{
			cout << "line: " << __LINE__ << endl;
			burnShip(cmp_val, col, row, intermediateBoard); // need to look for all adacent similair letters and replace with x's in intermediate-recursive is advised
			return -1;
		}
		return 0;//success is reached, we are at len=0 and our ship fits exactly
	}
	if (shiplen > 0) //still need to investigate the remaining (expected) length of the ship
	{
		cout << "line: " << __LINE__ << endl;
		if ((!vert && intermediateBoard.get(col + 1, row) != cmp_val) || \
			(vert && intermediateBoard.get(col, row + 1) != cmp_val)) {//this means the ship is too short, expected more of the ship, yet there is nothing
			cout << "line: " << __LINE__ << endl;
			burnShip(cmp_val, col, row, intermediateBoard); // need to look for all adacent similair letters and replace with x's in intermediate-recursive is advised
			return -1;
		}
	}

	if (vert) {//should go down
		cout << "line: " << __LINE__ << endl;
		if (intermediateBoard.get(col, row) == 'o' && shiplen > 0) {//the invalid value of escaping array bounds
			cout << "line: " << __LINE__ << endl;
			burnShip(cmp_val, col, row - 1, intermediateBoard);
			return -1;
		}

		if (intermediateBoard.get(col - 1, row) == cmp_val || intermediateBoard.get(col + 1, row) == cmp_val) {
			cout << "line: " << __LINE__ << endl;
			burnShip(cmp_val, col, row, intermediateBoard);
			return -1;
		}
		return (verifyValidShape(gameBoard, intermediateBoard, cmp_val, shiplen - 1, vert, col, row + 1));
	}
	else {//now we should go right
		cout << "line: " << __LINE__ << endl;
		if (intermediateBoard.get(col, row) == 'o' && shiplen > 0) {//the invalid value of escaping array bounds
			cout << "line: " << __LINE__ << endl;
			burnShip(cmp_val, col - 1, row, intermediateBoard);
			return -1;
		}
		if (intermediateBoard.get(col, row - 1) == cmp_val || intermediateBoard.get(col, row + 1) == cmp_val)
		{
			cout << "line: " << __LINE__ << endl;
			intermediateBoard.print();
			burnShip(cmp_val, col, row, intermediateBoard);
			return -1;
		}
		return (verifyValidShape(gameBoard, intermediateBoard, cmp_val, shiplen - 1, vert, col + 1, row));
	}
	cout << "line: " << __LINE__ << endl;
	return -66666;
}

void burnShip(char cmp_val, int col, int row, Board& intermediateBoard,char temp) {//function to replace ship representation in intermediate board
	if (intermediateBoard.get(col, row) != cmp_val) return; //check all the squares containing this shp's letter
	if (intermediateBoard.set(col, row, temp)) return;       //turn them to x's when done
	burnShip(cmp_val, col + 1, row, intermediateBoard);	//do for all ajacent squares 
	burnShip(cmp_val, col - 1, row, intermediateBoard);
	burnShip(cmp_val, col, row + 1, intermediateBoard);
	burnShip(cmp_val, col, row - 1, intermediateBoard);
	return;
}


Ship shipScan(char value, bool vert, pair<int, int> topLeft, int shipLen, Board& gameBoard) {
	Ship temp(value, vert);
	vector<pair<int, int>> location;
	for (int i = 0; i < shipLen; i++) {
		if (vert)
			location.insert(location.begin(), 1, make_pair(topLeft.first, topLeft.second + i));
		else
			location.insert(location.begin(), 1, make_pair(topLeft.first + i, topLeft.second));
	}
	temp.putInPlace(location);
	burnShip(value, topLeft.first, topLeft.second, gameBoard);
	return temp;
}



int validateBoard(Board& gameBoard) {
	if (findShips(gameBoard)) {
		return -1;
	}
	return 0;
}