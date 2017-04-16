#pragma once
#include "utilities.h"

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
	vector<string> files = get_all_files_names_within_folder(config.workingDirectory);
	if (files.size() != 3) {
		cout << "I only like 3 files in my folders" << endl;
		return -1;
	}
	for (auto &file : files) {
		size_t index = file.find(".attack-a");
		if (index != string::npos) {
			//load attack file for a
			config.attackA = config.workingDirectory + "\\" + file;
			cout << "I found A's attack file: " << file << endl;
		}
		index = file.find(".attack-b");
		if (index != string::npos) {
			//load attack file for b
			config.attackB = config.workingDirectory + "\\" + file;
			cout << "I found B's attack file: " << file << endl;
		}
		index = file.find(".sboard");
		if (index != string::npos) {
			//load attack file for a
			config.pathBoard = config.workingDirectory + "\\" + file;
			cout << "I found the board file: " << config.pathBoard << endl;
		}
	}



	return 0;
}


