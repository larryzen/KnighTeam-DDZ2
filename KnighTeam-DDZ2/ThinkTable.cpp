#include "ThinkTable.h"
#include "define.h"
#include "vector"
#include "DDZCombFactory.h"



CThinkTable::CThinkTable()
{
}


CThinkTable::~CThinkTable()
{
}


int CThinkTable::getBid()
{
	int bid = 0;
	unsigned cards[15];
	int cardsNum = 0;
	memcpy(cards, Player::p3_EachCardNum, sizeof(cards));
	cardsNum = Player::p3_cardsNum;
	DDZCombFactory ddz_CF = DDZCombFactory(cards,Player::p3_cardsNum);
	vector<CARDSMOVE> moves = ddz_CF.getComb1LeastSingle().moves;
	ddz_CF.setCarryCards1_1ForSantiao(&moves);
	ddz_CF.setCarryCards3_1ForThreeJunko(&moves);
	

	bid =getBidByComb(moves.size());//根据组合步数叫分

	int level = getBidByTopCards(cards);

	updateBidByCombsWithTopCards(&bid, level);

	updateBidByTheirBid(&bid);

	int currentBid = getCurrentMaxBid();

	if (currentBid < bid) 
	{
		return bid;
	}
	else
	{
		return 0;	//若没有前面叫牌者叫的分高，则必须叫0分
	}
	
}


/**
*  根据组合步数和大牌数量改变叫分
* @bid       原本叫分
* @level    大牌级数,1好 2一般 3差
*/
void CThinkTable::updateBidByCombsWithTopCards(int *bid, const int level)
{
	switch(*bid)
	{
	case 0:
		{
			if( level ==1)
			{
				*bid = 2;
			}

			break;
		}
	case 1:
		{
			if( level ==1)
			{
				*bid = 2;
			}
			else if (level == 3)
			{
				*bid = 0;
			}
			break;
		}
	case 2:
		{
			if( level == 1)
			{
				*bid = 3;
			}
			else if (level == 3)
			{
				*bid = 1;
			}
			break;
		}
	case 3:
		{
			if( level ==3 || level == 2)
			{
				*bid = 2;
			}
			break;
		}
	}
}

/**
*    根据组合步数叫分
*/
int CThinkTable::getBidByComb(int combsNum)
{
	if(combsNum < 6)
	{
		return 3;
	}
	else if(combsNum < 8)
	{
		return 2;
	}
	else if(combsNum < 10)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*	统计手中大牌个数，所到达的水平
*	好、一般、差
*	1、 2 、 3
*/
int CThinkTable::getBidByTopCards(unsigned *cards)
{
	if(cards[14] == 1) // 一个大王
	{
		if(cards[13] == 1) // 一个小王
		{
			if(cards[12] >0)
			{
				return 1;//好
			}
			else
			{
				return 2;//一般
			}
		}
		else						// 没有小王
		{
			if(cards[12] > 1)
			{
				return 1;
			}
			else if(cards[12] == 1)
			{
				return 2;
			}
			else
			{
				return 3;//差
			}
		}
	}
	else						// 没有大王
	{
		if(cards[13] == 1)// 一个小王
		{
			if(cards[12] > 2)
			{
				return 1;
			}
			else if(cards[12] >1)
			{
				return 2;
			}
			else
			{
				return 3;
			}
		}
		else					// 没有小王
		{
			if(cards[12] > 3)
			{
				return 1;
			}
			else if(cards[12] >2)
			{
				return 2;
			}
			else
			{
				return 3;
			}
		}
	}
}

/**
*	统计我方牌点数
*/
int CThinkTable::getBidByCardsPoints(unsigned *cards)
{
	int cardsPoints = 0;// 记录牌点数
	for (size_t i = 0; i < 13; i++)
	{
		if (cards[i] == 1)
		{
			if (i < 4)
			{
				cardsPoints -= 2;
			}
			else if (i < 8)
			{
				cardsPoints -= 1;
			}
		}
		else if (cards[i] == 2)
		{
			if (i < 3)
			{
				cardsPoints -= 1;
			}
			else if (i > 8 && i < 12)
			{
				cardsPoints += 1;
			}
		}
		else if (cards[i] == 3)
		{
			if (i >= 8)
			{
				cardsPoints += 2;
			}
		}
		else
		{
			cardsPoints += 6;
		}
	}


	return cardsPoints;
}

/*
*	根据其他玩家已叫牌叫分
*/
void CThinkTable::updateBidByTheirBid(int *bid)
{
	if (Player::p1_bid != -1 && Player::p2_bid != -1)
	{
		if (Player::p1_bid < 2 && Player::p2_bid < 2)
		{
			if (*bid != 3)
			{
				*bid += 1;
			}	
		}
	}
}

/** 统计我方手中3~K、A、2、小王、大王15种牌型数量*/
void CThinkTable::initEachCardNum()
{
	for(size_t i=0;i<15;i++)
	{
		Player::p3_EachCardNum[i]=0;// 初始化或重置数量
	}
}

/**  更新我方手中牌p3_EachCardNum
*     @list  需更新牌的下标
*     @flag 标识---> 1减少 、0增加
*/
void CThinkTable::updateEachCardNum(vector<unsigned> list, int flag)
{
	if(flag)
	{
		increaseEachCardNum(list);
	}
	else
	{
		decreaseEachCardNum(list);
	}
}


void CThinkTable::increaseEachCardNum(vector<unsigned> list)
{
	for(int i=list.size()-1;i>=0;i--)
	{
		if(list[i] == 53)
		{
			Player::p3_EachCardNum[14]++;
		}
		else
		{
			int k = list[ i ] / 4;

			Player::p3_EachCardNum[ k ]++;
		}
	}
}

void CThinkTable::decreaseEachCardNum(vector<unsigned> list)
{
	for(int i=list.size()-1;i>=0;i--)
	{
		if(list[i] == 53)
		{
			if(Player::p3_EachCardNum[i] != 0)
			{
				Player::p3_EachCardNum[14]--;
			}
		}
		else
		{
			int k = list[ i ] / 4;
			if(Player::p3_EachCardNum[ k ] !=0)
			{
				Player::p3_EachCardNum[ k ]--;
			}
		}
	}
}

/**
*	获得当前玩家最大叫分
*/
int CThinkTable::getCurrentMaxBid()
{
	int maxBid = -1;

	max(maxBid, Player::p1_bid);
	max(maxBid, Player::p2_bid);
	
	return maxBid;
}


bool CThinkTable::IsHalfGame()
{
	/*if (Player::p1_cardsNum < HalfGame || Player::p2_cardsNum < HalfGame || Player::p3_cardsNum)
	{
		return true;
	}*/

	if ((Player::p1_cardsNum +Player::p2_cardsNum+ Player::p3_cardsNum)/3 < HalfGame)
	{
		return true;
	}

	return false;
}


