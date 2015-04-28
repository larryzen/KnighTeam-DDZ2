#pragma once
#include "SearchEngine.h"
#include "time.h"

class MCSearch:public CSearchEngine
{
public:
	MCSearch(void);
	~MCSearch(void);

	vector<unsigned> SearchAGoodMove(Player p);
	int SearchAGoodMove(Player p,int nDepth);
	void SearchAGoodMove(Player p,int nDepth,int randTimes);
	int getMaxElementIndex(int v[300]);
	void initBestMoveList();
	

	int MoveIndex;
	int VODList[300];
	int BestMoveList[300];
};

