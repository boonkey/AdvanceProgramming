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

void findShips(Board gameBoard) {
	//define intermediate board AND result board
	Board intermediateBoard(BOARD_SIZE, BOARD_SIZE);
	for (int i = 1; i <= 10; i++) {
		for (int j = 1; j <= 10; j++) {
			switch (gameBoard.get(i, j)) {
			case 'b':
			case 'B':
			case 'M':
			case 'm':
			case 'p':
			case 'P':
			case 'd':
			case 'D':
				topLeftOfShip(gameBoard, intermediateBoard, i, j);
			default:
				break;
			}
		}
	}
	//destroy intermediate board
	intermediateBoard.~Board();
}


void topLeftOfShip(Board gameBoard,Board intermediateBoard, int i, int j) {
	bool vert;
	int shiplen;
	char cmp_val = gameBoard.get(i, j);
	switch(cmp_val) {
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
			shiplen = 3;
			break;
	case 'm':
	case 'M':
			shiplen = 4;
			break;
	}
	if ((shiplen != 1) && (gameBoard.get(i + 1, j) == cmp_val))  vert = false; 
	if (!verifyValidShape(gameBoard, intermediateBoard, cmp_val, shiplen, vert, i, j))
		gameBoard.addShip(shipScan(gameBoard, cmp_val, vert, shiplen));
}


int verifyValidShape(Board gameBoard, Board intermediateBoard, char cmp_val, int shiplen, bool vert, int i, int j) {//valid ship=0, invalid =-1 and burn it
	if (shiplen == 0)
	{
		if (intermediateBoard.get(i + 1, j) == cmp_val || intermediateBoard.get(i - (int)(vert), j) == cmp_val
			|| intermediateBoard.get(i, j - (int)(!vert)) == cmp_val || intermediateBoard.get(i, j + 1) == cmp_val)//this needs a quick fix, figure out not to check both top and left, just one
		{
			burnShip(cmp_val, i, j, intermediateBoard); // need to look for all adacent similair letters and replace with x's in intermediate-recursive is advised
			return -1;
		}
		else return 0;//success is reached, we are at len=0 and our ship fits exactly
	}
	if (shiplen >0) //still need to investigate the remaining (expected) length of the ship
	{
		if (intermediateBoard.get(i + (int)(!vert), j) == cmp_val || intermediateBoard.get(i, j + (int)vert) == cmp_val) {//this means the ship is too short, expected more of the ship, yet there is nothing
			burnShip(cmp_val, i, j, intermediateBoard); // need to look for all adacent similair letters and replace with x's in intermediate-recursive is advised
			return -1;
		}
	}

	if (vert) {//should go down
		if (cmp_val == 'o' && shiplen > 0) {//the invalid value of escaping array bounds
			burnShip(cmp_val, i, j - 1, intermediateBoard);
			return -1;
		}

		if (intermediateBoard.get(i - 1, j) == cmp_val || intermediateBoard.get(i + 1, j) == cmp_val) {
			burnShip(cmp_val, i, j, intermediateBoard);
			return (verifyValidShape(gameBoard, intermediateBoard, cmp_val, shiplen - 1, vert, i + 1, j));
		}

	}
	else {//now we should go right
		if (cmp_val == 'o' && shiplen > 0) {//the invalid value of escaping array bounds
			burnShip(cmp_val, i - 1, j, intermediateBoard);
			return -1;
		}
		if (intermediateBoard.get(i, j - 1) == cmp_val || intermediateBoard.get(i, j + 1) == cmp_val)
		{
			burnShip(cmp_val, i, j, intermediateBoard); 
			return -1;
		}
		return (verifyValidShape(gameBoard, intermediateBoard, cmp_val, shiplen - 1, vert, i + 1, j));
	}

	if (vert) {//should go down
		if (cmp_val != 'o') {//the invalid value of escaping array bounds
			if (intermediateBoard.get(i - 1, j) == cmp_val || intermediateBoard.get(i + 1, j) == cmp_val)//left and right are other ships, bcz we only go down
			{
				burnShip(cmp_val, i, j, intermediateBoard);
				return -1;
			}
			return (verifyValidShape(gameBoard, intermediateBoard, cmp_val, shiplen - 1, vert, i, j + 1));
		}
	}
}


void burnShip(char cmp_val, int i, int j, Board intermediateBoard) {//function to replace ship representation in intermediate board
	if (intermediateBoard.get(i, j) != cmp_val) return; //check all the squares containing this shp's letter
	if (intermediateBoard.set(i, j, 'x')) return;       //turn them to x's when done
	burnShip(cmp_val, i + 1, j, intermediateBoard);	//do for all ajacent squares 
	burnShip(cmp_val, i - 1, j, intermediateBoard);
	burnShip(cmp_val, i, j + 1, intermediateBoard);
	burnShip(cmp_val, i, j - 1, intermediateBoard);
	return;
}


Ship shipScan(Board gameBoard, char cmp_val, bool vert, int ship_len) {
	Ship temp('f',true);
	return temp;
}
