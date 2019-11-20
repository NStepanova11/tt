#include "FirstFollowHandler.h"

void FirstFollowHandler::ReadGrammarFromFile()
{
	ifstream fin("g.txt");
	string line;
	stringstream ss;
	int lineNum = 0;

	while (getline(fin, line))
	{
		lineNum++; //индекс прочитанной строки

		ss << line;
		vector<string> ruleBody = {};
		string lexem = "";

		string leftNonTerminal;
		ss >> leftNonTerminal;
		if (lineNum == 1)
			startNoTerminal = leftNonTerminal;

		leftParts.push_back(leftNonTerminal);
		while (ss >> lexem)
		{
			if (lexem == "->")
				continue;
			else if (lexem == "|")
			{
				leftParts.push_back(leftNonTerminal);
				rightParts.push_back(ruleBody);
				ruleBody.clear();
			}
			else
				ruleBody.push_back(lexem);
		}
		rightParts.push_back(ruleBody); // тк. последнее правило формируется но после последней лексемы выходит из цикла
		ss.clear();
	}

	cout << "size of left: " << leftParts.size() << "  size of right: " << rightParts.size() << endl;
}

void FirstFollowHandler::ShowGrammar()
{
	cout << "\n----- RULES -----" << endl;
	for (int i=0; i<leftParts.size(); i++)
	{
		cout << leftParts[i] << " -> ";
		int n = 0;
		for (auto lexem : rightParts[i])
		{
			cout << " "<<lexem;
		}
		cout << endl;
	}

	//список уникальных левых частей
	cout << "unique heads: ";
	for (auto head : leftParts)
	{
		if (find(uniqueRuleHeads.begin(), uniqueRuleHeads.end(), head) == uniqueRuleHeads.end())
		{
			uniqueRuleHeads.push_back(head);
			cout << head << " ";
		}
	}
	cout << endl;

	vector<bool> fstStatus;
	for (size_t i = 0; i < uniqueRuleHeads.size(); i++)
	{
		fstStatus.push_back(true);
	}


	
	//первоначальный список first
	for (size_t i = 0; i < uniqueRuleHeads.size(); i++)
	{
		//cout << "fst " << uniqueRuleHeads[i] << ": ";
		for (size_t j = 0; j < leftParts.size(); j++)
		{
			if (leftParts[j] == uniqueRuleHeads[i])
			{
				fst.push_back(rightParts[j][0]);
				//cout << rightParts[j][0] << " ";
				if (find(leftParts.begin(), leftParts.end(), rightParts[j][0]) != leftParts.end())
				{
					fstStatus[i] = false;
				}
			}
		}
		firsts.push_back(fst);
		fst.clear();
		//cout << endl;
	}

	cout << "fst status: ";
	for (auto st : fstStatus)
	{
	cout << st << " ";
	}
	cout << endl;

	for (size_t idx = 0; idx < fstStatus.size(); idx++)
	{
		if (fstStatus[idx] == false)
		{
			for (auto e : firsts.at(idx))
			{
				if (find(leftParts.begin(), leftParts.end(), e) != leftParts.end())
				{
					//cout << "From " << uniqueRuleHeads[idx] << " go to " << e;
					FindFstInNextRule(e, idx, fstStatus);
					if (fstStatus[idx] == true)
						break;
				}
			}
		}
	}

	ShowFirsts();
}

void FirstFollowHandler::ShowFirsts()
{
	cout << "-----Firsts-----" << endl;
	for (size_t i = 0; i < uniqueRuleHeads.size(); i++)
	{
		cout << uniqueRuleHeads[i]<<" = ";
		for (size_t j = 0; j < firsts[i].size(); j++)
		{
			cout << " " << firsts[i][j];
		}
		cout << endl;
	}
}

void FirstFollowHandler::FindFstInNextRule(string e, int idx, vector<bool>& fstStatus)
{
	for (size_t i = 0; i < uniqueRuleHeads.size(); i++)
	{
		if (uniqueRuleHeads[i] == e)
		{
			if (fstStatus[i] == true)
			{
				//cout << " = ";
				for (auto fst : firsts[i])
				{
					firsts[idx].push_back(fst);
					//cout << fst << " ";
				}
				fstStatus.at(idx) = true;
				//cout << endl;
				break;
			}
			else
			{
				//cout << " -> " << firsts[i][0];
				FindFstInNextRule(firsts[i][0], idx, fstStatus);
			}
		}
	}
}
