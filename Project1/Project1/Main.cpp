#include "main.h"
//#include "data_types.h"
#include <regex>
#include <cstdlib>



int main(int argc, char* argv[])
{
	Board mainGameBoard(BOARD_SIZE, BOARD_SIZE);
	config.workingDirectory = argv[1];
	initGame();
	cout << config.attackA << endl;
	//system("pause");
	//exit(0);

	//TODO add file read and format checks (should be in fileParsing.cpp)
	if (mainGameBoard.loadFromFile(config.pathBoard)) {
		perror("Failed to load from file");
		exit(-7); //failed to read file
	}
	Player A(BOARD_SIZE, BOARD_SIZE);
	Player B(BOARD_SIZE, BOARD_SIZE);
	
	/*string pathA = argv[1];
	string pathB = argv[1];
	pathA.append("\\clean_movesA.attack-a");
	pathB.append("\\clean_movesB.attack-b");*/
	//TODO fix unable to open file - change file name and location as needed
	pair<vector<pair<int, int>>, int> A_attacks = parseAttackFile(config.attackA);
	//system("pause");
	if( A_attacks.second == 0 )	{ //functions ended as expected
		A.setlistOfAttacks(A_attacks.first);
	}
	pair<vector<pair<int, int>>, int> B_attacks = parseAttackFile(config.attackB);
	if (B_attacks.second == 0) { //functions ended as expected
		B.setlistOfAttacks(B_attacks.first);
	}


/*	examples to see the board is working
	mainGameBoard.set(1, 2, 'c');
	mainGameBoard.set(1, 1, 'x');
	mainGameBoard.set(2, 2, 'c');
	mainGameBoard.set(7, 2, 'c');
	mainGameBoard.set(9, 6, 'p');
	mainGameBoard.set(9, 9, 'x');
	mainGameBoard.print();
	*/

	//TODO change to send only A part of the board (same for B)
	A.setBoard(mainGameBoard.getFullBoard(), 10, 10);
	A.printBoard();
	B.setBoard(mainGameBoard.getFullBoard(), 10, 10);
	B.printBoard();

	//fill rest of game logic
	//while true: ask for player attack , notify players on attack result , keep score


	//cout << "hello" << endl;

//block the terminal for view
	char* x="";
	cin >> x;
	return 0;
}

