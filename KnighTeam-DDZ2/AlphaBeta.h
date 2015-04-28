#pragma once
#include "SearchEngine.h"
#include "Player.h"

class AlphaBeta:public CSearchEngine
{
public:
	AlphaBeta(void);
	virtual ~AlphaBeta(void);
	int SearchAGoodMove(Player p,int nDepth);
	//vector<int> SearchAGoodMove(Player p);

	static CARDSMOVE bestMove;
	int PassivePlay(vector<CARDSMOVE> moves,Player p);

	int score;
	int currentScore;
	CARDSMOVE singleOrDualJunko[200];
	int junkoNum;

};

