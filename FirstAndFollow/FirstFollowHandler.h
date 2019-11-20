#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <algorithm>

using namespace std;

class FirstFollowHandler
{
private:
	vector<string> leftParts;
	vector<vector<string>> rightParts;
	string startNoTerminal;
	vector<string> fst;
	vector<vector<string>> firsts;
	vector<string> uniqueRuleHeads;


public:
	void ReadGrammarFromFile();
	void ShowGrammar();
	void ShowFirsts();
	void FindFstInNextRule(string e, int idx, vector<bool>& fstStatus);
};

