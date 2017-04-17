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


