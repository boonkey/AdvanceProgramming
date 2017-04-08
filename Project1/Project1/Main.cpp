#include "main.h"
#include <regex>

int main()
{
	Board mainGameBoard(10, 10);
	//TODO add file read and format checks
	Player A(10, 10);
	//TODO fix unable to open file
	pair<vector<pair<int, int>>, int> A_attacks = parseAttackFile(".\clean_movesA.attack-a");
	if( A_attacks.second == 0 )	{ //functions ended as expected
		A.setlistOfAttacks(A_attacks.first);
	}

	mainGameBoard.set(1, 2, 'c');
	mainGameBoard.set(1, 1, 'x');
	mainGameBoard.set(2, 2, 'c');
	mainGameBoard.set(7, 2, 'c');
	mainGameBoard.set(9, 6, 'p');
	mainGameBoard.set(9, 9, 'x');
	//mainGameBoard.print();

	A.setBoard(mainGameBoard.getFullBoard(), 10, 10);
	//A.printBoard();



	//cout << "hello" << endl;

//block the terminal for view
	char* x="";
	cin >> x;

	return 0;
}

/*
pair<vector<pair<int,int>>,int> parseAttackFile(string filename){
	vector<pair<int, int>> results;
	string line;
	smatch m;
	ifstream myfile(filename);
	regex exp("\s*(\d*)\s*,\s*(\d*)");
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			if(regex_search(line, m, exp)){
				for (auto x : m) cout << x << " ";
				cout << endl;
			}
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	return make_pair(results, 0);
}


*/

