#include "main.h"
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
	
	if (mainGameBoard.loadFromFile(config.pathBoard)) {
		perror("Failed to load from file");
		return ERR_LOADING_BOARD; //failed to read file
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


	A.setBoard(mainGameBoard.getSidedBoard(true), BOARD_SIZE, BOARD_SIZE);
	B.setBoard(mainGameBoard.getSidedBoard(false), BOARD_SIZE, BOARD_SIZE);
	
	if (findShips(mainGameBoard)) {
		cout << "Error: board is not cool" << endl;
		return -123;
	}

	/*TODO : main board has no ships*/
	for (auto ship : mainGameBoard.ships) {	// go over all ships, check if one was hit
		ship.print();
	}

	//fill rest of game logic
	//while true: ask for player attack , notify players on attack result , keep score
	int scoreA = 0;
	int scoreB = 0;
	bool stillPlayingA = true;
	bool stillPlayingB = true;
	bool currentPlayer = true; //true for A , false for B
	while(true){
		//check if game ended 
		if (mainGameBoard.gameOver(true)) {
			cout << "Player A won" << endl;
			stillPlayingA = false;
			stillPlayingB = false;
		} else if (mainGameBoard.gameOver(false)) {
			cout << "Player B won" << endl;
			stillPlayingA = false;
			stillPlayingB = false;
		}
		if ((stillPlayingA == false) && (stillPlayingB == false)) {	//both players stoped playing (game ended or no more moves
			cout << "Player A : " << scoreA << endl;
			cout << "Player B : " << scoreB << endl;
			break;
		}
		//else -> keep playing
		if (currentPlayer) {	//side A turn
			if (stillPlayingA == false) {	//A isnt playing anymore
				currentPlayer = false; //pass the turn to B
				continue;
			}
			pair<int, int> thisTurnAttack = A.attack();
////////////////////////
			cout << "attack possition was given by A in <" << thisTurnAttack.first << ", " << thisTurnAttack.second << ">" << endl;
///////////////////////
			if ((thisTurnAttack.first == -1) && (thisTurnAttack.second == -1)) {	// A has no moves left and did not attack
				stillPlayingA = false;
				currentPlayer = false; //pass the turn to B
				continue;
			} else {	//attack was made -> process and notify players
				if ((thisTurnAttack.first >= 1) && (thisTurnAttack.first <= 10) && (thisTurnAttack.second >= 1) && (thisTurnAttack.second <= 10)) {	//valid attack possition
					for (auto ship : mainGameBoard.ships) {	// go over all ships, check if one was hit
						if (ship.checkAttack(thisTurnAttack)) {	//if hit was made
							if (ship.checkAlive() == false) { //if ship sunk
								//let the players know A sank a ship
								A.notifyOnAttackResult(0, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Sink);
								B.notifyOnAttackResult(0, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Sink);
								if (ship.isSideA() == false) {	//A sank B's ship and is rewarded
									scoreA = scoreA + ship.getShipScore();
									continue;	//next turn without changing sides
								} else { //A sank his own ship
									currentPlayer = false; //pass the turn to B
									continue;
								}
							} else { //hit made but ship did not sank
								//let the players know A hit a ship
								A.notifyOnAttackResult(0, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Hit);
								B.notifyOnAttackResult(0, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Hit);
								if (ship.isSideA() == false) {	//A Hit B's ship and is rewarded
									continue;	//next turn without changing sides
								} else { //A Hit his own ship
									currentPlayer = false; //pass the turn to B
									continue;
								}
							}
						}
					}
					//if we get here then no ship was hit
					//let the players know A Missed
					A.notifyOnAttackResult(0, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Miss);
					B.notifyOnAttackResult(0, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Miss);
					currentPlayer = false; //pass the turn to B
					continue;
				} else {	//invalid attack
					cout << "Error: Invalid attack possition was given in <" << thisTurnAttack.first << "," << thisTurnAttack.second << ">  , turn pass to next player" << endl;
					currentPlayer = false; //pass the turn to B
					continue;
				}
			}
		}else{	//side B turn
			if (stillPlayingB == false) {	//B isnt playing anymore
				currentPlayer = true; //pass the turn to A
				continue;
			}
			pair<int, int> thisTurnAttack = B.attack();
////////////////////////
			cout << "attack possition was given by A in <" << thisTurnAttack.first << ", " << thisTurnAttack.second << ">" << endl;
///////////////////////
			if ((thisTurnAttack.first == -1) && (thisTurnAttack.second == -1)) {	// B has no moves left and did not attack
				stillPlayingB = false;
				currentPlayer = true; //pass the turn to A
				continue;
			} else {	//attack was made -> process and notify players
				if ((thisTurnAttack.first >= 1) && (thisTurnAttack.first <= 10) && (thisTurnAttack.second >= 1) && (thisTurnAttack.second <= 10)) {	//valid attack possition
					for (auto ship : mainGameBoard.ships) {	// go over all ships, check if one was hit
						if (ship.checkAttack(thisTurnAttack)) {	//if hit was made
							if (ship.checkAlive() == false) { //if ship sunk
								//let the players know B sank a ship
								A.notifyOnAttackResult(1, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Sink);
								B.notifyOnAttackResult(1, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Sink);
								if (ship.isSideA() == true) {	//B sank A's ship and is rewarded
									scoreB = scoreB + ship.getShipScore();
									continue;	//next turn without changing sides
								} else { //A sank his own ship
									currentPlayer = true; //pass the turn to A
									continue;
								}
							} else { //hit made but ship did not sank
								//let the players know B hit a ship
								A.notifyOnAttackResult(1, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Hit);
								B.notifyOnAttackResult(1, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Hit);
								if (ship.isSideA() == true) {	//B Hit A's ship and is rewarded
									continue;	//next turn without changing sides
								} else { //B Hit his own ship
									currentPlayer = true; //pass the turn to A
									continue;
								}
							}
						}
					}
					//if we get here then no ship was hit
					//let the players know B Missed
					A.notifyOnAttackResult(1, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Miss);
					B.notifyOnAttackResult(1, thisTurnAttack.first, thisTurnAttack.second, AttackResult::Miss);
					currentPlayer = true; //pass the turn to A
					continue;
				} else {	//invalid attack
					cout << "Error: Invalid attack possition was given in <" << thisTurnAttack.first << "," << thisTurnAttack.second << ">  , turn pass to next player" << endl;
					currentPlayer = true; //pass the turn to A
					continue;
				}
			}
		}
	}



//block the terminal for view
	system("pause");
	return 9;
}

