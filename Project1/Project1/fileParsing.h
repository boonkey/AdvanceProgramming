#pragma once

#include <regex>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

pair<vector<pair<int, int>>, int> parseAttackFile(string filename);