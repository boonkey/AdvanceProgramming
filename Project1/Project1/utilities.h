#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include "data_types.h"
using namespace std;
extern Configuration config;
int initGame();
vector<string> get_all_files_names_within_folder(string folder);