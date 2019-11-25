#include "ll1.h"

void ll1::AddLine(string lexem, vector<string> guideSet, int jump, bool left)
{
	line l;
	l.lexem =lexem;
	l.guideSet = guideSet;
	l.jump = jump;
	l.left = left;
	table.push_back(l);
}

void ll1::BuildTable()
{
	FirstFollowHandler gr;
	gr.ReadGrammarFromFile();
	unordered_map<string, vector<string>> predicts = gr.GetSets();
	vector<string> leftParts = gr.GetLeftParts();
	vector<vector<string>> rightParts = gr.GetRightParts();

	int jump = 0;
	for (auto predict : predicts)
	{
		int alt_num = 0;
		for (size_t i = 0; i < leftParts.size(); i++)
		{
			if (leftParts[i] == predict.first)
			{
				vector<string> v;
				v.push_back(predict.second[alt_num]);
				alt_num++;
				jump++;
				AddLine(predict.first, v, jump, true);

				for (size_t m = 0; m < rightParts[i].size(); m++)
				{
					jump++;
					if (find(leftParts.begin(), leftParts.end(), rightParts[i][m]) == leftParts.end()) //если очередной символ в правиле терминал
					{
						if (rightParts[i][m] != "#")
						{
							if (m != rightParts[i].size() - 1)
							{
								AddLine(rightParts[i][m], {}, jump, false);
							}
							else
							{
								AddLine(rightParts[i][m], {}, NULL, false);
							}
						}
						else
						{
							vector<string> v;
							v.push_back(predicts.at(leftParts[i])[alt_num-1]);
							AddLine(rightParts[i][m], v, NULL, false);
						}
					}
					else //если нетерминал
					{
						AddLine(rightParts[i][m], predicts.at(rightParts[i][m]), jump, false);
					}
				}
			}
		}
	}
	UpdateLinksForLeft(leftParts);
}

void ll1::ShowTable()
{
	cout << "\n\n";
	int i = 0;
	for (auto l : table)
	{
		i++;
		cout << setw(4) << left << to_string(i)+".";
		cout<<setw(10)<<left<<l.lexem;
		string str = "";
		for (auto e : l.guideSet)
			str.append(e + " ");
		cout << setw(15) << left << str ;
		if (l.jump!=0)
			cout << setw(10) << left << l.jump+1;
		else
			cout << setw(10) << left << "NULL" ;
		cout.setf(ios::boolalpha);
		cout << setw(10) << left<< l.left << endl;
	}
}


void ll1::UpdateLinksForLeft(vector<string> leftParts)
{
	for (size_t i = 0; i < table.size(); i++)
	{
		if (find(leftParts.begin(), leftParts.end(), table[i].lexem) != leftParts.end() && table[i].left == false)
		{
			for (size_t z = 0; z < table.size(); z++)
			{
				if (table[z].lexem == table[i].lexem && table[z].left == true)
				{
					table[i].jump = z;
				}
			}
		}

	}
}

/*
void ll1::BuildTable()
{
	FirstFollowHandler gr;
	gr.ReadGrammarFromFile();
	unordered_map<string, vector<string>> predicts = gr.GetSets();
	vector<string> leftParts = gr.GetLeftParts();
	vector<vector<string>> rightParts = gr.GetRightParts();

	int jump = 0;
	int alt_num=0;
	cout << endl;
	for (size_t i = 0; i < leftParts.size(); i++)
	{
		jump++;
		vector<string> v;
		cout << "Left: " << leftParts[i] << endl;
		///todo: неправильно определяет направляющее множество в альтернативном правиле
		if (alt_num < predicts.at(leftParts[i]).size()-1)
		{
			cout << "alt_num: " << alt_num << endl;
			cout << "set: " << predicts.at(leftParts[i])[alt_num] << endl;
			v.push_back(predicts.at(leftParts[i])[alt_num]); //берем очередной направляющий символ
			alt_num++;
		}
		else
		{
			alt_num = 0;
			v.push_back(predicts.at(leftParts[i])[alt_num]); //берем очередной направляющий символ
		}

		AddLine(leftParts[i], v, jump, true); //если несколько альтернатив, до добавляем строки для каждой альтернативы

		for (size_t k = 0; k < rightParts[i].size(); k++)
		{
			jump++;
			if (find(leftParts.begin(), leftParts.end(), rightParts[i][k]) == leftParts.end()) //если очередной символ в правиле терминал
			{
				if (rightParts[i][k] != "#")
				{
					if (k != rightParts[i].size() - 1)
					{
						AddLine(rightParts[i][k], {}, jump, false);
					}
					else
					{
						AddLine(rightParts[i][k], {}, NULL, false);
					}
				}
				else
				{
					vector<string> v;
					v.push_back(predicts.at(leftParts[i])[k]);
					AddLine(rightParts[i][k], v, NULL, false);
				}
			}
			else //если нетерминал
			{
				AddLine(rightParts[i][k], predicts.at(rightParts[i][k]), jump, false);
			}
		}
	}
}
*/
/*
void ll1::BuildTable()
{
	FirstFollowHandler gr;
	gr.ReadGrammarFromFile();
	unordered_map<string, vector<string>> predicts = gr.GetSets();
	vector<string> leftParts = gr.GetLeftParts();
	vector<vector<string>> rightParts = gr.GetRightParts();

	int jump = 0;
	for (auto predict : predicts) ///нетерминал - направляющее множество
	{
		for (auto p : predict.second)
		{
			vector<string> v;
			v.push_back(p);
			jump++;
			AddLine(predict.first, v, -1, true); //если несколько альтернатив, до добавляем строки для каждой альтернативы
		}

		int alt_num = -1;
		for (size_t index = 0; index < leftParts.size(); index++)
		{
			if (leftParts[index] == predict.first)
			{
				alt_num++;
				for (size_t k = 0; k < rightParts[index].size(); k++)
				{
					jump++;
					if (find(leftParts.begin(), leftParts.end(), rightParts[index][k]) == leftParts.end()) //если очередной символ в правиле терминал
					{
						if (rightParts[index][k] != "#")
						{
							if (k != rightParts[index].size() - 1)
							{
								AddLine(rightParts[index][k], {}, jump, false);
							}
							else
							{
								AddLine(rightParts[index][k], {}, NULL, false);
							}
						}
						else
						{
							vector<string> v;
							v.push_back(predict.second[alt_num]);
							AddLine(rightParts[index][k], v, NULL, false);
							alt_num = -1;
						}
					}
					else //если нетерминал
					{
						AddLine(rightParts[index][k], predicts.at(rightParts[index][k]), -1, false);
					}
				}
			}
		}
	}

	//UpdateLinksForLeft();
}
*/