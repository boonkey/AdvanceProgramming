#include "main.h"

int main()
{
	Board mainGameBoard(10, 10);
	Player A(10, 10);

	mainGameBoard.set(1, 2, 'c');
	mainGameBoard.set(1, 1, 'x');
	mainGameBoard.set(2, 2, 'c');
	mainGameBoard.set(7, 2, 'c');
	mainGameBoard.set(9, 6, 'p');
	mainGameBoard.set(9, 9, 'x');
	mainGameBoard.print();

	A.setBoard(mainGameBoard.getFullBoard(), 10, 10);
	A.printBoard();



	cout << "hello" << endl;

//block the terminal for view
	char* x="";
	cin >> x;

	return 0;
}



