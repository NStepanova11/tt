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
	vector<bool> firstContainEmpty;
	vector<string> flw;
	vector<vector<string>> follows;
	vector<vector<string>> predicts;
	vector<string> uniqueRuleHeads;
	void ReadGrammarFromFile();

	void GetUniqueRuleHeads();

	void GetFirsts();
	void FindFstInNextRule(string e, int idx, vector<bool>& fstStatus);
	void FindFlwInNextRule(string e, int idx, vector<bool>& fstStatus);

	void GetFollows();
	void GetPredicts();

public:
	void ShowGrammar();
	void GetSets();
	void ShowFirsts();
	void ShowFollows();
	void ShowPredicts();
};

