#pragma once
#include "SearchEngine.h"
#include "Player.h"
#include "DDZMoveManager.h"
#include "Eveluation.h"

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
	DDZMoveManager ddz_MM;
	CEveluation ddz_Eve;
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
	void init_EXPAND_DEPTH();
	void initCardsTypeNum();
	void CardsTypeNumCount(vector<CARDSMOVE> moves);
	int getMaxTimesBestMove();

	long start;
	long end;

	int EXPAND_DEPTH;
	int TOTAL_DEPTH;
	int SEARCH_TIME;
	double cpy_p1_MemberShip[TOTAL_KINDS], cpy_p2_MemberShip[TOTAL_KINDS];

	bool IsGetAGoodMove;
	void FinalDeal();

	void CanFinishGame();
	int getTopCardInRemaining();
	void getAGoodMoveAsOnlyOneCard(vector<CARDSMOVE> moves);
};

