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
	bool left;
};

class ll1
{
private: 
	vector<line> table;
	void AddLine(string lexem, vector<string> guideSet, int jump, bool left);
	void UpdateLinksForLeft(vector<string> leftParts);
public:
	void BuildTable();
	void ShowTable();
};

