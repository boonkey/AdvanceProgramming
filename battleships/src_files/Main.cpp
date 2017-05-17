#include "main.h"

Configuration config;
IBattleshipGameAlgo* B;
IBattleshipGameAlgo* A;
#define CHECKPOINT printf("CHECKPOINT: line %d, func %s\n",__LINE__,__func__); system("pause")

LPCSTR getDllPath(bool isA) {
	HANDLE dir;
	WIN32_FIND_DATAA fileData; //data struct for file
	vector<HINSTANCE>::iterator itr;

	string path(config.workingDirectory); //Here you should take actual folder

	string s = "\\*.dll"; // only .dll endings
	dir = FindFirstFileA((path + s).c_str(), &fileData); // Notice: Unicode compatible version of FindFirstFile
	if (dir != INVALID_HANDLE_VALUE) //check if the dir opened successfully
	{
		if (!isA) {
			if (!FindNextFileA(dir, &fileData)) { // Notice: Unicode compatible version of FindNextFile
				cout << "Error, ran out of dll files" << endl;
				return NULL;
			}
		}
		// test each file suffix and set variables as needed
		LPCSTR fileName(fileData.cFileName);
		string fullFileName = path + "\\" + fileName;	// THIS IS THE DLL FILE PATH
			//string shapeName = fileName.substr(0, fileName.find("."));
		return fullFileName.c_str();
	} else {
		return NULL;
	}
}


IBattleshipGameAlgo* loadAndInitAlgo(bool isA, Board& mainGameBoard) {
	// Load dynamic library
	LPCSTR dllPath = getDllPath(isA);
	if (!dllPath) {
		cout << "Missing an algorithm (dll) file looking in path: " << config.workingDirectory << endl;
		return NULL;
	}
	HINSTANCE dllLib = LoadLibraryA(dllPath);
	if (!dllLib) {
		cout << "could not load the dynamic library: " << dllPath << "]" << endl;
		return NULL;
	}
	auto loadedAlgo = (GetAlgo)(GetProcAddress(dllLib, "GetAlgorithm"));
	if (!loadedAlgo) {
		cout << "Cannot load dll: " << dllPath << endl;
		return NULL;	
	}
	IBattleshipGameAlgo* loadedPlayer = loadedAlgo();
	// if we get here we can move to init the loaded algo
	int side = isA ? 0 : 1;
	loadedPlayer->setBoard(side, mainGameBoard.getSidedBoard(isA), BOARD_SIZE, BOARD_SIZE);
	if (!loadedPlayer->init(config.workingDirectory)) {
		cout << "Algorithm initialization failed for dll: " << dllPath << endl;
		return NULL;
	}
	return loadedPlayer;
}

int do_turn(Board &mainGameBoard, int isTurnB) {
	pair<int, int> thisTurnAttack;
	if (isTurnB)
		thisTurnAttack = B->attack();
	else
		thisTurnAttack = A->attack();
	if ((thisTurnAttack.first == -1) && (thisTurnAttack.second == -1)) {	//  no moves left and did not attack
		//cout << "returning -1 " << isTurnB << endl;
		system("pause");
		return -1;
	}
	else {	//attack was made -> process and notify players
		if ((thisTurnAttack.first >= 1) && (thisTurnAttack.first <= 10) && (thisTurnAttack.second >= 1) && (thisTurnAttack.second <= 10)) {	//valid attack possition
			mainGameBoard.kaboom(thisTurnAttack);
			for (int i = 0; i < mainGameBoard.ships.size(); i++) {	// go over all ships, check if one was hit
				Ship &ship = mainGameBoard.ships[i];
				if (ship.checkAttack(thisTurnAttack)) {	//if hit was made
					if (ship.checkAlive() == false) { //if ship sunk
													  //let the players know a ship was sunk
						A->notifyOnAttackResult(isTurnB, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Sink);
						B->notifyOnAttackResult(isTurnB, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Sink);
						if (ship.isSideA() == false) {
							config.scoreA += ship.getShipScore();
							if (!isTurnB)
								return 0;	// A gets another go
							return 1;
						}
						else {
							config.scoreB += ship.getShipScore();
							if (isTurnB)
								return 1;	// B gets another go
							return 0;
						}
					}
					else { //hit made but ship did not sank
						   //let the players know A hit a ship
						A->notifyOnAttackResult(isTurnB, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Hit);
						B->notifyOnAttackResult(isTurnB, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Hit);
						if (ship.isSideA() == false) {
							if (!isTurnB)
								return 0;	// A gets another go
							return 1;
						}
						else {
							if (isTurnB)
								return 1;	// B gets another go
							return 0;
						}
					}
				}
			}
			//if we get here then no ship was hit
			//let the players know A Missed
			A->notifyOnAttackResult(0, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Miss);
			B->notifyOnAttackResult(0, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Miss);
			return 1 - isTurnB;
		}
		else {	//invalid attack
			//cout << "invalid attack: " << thisTurnAttack.first << "," << thisTurnAttack.second << endl;
			return  1 - isTurnB;
		}
	}
}

int main(int argc, char* argv[])
{
	system("cls");
	HideCursor();
	int err;
	vector<string> args;
	Board mainGameBoard(BOARD_SIZE, BOARD_SIZE);
	for (int i = 1; i < argc; i++) {
		if (string(argv[i]) == "-delay") {
			if (i + 1 < argc) {
				config.delay = atoi(argv[i + 1]);
				i++;
				continue;
			}
			else {
				cout << "Error: -delay without value" << endl;
				return -1;
			}
		}
		if (string(argv[i]) == "-quiet") {
			config.quiet = true;
			continue;
		}
		//fix that
		config.workingDirectory = argv[i];
	}
	
	if ((err = initGame())) {
		system("pause");
		return err;
	}
	SetTextColor(RED);
	if (mainGameBoard.loadFromFile(config.pathBoard)) {
		system("pause");
		return ERR_LOADING_BOARD; //failed to read file
	}
	mainGameBoard.print();
	A = loadAndInitAlgo(true , mainGameBoard);
	if (A == NULL) {
		cout << "problem loading A" << endl;
		return -999;
	}
	B = loadAndInitAlgo(false, mainGameBoard);
	if (B == NULL) {
		cout << "problem loading B" << endl;
		return -998;
	}
	
	if ((err = validateBoard(mainGameBoard))) {
		system("pause");
		return err;
	}
	mainGameBoard.print();
	
	bool stillPlayingA = true;
	bool stillPlayingB = true;

	//true for A , false for B
	int isTurnB = 0; 

	while (true) {
		int turnResult;
		//system("pause");
		//cout << "==============================" << endl;
		//cout << "A " << scoreA << " playing? " << stillPlayingA << endl;
		//cout << "B " << scoreB << " playing? " << stillPlayingB << endl;

		//check if game ended 
		if (mainGameBoard.gameOver(true) || mainGameBoard.gameOver(false)) {
			stillPlayingA = false;
			stillPlayingB = false;
		}

		if ((stillPlayingA == false) && (stillPlayingB == false)) {	//both players stoped playing (game ended or no more moves
			SetTextColor(WHITE);
			gotoxy(0, 0);
			if (config.scoreA > config.scoreB) cout << "Player A won" << endl;
			if (config.scoreA < config.scoreB) cout << "Player B won" << endl;
			cout << "Points:" << endl;
			cout << "Player A : " << config.scoreA << endl;
			cout << "Player B : " << config.scoreB << endl;
			break;
		}

		turnResult = do_turn(mainGameBoard, isTurnB);
		if (turnResult == -1 && isTurnB) {
			stillPlayingB = false;
			isTurnB = 0;
		}
		else if (turnResult == -1 && !isTurnB) {
			stillPlayingA = false;
			isTurnB = 1;
		}
		else
			isTurnB = turnResult;
	}

		

//block the terminal for view
	system("pause");
	return 0;
}

