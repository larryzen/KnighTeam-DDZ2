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
	void initMyCards(int cards[20]);
	int getBid(Player p);
	void initCardsInfo(Player p);// 
	void initEachCardNum(Player p);

	void getMyCardsInfo(Player p);
};

