#include "Player.h"

Configuration config;

//constructor
class FileReaderAlgo : public Player {
public:
	//destructor
	~FileReaderAlgo() {}

	pair<vector<pair<int, int>>, int> parseAttackFile(string filename) {
		vector<pair<int, int>> results;
		string line;
		smatch m;
		int numCounter = 0;
		ifstream myfile(filename);
		regex exp("\\s*(\\d*)\\s*,\\s*(\\d*)");
		if (myfile.is_open()) {
			while (getline(myfile, line)) {
				if (regex_search(line, m, exp)) {
					if (m.size() == 3) {
						pair<int, int> temp = make_pair(atoi(m[1].str().c_str()), atoi(m[2].str().c_str()));
						results.insert(results.end(), 1, temp);
					}
				}
			}
			myfile.close();
			return make_pair(results, 0);
		}
		else {
			cout << "Error: Unable to open file" << endl;
			perror("you suck");
			return  make_pair(results, -1);
		}
	}

	void setBoard(int player, const char** board, int numRows, int numCols) {
		Player::setBoard(player, board, numRows, numCols);
	}

	bool init(const std::string & path) {
		vector<string> files = get_all_files_names_within_folder(path);
		vector<string> attackFiles;
		for (auto file : files) {
			if (file.find(".attack", file.length() - 7) == file.length() - 7) {
				int err;
				if ((err = pathExist((path + "\\" + file))) != ERR_PATH_NOT_FOLDER)
					cout << "Error: path not here: " << err << endl;
				attackFiles.push_back((path + "\\" + file));
			}
		}
		pair<vector<pair<int, int>>, int> attacks;
		if (attackFiles.size() == 0) {
			cout << "No Attack Files found" << endl;
			return false;
		}
		else if (attackFiles.size() == 1 || isPlayerA) {
			attacks = parseAttackFile(attackFiles[0]);
			if (attacks.second) {
				cout << "Error parsing attack file" << endl;
				return false;
			}
			setlistOfAttacks(attacks.first);
			nextAttack = listOfAttacks.begin();
			return true;
		}
		attacks = parseAttackFile(attackFiles[1]);
		if (attacks.second) {
			cout << "Error parsing attack file" << endl;
			return false;
		}
		setlistOfAttacks(attacks.first);
		nextAttack = listOfAttacks.begin();
		return true;
	}
	pair<int, int> attack() {
		return Player::attack();
	}
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) {
		return;
	}

	
	void setSide(bool sideA) {
		isPlayerA = sideA;
	}

};

IBattleshipGameAlgo* GetAlgorithm() {
	_instancesVec.push_back(new FileReaderAlgo());
	return _instancesVec[_instancesVec.size() - 1];
}