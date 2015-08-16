#pragma once
#include "vector"
#include "Player.h"
#include "MoveGenerator.h"
#include "Eveluation.h"
#include "define.h"

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
//	virtual vector<int> SearchAGoodMove(Player p);

	
};

