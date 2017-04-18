#include "fileParsing.h"

pair<vector<pair<int, int>>, int> parseAttackFile(string filename) {
	vector<pair<int, int>> results;
	string line;
	smatch m;
	int numCounter = 0;
	//cout << "loading file: " << filename << endl;
	ifstream myfile(filename);
	regex exp("\\s*(\\d*)\\s*,\\s*(\\d*)");
	if (myfile.is_open()) {
		while (getline(myfile, line)) {			
			if (regex_search(line, m, exp)) {
				if (m.size() == 3) {
					pair<int,int> temp = make_pair(atoi(m[1].str().c_str()), atoi(m[2].str().c_str()));
					results.insert(results.end(),1,temp);
				}
			}
		}
		myfile.close();
		//cout << "File loaded Successfully" << endl;
		return make_pair(results, 0);
	} else {
		cout << "Error: Unable to open file" << endl;
		return  make_pair(results, -1);
	}
}




