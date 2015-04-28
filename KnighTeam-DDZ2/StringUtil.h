#pragma once
#include "string"
#include "sstream"
#include "list"
#include "vector"

using namespace std;

class StringUtil
{
public:
	StringUtil();
	~StringUtil();
	int getInt(string strText);
	string getString(int n);
	vector<unsigned> stringSplit(string strText,char ch);
};