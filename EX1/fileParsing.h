#pragma once

#include <regex>
#include <vector>
#include <iostream>
#include <fstream>
#include "data_types.h"
using namespace std;

pair<vector<pair<int, int>>, int> parseAttackFile(string filename);