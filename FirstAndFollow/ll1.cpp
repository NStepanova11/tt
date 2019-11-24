#include "ll1.h"

void ll1::AddLine(string lexem, vector<string> guideSet, int jump)
{
	line l;
	l.lexem =lexem;
	l.guideSet = guideSet;
	l.jump = jump;
	table.push_back(l);
}

/*
void ll1::BuildTable()
{
	FirstFollowHandler gr;
	gr.ReadGrammarFromFile();
	unordered_map<string, vector<string>> predicts = gr.GetSets();
	vector<string> leftParts = gr.GetLeftParts();
	vector<vector<string>> rightParts = gr.GetRightParts();

	int jump = 0;  //индекс строки, на которую нужно перейти
	for (size_t i = 0; i < leftParts.size(); i++)
	{
		jump++;
		AddLine(leftParts[i], predicts.at(leftParts[i]), jump);

		for (size_t k = 0; k < rightParts[i].size(); k++)
		{
			jump++;
			if (find(leftParts.begin(), leftParts.end(), rightParts[i][k]) == leftParts.end()) //если очередной символ в правиле терминал
			{
				if (k != rightParts[i].size() - 1)
				{
					AddLine(rightParts[i][k], {}, jump);
				}
				else
				{
					AddLine(rightParts[i][k], {}, NULL);
				}
			}
			else //если нетерминал
			{
				AddLine(rightParts[i][k], predicts.at(rightParts[i][k]), 1000);
				if (predicts.at(rightParts[i][k]).size() > 1)
				{
					for (auto ee : predicts.at(rightParts[i][k]))
					{
						vector<string> v;
						v.push_back(ee);
						AddLine(rightParts[i][k],v, 1000);
						jump++;

					}
				}
			}
		}
	}
}
*/


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
		{
			for (auto p : predict.second)
			{
				vector<string> v;
				v.push_back(p);
				jump++;
				AddLine(predict.first, v, 1); //если несколько альтернатив, до добавляем строки для каждой альтернативы
			}
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
								AddLine(rightParts[index][k], {}, 1);
							}
							else
							{
								AddLine(rightParts[index][k], {}, NULL);
							}
						}
						else
						{
							vector<string> v;
							v.push_back(predict.second[alt_num]);
							AddLine(rightParts[index][k], v, NULL);
							alt_num = -1;
						}
					}
					else //если нетерминал
					{
						AddLine(rightParts[index][k], predicts.at(rightParts[index][k]), 1);
					}
				}
			}
		}
	}
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
			cout << setw(10) << right << l.jump+1 << endl;
		else
			cout << setw(10) << right << "NULL" << endl;
	}
}


