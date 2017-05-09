#include "Player.h"


//constructor
class FileReaderAlgo : public Player {
public:
	//destructor
		//TODO what player argument is for?
	void setSide(bool sideA) {
		isPlayerA = sideA;
	}

	void notifyOnAttackResult(int player, int row, int col, AttackResult result) {
		//on this exercise notify the player doesnt need to do anythinng. 
		return;
	}

	bool init(const std::string & path)
	{
		vector<string> files = get_all_files_names_within_folder(path);
		vector<string> attackFiles;
		for (auto file : files) {
			if (file.find(".attack", file.length() - 7) == file.length() - 7) {
				int err;
				if ((err = pathExist((path + "\\" + file))) != ERR_PATH_NOT_FOLDER)
					cout << "path not here: " << err << endl;
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

	
	//TODO implement this method
};