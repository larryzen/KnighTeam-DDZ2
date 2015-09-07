#include "FileWriter.h"
#include "string.h"
#include "time.h"

FileWriter::FileWriter(void)
{
	FileWriter(getSystemTime1());
}

FileWriter::~FileWriter(void)
{
}

FileWriter::FileWriter(string filePath)
{
	this->filePath=filePath;
}


void FileWriter::WriteToFile(vector<CARDSMOVE> moves,CARDSMOVE bestMove)
{
	fstream fout=fstream(filePath,ios::app);

	for(unsigned i=0;i<moves.size();i++)
	{
		fout<<"\t\t";
		fout << moves.at(i).side << "\t";
		for(unsigned j=0;j<moves.at(i).cards.size();j++)
		{
			fout<<moves.at(i).cards.at(j)<<" ";
		}
		fout<<endl;		
	}

	fout<<"\t\t================================="<<endl;
	fout.flush();
	fout.close();
}


void FileWriter::CombsToFile(CombsLib combs)
{
	fstream fout=fstream(filePath,ios::app);

	for(size_t i=0;i<combs.size();i++)
	{
		fout<<"\t\t组合 "<<i<<endl;
		for(size_t j=0;j<combs[i].moves.size();j++)
		{
			fout<<"\t\t";
			for (size_t k = 0; k<combs[i].moves.at(j).cards.size(); k++)
			{
				fout << combs[i].moves.at(j).cards.at(k) << " ";
			}
			fout<<endl;
		}

		fout<<endl<<endl;
	}

	fout.flush();
	fout.close();
}

void FileWriter::writeOrderFromFatherExe(char *cInMessage)
{
	fstream fout = fstream(filePath, ios::app);
	string a = string(cInMessage);
	fout << a << endl;
	fout.flush();
	fout.close();
}

/**
*	将错误信息写入error.txt中
*
*/
void FileWriter::writeErrorInfo(string errorInfo)
{
	fstream fout = fstream("error.txt", ios::app);
	
	fout << errorInfo << endl;
	fout.flush();
	fout.close();
}

void FileWriter::writeRandNun(int r)
{
	fstream fout = fstream(filePath, ios::app);
	fout << r <<",";
	fout.flush();
	fout.close();
}

string FileWriter::getSystemTime1()
{
	time_t t;
	time(&t);

	char strTime[64];
	struct tm tminfo;
	localtime_s(&tminfo, &t);
	strftime(strTime, sizeof(strTime), "%Y年%m月%d日%H时%M分%S秒.txt", &tminfo);
	string s = string(strTime);
	return s;
}