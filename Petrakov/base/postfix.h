#ifndef __POSTFIX_H__
#define __POSTFIX_H__

#include "stack.h"
#include "Automat.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class TPostfix
{
  string infix;
  string postfix;
  vector<string> lexems;
  map<char, int> priority;
  SyntaxAutomat automat;

  void Parse();
public:
	void ToPostfix();
	TPostfix();
	TPostfix(string infx);
	string GetInfix();
	string GetPostfix();
	vector<string> GetOperands();
	vector<string> GetLexems();
	double Calculate(); // Ввод переменных, вычисление по постфиксной форме
	bool Validate();
};

#endif
