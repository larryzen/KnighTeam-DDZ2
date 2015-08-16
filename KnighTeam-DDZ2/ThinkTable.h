#pragma once
#include "stdafx.h"
#include "stdafx.h"
#include "define.h"
#include "DDZPlayer.h"
#include "vector"
#include "Player.h"

class CThinkTable
{

public:
	CThinkTable();
	~CThinkTable();
	int getBid();
	
	void initEachCardNum();
	void updateEachCardNum(vector<unsigned> list, int flag);
	static bool IsHalfGame();
private:
	void increaseEachCardNum(vector<unsigned> list);
	void decreaseEachCardNum(vector<unsigned> list);

	int getBidByComb(int combsNum);
	int getBidByTopCards(unsigned *cards);
	int getBidByCardsPoints(unsigned *cards);
	void updateBidByTheirBid(int *bid);
	void updateBidByCombsWithTopCards(int *bid, const int *level);
	int getCurrentMaxBid();
};

