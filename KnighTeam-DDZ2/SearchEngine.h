#pragma once
#include "vector"
#include "Player.h"
#include "MoveGenerator.h"
#include "Eveluation.h"
#include "define.h"
#include "DDZMoveManager.h"

using namespace std;


class CSearchEngine
{
	
public:
	

	CSearchEngine(void);
	~CSearchEngine(void);

	void MakeMove(CARDSMOVE move,int whoseGo);
	void UnMakeMove(CARDSMOVE move,int whoseGo);

	int IsGameOver(int nDepth);
	virtual int SearchAGoodMove(int nDepth)=0;// 在子类中实现
	CARDSMOVE bestMove;

	bool CanWin(vector<CARDSMOVE> FirstMoves, int outWay);
	bool CanWin2(vector<CARDSMOVE> moves);
	bool CanWin3(vector<CARDSMOVE> moves);
	bool CanWin4(vector<CARDSMOVE> moves);
//	virtual vector<int> SearchAGoodMove(Player p);


private:
	int gain();
	int Spring(int turn);
	int ReSpring(int turn);
	int GetAllGain(int turn);
	DDZMoveManager ddz_MM;

	bool IsMaxOnlyOne(CARDSMOVE move);
};

