#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include "data_types.h"

using namespace std;
extern Configuration config;
int initGame();
int pathExist(string pathname);
vector<string> get_all_files_names_within_folder(string folder);
int topLeftOfShip(Board gameBoard, Board* intermediateBoard, int i, int j);
int findShips(Board gameBoard);
int validateBoard(Board gameBoard);
int verifyValidShape(Board gameBoard, Board* intermediateBoard, char cmp_val, int shiplen, bool vert, int i, int j);
void burnShip(char cmp_val, int i, int j, Board *intermediateBoard);
Ship shipScan(char value, bool vert, pair<int, int> topLeft, int shipLen);
