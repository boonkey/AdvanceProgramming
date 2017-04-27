#include "main.h"




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
	if ((err = initGame())) {
		system("pause");
		return err;
	}
	
	if (mainGameBoard.loadFromFile(config.pathBoard))  return ERR_LOADING_BOARD; //failed to read file
	
	mainGameBoard.print();

	Player A(BOARD_SIZE, BOARD_SIZE,true);
	Player B(BOARD_SIZE, BOARD_SIZE,false);
	A.setSide(true);
	
	
	//move into players
	pair<vector<pair<int, int>>, int> A_attacks = parseAttackFile(config.attackA);
	
	if( A_attacks.second == 0 )	{ //functions ended as expected
		A.setlistOfAttacks(A_attacks.first);
	}
	pair<vector<pair<int, int>>, int> B_attacks = parseAttackFile(config.attackB);
	if (B_attacks.second == 0) { //functions ended as expected
		B.setlistOfAttacks(B_attacks.first);
	}


	A.setBoard(mainGameBoard.getSidedBoard(true), BOARD_SIZE, BOARD_SIZE);
	B.setBoard(mainGameBoard.getSidedBoard(false), BOARD_SIZE, BOARD_SIZE);
	
	A.nextAttack = A.listOfAttacks.begin();
	B.nextAttack = B.listOfAttacks.begin();

	if ((err = validateBoard(mainGameBoard))) {
		//cout << "Error: board is not cool" << endl;
		system("pause");
		return err;
	}

	/*mainGameBoard.print();
	for (auto ship : mainGameBoard.ships)
		ship.print();
	*/


	//fill rest of game logic
	//while true: ask for player attack , notify players on attack result , keep score
	int scoreA = 0;
	int scoreB = 0;
	bool stillPlayingA = true;
	bool stillPlayingB = true;
	if (A.listOfAttacks.size() == 0)
		stillPlayingA = false;
	if (B.listOfAttacks.size() == 0)
		stillPlayingB = false;
	bool currentPlayer = true; //true for A , false for B
	while(true){
	NEXT_TURN:
		//system("pause");
		cout << "==============================" << endl;
		//cout << "A " << scoreA << " playing? " << stillPlayingA << endl;
		//cout << "B " << scoreB << " playing? " << stillPlayingB << endl;

		//check if game ended 
		if (mainGameBoard.gameOver(true) || mainGameBoard.gameOver(false)) {
			stillPlayingA = false;
			stillPlayingB = false;
		}

		if ((stillPlayingA == false) && (stillPlayingB == false)) {	//both players stoped playing (game ended or no more moves
			if (scoreA > scoreB) cout << "Player A won" << endl;
			if (scoreA < scoreB) cout << "Player B won" << endl;
			cout << "Points:" << endl;
			cout << "Player A : " << scoreA << endl;
			cout << "Player B : " << scoreB << endl;
			break;
		}
		//else -> keep playing
		if (currentPlayer) {	//side A turn
			if (stillPlayingA == false) {	//A isnt playing anymore
				currentPlayer = false; //pass the turn to B
				goto NEXT_TURN;
			}
			pair<int, int> thisTurnAttack = A.attack();
			cout << "PLAYER A: " << thisTurnAttack.first << "," << thisTurnAttack.second << endl;
////////////////////////
			//cout << "attack possition was given by A in <" << thisTurnAttack.first << ", " << thisTurnAttack.second << ">" << endl;
///////////////////////
			if ((thisTurnAttack.first == -1) && (thisTurnAttack.second == -1)) {	// A has no moves left and did not attack
				stillPlayingA = false;
				currentPlayer = false; //pass the turn to B
				goto NEXT_TURN;
			} else {	//attack was made -> process and notify players
				if ((thisTurnAttack.first >= 1) && (thisTurnAttack.first <= 10) && (thisTurnAttack.second >= 1) && (thisTurnAttack.second <= 10)) {	//valid attack possition
					for (int i = 0;i<mainGameBoard.ships.size();i++) {	// go over all ships, check if one was hit
						Ship &ship = mainGameBoard.ships[i];
						if (ship.checkAttack(thisTurnAttack)) {	//if hit was made
							if (ship.checkAlive() == false) { //if ship sunk
								//let the players know A sank a ship
								A.notifyOnAttackResult(0, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Sink);
								B.notifyOnAttackResult(0, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Sink);
								if (ship.isSideA() == false) {	//A sank B's ship and is rewarded
									scoreA = scoreA + ship.getShipScore();
									goto NEXT_TURN;	//next turn without changing sides
								} else { //A sank his own ship
									currentPlayer = false; //pass the turn to B
									scoreB = scoreB + ship.getShipScore();
									goto NEXT_TURN;
								}
							} else { //hit made but ship did not sank
								//let the players know A hit a ship
								A.notifyOnAttackResult(0, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Hit);
								B.notifyOnAttackResult(0, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Hit);
								//cout << "hit a ship : " << ship.isSideA() << endl;
								if (!ship.isSideA()) {	//A Hit B's ship and is rewarded
									currentPlayer = true;
									goto NEXT_TURN;	//next turn without changing sides
								} else { //A Hit his own ship
									currentPlayer = false; //pass the turn to B
									goto NEXT_TURN;
								}
							}
						}
					}
					//if we get here then no ship was hit
					//let the players know A Missed
					A.notifyOnAttackResult(0, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Miss);
					B.notifyOnAttackResult(0, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Miss);
					currentPlayer = false; //pass the turn to B
					goto NEXT_TURN;
				} else {	//invalid attack
					//cout << "Error: Invalid attack possition was given in <" << thisTurnAttack.first << "," << thisTurnAttack.second << ">  , turn pass to next player" << endl;
					//currentPlayer = false; //pass the turn to B
					goto NEXT_TURN;
				}
			}








		}else{	//side B turn
			if (stillPlayingB == false) {	//B isnt playing anymore
				currentPlayer = true; //pass the turn to A
				goto NEXT_TURN;
			}
			pair<int, int> thisTurnAttack = B.attack();
			cout <<"PLAYER B: "<< thisTurnAttack.first << "," << thisTurnAttack.second << endl;
////////////////////////
			//cout << "attack possition was given by A in <" << thisTurnAttack.first << ", " << thisTurnAttack.second << ">" << endl;
///////////////////////
			
			if ((thisTurnAttack.first == -1) && (thisTurnAttack.second == -1)) {	// B has no moves left and did not attack
				stillPlayingB = false;
				currentPlayer = true; //pass the turn to A
				goto NEXT_TURN;
			} else {	//attack was made -> process and notify players
				if ((thisTurnAttack.first >= 1) && (thisTurnAttack.first <= 10) && (thisTurnAttack.second >= 1) && (thisTurnAttack.second <= 10)) {	//valid attack possition
					for (int i = 0; i < mainGameBoard.ships.size();i++) {	// go over all ships, check if one was hit
						Ship& ship = mainGameBoard.ships[i];
						if (ship.checkAttack(thisTurnAttack)) {	//if hit was made
							if (ship.checkAlive() == false) { //if ship sunk
								//let the players know B sank a ship
								A.notifyOnAttackResult(1, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Sink);
								B.notifyOnAttackResult(1, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Sink);
								if (ship.isSideA() == true) {	//B sank A's ship and is rewarded
									scoreB = scoreB + ship.getShipScore();
									goto NEXT_TURN;	//next turn without changing sides
								} else { //A sank his own ship
									currentPlayer = true; //pass the turn to A
									scoreA = scoreA + ship.getShipScore();
									goto NEXT_TURN;
								}
							} else { //hit made but ship did not sank
								//let the players know B hit a ship
								A.notifyOnAttackResult(1, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Hit);
								B.notifyOnAttackResult(1, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Hit);
								if (ship.isSideA() == true) {	//B Hit A's ship and is rewarded
									goto NEXT_TURN;	//next turn without changing sides
								} else { //B Hit his own ship
									currentPlayer = true; //pass the turn to A
									goto NEXT_TURN;
								}
							}
						}
					}
					//if we get here then no ship was hit
					//let the players know B Missed
					A.notifyOnAttackResult(1, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Miss);
					B.notifyOnAttackResult(1, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Miss);
					currentPlayer = true; //pass the turn to A
					goto NEXT_TURN;
				} else {	//invalid attack
					//cout << "Error: Invalid attack possition was given in <" << thisTurnAttack.first << "," << thisTurnAttack.second << ">  , turn pass to next player" << endl;
					//currentPlayer = true; //pass the turn to A
					goto NEXT_TURN;
				}
			}
		}

	}

//block the terminal for view
	system("pause");
	return 0;
}

