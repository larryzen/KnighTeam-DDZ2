#include "StringUtil.h"
#include "vector"

using namespace std;

StringUtil::StringUtil()
{

}

StringUtil::~StringUtil()
{
}

int StringUtil::getInt(string strText)
{
	int n=0;
	stringstream os;
	os<<strText;
	os>>n;

	return n;
}

string StringUtil::getString(int n)
{
	string s;
	stringstream os;
	os<<n;
	os>>s;

	return s;
}

vector<unsigned> StringUtil::stringSplit(string strText,char ch)
{
	vector<unsigned> list;
	string tmp_str="";
	int count=0;
	istringstream istr(strText);

	while(! istr.eof())
	{
		getline(istr,tmp_str,ch);
		list.push_back(getInt(tmp_str));
	}

	return list;
}