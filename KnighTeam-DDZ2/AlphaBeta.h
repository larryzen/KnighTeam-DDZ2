#pragma once
#include "SearchEngine.h"
#include "Player.h"

class AlphaBeta:public CSearchEngine
{
public:
	AlphaBeta(void);
	virtual ~AlphaBeta(void);
	int SearchAGoodMove(int nDepth);
	//vector<int> SearchAGoodMove(Player p);
	int SearchAGoodMove();

	int score;
	int currentScore;
	CARDSMOVE singleOrDualJunko[200];
	int junkoNum;

private:
	
	int IsFinal();
	bool IsFinal1LastOne();
	bool IsFinal2LastTwo();
	bool IsFinal3LastThree();
	bool IsFinalByLastCardsNum(int lastCardsNum);
	int SearchAGoodMove(int nDepth, int nodeIndex);
	vector<CARDSMOVE> FirstMoves;
	int	getMaxScoreIndex(vector<CARDSMOVE> moves);
	int BestMoveLinkList[BestMoveLinkListLen];

	void initBestMoveLinkList();
	int getMaxTimesBestMove();

	long start;
	long end;

};

