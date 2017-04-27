#pragma once
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <utility> // for std::pair
#include <vector>
#include <Windows.h>

using namespace std;

#define BOARD_SIZE 10

//================== Error Code Definition ===========
#define ERR_WRONG_NUM_OF_ARGS  -1
#define ERR_TOO_MUCH_FILES     -2
#define ERR_PATH_DOESNT_EXIST  -3
#define ERR_PATH_NOT_FOLDER    -4
#define ERR_NUM_OF_ATTACKA     -5
#define ERR_NUM_OF_ATTACKB     -6
#define ERR_NUM_OF_BOARDS      -7
#define ERR_MISSING_ATTACKA    -8
#define ERR_MISSING_ATTACKB    -9
#define ERR_MISSING_SBOARD     -10
#define ERR_LOADING_BOARD      -11
#define ERR_BAD_BOARD          -12
#define ERR_UNREACHABLE        -13

struct Configuration{
	string workingDirectory;
	string attackA, attackB, pathBoard;
	//Board gameBoard(BOARD_SIZE,BOARD_SIZE);
};

Configuration config;
