#include "FirstFollowHandler.h"

void FirstFollowHandler::ReadGrammarFromFile()
{
	ifstream fin("testGrammar.txt");
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
	ReadGrammarFromFile();
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
}

void FirstFollowHandler::GetSets()
{
	//не сделано: удалить из firsts нетерминалы
	GetFirsts();
	ShowFirsts();
	GetFollows();
	ShowFollows();
}


void FirstFollowHandler::GetUniqueRuleHeads()
{
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
}

void FirstFollowHandler::GetFirsts()
{
	GetUniqueRuleHeads();

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

	//прохождение по нетерминалам в множествах first
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

	for (size_t i = 0; i < firsts.size(); i++)
	{
		for (auto nt : uniqueRuleHeads)
		{
			while (find(firsts[i].begin(), firsts[i].end(), nt) != firsts[i].end())
			{
				auto i_nt = find(firsts[i].begin(), firsts[i].end(), nt);
				//if (i_nt != firsts[i].end())
				{
					firsts[i].erase(i_nt);
				}
			}
		}
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

void FirstFollowHandler::ShowFirsts()
{
	cout << "-----Firsts-----" << endl;
	for (size_t i = 0; i < uniqueRuleHeads.size(); i++)
	{
		cout << uniqueRuleHeads[i] << " = ";
		for (size_t j = 0; j < firsts[i].size(); j++)
		{
			//if (find(uniqueRuleHeads.begin(), uniqueRuleHeads.end(), firsts[i][j]) == uniqueRuleHeads.end())
				cout << " " << firsts[i][j];
		}
		cout << endl;
	}
}


void FirstFollowHandler::GetFollows()
{
	vector<bool> flwStatus;
	for (size_t i = 0; i < uniqueRuleHeads.size(); i++)
	{
		flwStatus.push_back(true);
	}

	for (size_t i = 0; i < uniqueRuleHeads.size(); i++)
	{
		if (i == 0)
		{
			flw.push_back("$");
		}

		for (size_t j = 0; j < rightParts.size(); j++)
		{
			if (rightParts[j].size() != 1)
			{
				for (size_t k = 1; k < rightParts[j].size(); k++)
				{
					if (rightParts[j][k] == uniqueRuleHeads[i])
					{
						if (k != rightParts[j].size() - 1)
						{
							if (find(uniqueRuleHeads.begin(), uniqueRuleHeads.end(), rightParts[j][k + 1]) == uniqueRuleHeads.end())
								flw.push_back(rightParts[j][k + 1]);
							
							else
							{
								auto it = find(uniqueRuleHeads.begin(), uniqueRuleHeads.end(), rightParts[j][k + 1]);
								auto index = distance(uniqueRuleHeads.begin(), it);
								for (size_t m = 0; m < firsts[index].size(); m++)
								{
									if (firsts[index][m] != "#")
									{
										flw.push_back(firsts[index][m]);
									}
									else
									{
										flw.push_back(leftParts[j]);
										flwStatus[i] = false;
									}
								}
							}
						}
						else
						{
							flw.push_back(leftParts[j]);
							flwStatus[i] = false;
						}
					}
				}
			}
		}
		follows.push_back(flw);
		flw.clear();

	}
	cout << "flw status: ";
	for (auto st : flwStatus)
	{
		cout << st << " ";
	}
	cout << endl;
	

	//прохождение по нетерминалам в множествах first
	for (size_t idx = 0; idx < flwStatus.size(); idx++)
	{
		if (flwStatus[idx] == false)
		{
			for (auto e : follows.at(idx))
			{
				if (find(uniqueRuleHeads.begin(), uniqueRuleHeads.end(), e) != uniqueRuleHeads.end())
				{
					//cout << "From " << uniqueRuleHeads[idx] << " go to " << e;
					FindFlwInNextRule(e, idx, flwStatus);
					if (flwStatus[idx] == true)
						break;
				}
			}
		}
	}

	for (size_t i = 0; i < follows.size(); i++)
	{
		for (auto nt : uniqueRuleHeads)
		{
			while (find(follows[i].begin(), follows[i].end(), nt) != follows[i].end())
			{
				auto i_nt = find(follows[i].begin(), follows[i].end(), nt);
				//if (i_nt != firsts[i].end())
				{
					follows[i].erase(i_nt);
				}
			}
		}
	}

}


void FirstFollowHandler::ShowFollows()
{
	cout << "-----Follows-----" << endl;
	for (size_t i = 0; i < uniqueRuleHeads.size(); i++)
	{
		cout << uniqueRuleHeads[i] << " = ";
		for (size_t j = 0; j < follows[i].size(); j++)
		{
			//if (find(uniqueRuleHeads.begin(), uniqueRuleHeads.end(), follows[i][j]) == uniqueRuleHeads.end())
				cout << " " << follows[i][j];
		}
		cout << endl;
	}
}

void FirstFollowHandler::FindFlwInNextRule(string e, int idx, vector<bool>& flwStatus)
{
	for (size_t i = 0; i < uniqueRuleHeads.size(); i++)
	{
		if (uniqueRuleHeads[i] == e)
		{
			if (flwStatus[i] == true)
			{
				//cout << " = ";
				for (auto flw : follows[i])
				{
					follows[idx].push_back(flw);
					//cout << fst << " ";
				}
				flwStatus.at(idx) = true;
				//cout << endl;
				break;
			}
			else
			{
				//cout << " -> " << firsts[i][0];
				FindFstInNextRule(follows[i][0], idx, flwStatus);
			}
		}
	}
}
