#include "fileParsing.h"

pair<vector<pair<int, int>>, int> parseAttackFile(string filename) {
	vector<pair<int, int>> results;
	string line;
	smatch m;
	ifstream myfile(filename);
	regex exp("\s*(\d*)\s*,\s*(\d*)");
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			if (regex_search(line, m, exp)) {
				for (auto x : m) cout << x << " ";
				cout << endl;
			}
		}
		myfile.close();
		return make_pair(results, 0);
	} else {
		cout << "Unable to open file" << endl;
		return  make_pair(results, -1);
	}
	
}


