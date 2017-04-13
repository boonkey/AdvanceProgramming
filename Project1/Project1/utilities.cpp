#include "utilities.h"
#include "data_types.h"

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



int initGame(string path) {
	//get all files
	vector<string> files = get_all_files_names_within_folder(path);
	if (files.size() != 3) {
		cout << "I only like 3 files in my folders" << endl;
		return -1;
	}
	for (auto &file : files) {
		size_t index = file.find(".attack-a");
		if (index != string::npos) {
			//load attack file for a
			cout << "I found A's attack file: " << file << endl;
		}
		index = file.find(".attack-b");
		if (index != string::npos) {
			//load attack file for b
			cout << "I found B's attack file: " << file << endl;
		}
		index = file.find(".sboard");
		if (index != string::npos) {
			//load attack file for a
			
			cout << "I found the board file: " << file << endl;
		}
	}



	return 0;
}


