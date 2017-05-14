#include "Player.h"

Configuration config;

//constructor
class FileReaderAlgo : public Player {
public:
	//destructor
	FileReaderAlgo::~FileReaderAlgo() {}

	void FileReaderAlgo::setBoard(int player, const char** board, int numRows, int numCols) {
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
		if (attackFiles.size() == 0)
			return false;
		else if (attackFiles.size() == 1 || isPlayerA) {
			cout << attackFiles[0] << endl;
			attacks = parseAttackFile(attackFiles[0]);
			if (attacks.second)
				return false;
			setlistOfAttacks(attacks.first);
			nextAttack = listOfAttacks.begin();
			return true;
		}
		cout << attackFiles[1] << endl;
		attacks = parseAttackFile(attackFiles[1]);
		if (attacks.second)
			return false;
		setlistOfAttacks(attacks.first);
		nextAttack = listOfAttacks.begin();
		return true;
	}
	pair<int, int> FileReaderAlgo::attack() {
		return Player::attack();
	}
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) {
		//on this exercise notify the player doesnt need to do anythinng. 
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