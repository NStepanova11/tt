#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include "FirstFollowHandler.h"

using namespace std;


struct line {
	string lexem;
	vector<string> guideSet;
	int jump;
};

class ll1
{
private: 
	vector<line> table;
	void AddLine(string lexem, vector<string> guideSet, int jump);

public:
	void BuildTable();
	void ShowTable();
};

