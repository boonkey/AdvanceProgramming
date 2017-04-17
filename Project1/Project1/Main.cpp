#include "main.h"
//#include "data_types.h"
#include <regex>
#include <cstdlib>



int main(int argc, char* argv[])
{
	int err;
	Board mainGameBoard(BOARD_SIZE, BOARD_SIZE);
	if (argc > 2) {
		cout << "Error: too much arguments" << endl;
		return ERR_WRONG_NUM_OF_ARGS;
	}
	if (argc == 2)
		config.workingDirectory = argv[1];
	else
		config.workingDirectory = ".";
	if ((err = initGame()))
		return err;
	cout << config.attackA << endl;
	//system("pause");
	//exit(0);

	//TODO add file read and format checks (should be in fileParsing.cpp)
	if (mainGameBoard.loadFromFile(config.pathBoard)) {
		perror("Failed to load from file");
		return -999; //failed to read file
	}
	Player A(BOARD_SIZE, BOARD_SIZE);
	Player B(BOARD_SIZE, BOARD_SIZE);
	A.setSide(true);
	
	pair<vector<pair<int, int>>, int> A_attacks = parseAttackFile(config.attackA);
	
	if( A_attacks.second == 0 )	{ //functions ended as expected
		A.setlistOfAttacks(A_attacks.first);
	}
	pair<vector<pair<int, int>>, int> B_attacks = parseAttackFile(config.attackB);
	if (B_attacks.second == 0) { //functions ended as expected
		B.setlistOfAttacks(B_attacks.first);
	}


	A.setBoard(mainGameBoard.getFullBoard(), 10, 10);
	cout << "-----[A board]-----" << endl;
	A.printBoard();
	B.setBoard(mainGameBoard.getFullBoard(), 10, 10);
	cout << "-----[B board]-----" << endl;
	B.printBoard();

	//fill rest of game logic
	//while true: ask for player attack , notify players on attack result , keep score


	//cout << "hello" << endl;

//block the terminal for view
	char* x="";
	cin >> x;
	return 0;
}

