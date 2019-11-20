#include "CGrammarReader.h"
#include "FirstFollowHandler.h"

int main()
{
	FirstFollowHandler gr;
	// 1 - прочитать грамматику
	gr.ReadGrammarFromFile();
	gr.ShowGrammar();
	// 2 - найти направляющие множества
	//gr.GetGuideSets();
	//gr.ShowGuideSets();
	
	system("pause");
	return 0;
}
