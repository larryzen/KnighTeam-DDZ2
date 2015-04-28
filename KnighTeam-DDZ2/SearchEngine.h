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

	void MakeMove(CARDSMOVE move,Player p,int whoseGo);
	void UnMakeMove(CARDSMOVE move,Player p,int whoseGo);

	int IsGameOver(Player p,int nDepth);
	int SearchAGoodMove(Player p,int nDepth);// 在子类中实现
//	virtual vector<int> SearchAGoodMove(Player p);

	
};

