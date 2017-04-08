#include "main.h"
#include <regex>

int main()
{
	Board mainGameBoard(10, 10);
	//TODO add file read and format checks (should be in fileParsing.cpp)
	Player A(10, 10);
	Player B(10, 10);

	//TODO fix unable to open file - change file name and location as needed
	pair<vector<pair<int, int>>, int> A_attacks = parseAttackFile(".\clean_movesA.attack-a");
	if( A_attacks.second == 0 )	{ //functions ended as expected
		A.setlistOfAttacks(A_attacks.first);
	}
	pair<vector<pair<int, int>>, int> B_attacks = parseAttackFile(".\clean_movesB.attack-b");
	if (A_attacks.second == 0) { //functions ended as expected
		A.setlistOfAttacks(A_attacks.first);
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
	//B.setBoard(mainGameBoard.getFullBoard(), 10, 10);

	//fill rest of game logic
	//while true: ask for player attack , notify players on attack result , keep score


	//cout << "hello" << endl;

//block the terminal for view
	char* x="";
	cin >> x;

	return 0;
}

