#pragma once
#include "fstream"
#include "string"
#include "define.h"

using namespace std;


class FileWriter
{
public:
	FileWriter(void);
	~FileWriter(void);
	FileWriter(string filePath);

	void WriteToFile(vector<CARDSMOVE> moves,CARDSMOVE bestMove);
	void CombsToFile(CombsLib combs);
	void writeOrderFromFatherExe(char *cInMessage);
	void writeErrorInfo(string errorInfo);
	void writeRandNun(int r);

private:
	string filePath;
};

