#include "ThinkTable.h"
#include "define.h"
#include "vector"
#include "MoveGenerator.h"
#include "DDZMoveManager.h"



CThinkTable::CThinkTable()
{
}


CThinkTable::~CThinkTable()
{
}

///**叫牌，返回分数 */
//int CThinkTable::getBid(Player p)
//{
//	int bid=0;
//	int cardsPoints=0;// 牌点
//	getMyCardsInfo(p);
//	if(Player::p3_cardsInfo.n.big==1)
//	{
//		cardsPoints+=6;  // 大王牌点+6
//	}
//	if(Player::p3_cardsInfo.n.small==1)
//	{
//		cardsPoints+=4;  // 小王牌点+4
//	}
//	if(Player::p3_cardsInfo.m.num!=0)
//	{
//		cardsPoints+=(2*Player::p3_cardsInfo.m.num); // 牌 2 牌点+2*数量
//	}
//
//	if(Player::p3_cardsInfo.a.num!=0) // 牌3
//	{
//		if(Player::p3_cardsInfo.a.num==1)
//			cardsPoints-=2;
//		if(Player::p3_cardsInfo.a.num==2)
//			cardsPoints-=1;
//		if(Player::p3_cardsInfo.a.num==4)
//			cardsPoints+=6;
//	}
//	if(Player::p3_cardsInfo.b.num!=0)  // 牌4
//	{
//		if(Player::p3_cardsInfo.b.num==1)
//			cardsPoints-=2;
//		if(Player::p3_cardsInfo.b.num==2)
//			cardsPoints-=1;
//		if(Player::p3_cardsInfo.b.num==4)
//			cardsPoints+=6;
//	}
//	if(Player::p3_cardsInfo.c.num!=0)  // 牌5
//	{
//		if(Player::p3_cardsInfo.c.num==1)
//			cardsPoints-=2;
//		if(Player::p3_cardsInfo.c.num==2)
//			cardsPoints-=1;
//		if(Player::p3_cardsInfo.c.num==4)
//			cardsPoints+=6;
//	}
//
//	if(Player::p3_cardsInfo.d.num!=0)  // 牌6
//	{
//		if(Player::p3_cardsInfo.d.num==1)
//			cardsPoints-=1;
//		if(p.p3_cardsInfo.d.num==4)
//			cardsPoints+=6;
//	}
//	if(p.p3_cardsInfo.e.num!=0)  // 牌7
//	{
//		if(p.p3_cardsInfo.e.num==1)
//			cardsPoints-=1;
//		if(p.p3_cardsInfo.e.num==4)
//			cardsPoints+=8;// 若有牌7的炸弹，则其他玩家小顺断牌，则我方牌差点都可以叫牌
//	}
//	if(p.p3_cardsInfo.f.num!=0)  // 牌8
//	{
//		if(p.p3_cardsInfo.f.num==1)
//			cardsPoints-=1;
//		if(p.p3_cardsInfo.f.num==4)
//			cardsPoints+=6;
//	}
//	if(p.p3_cardsInfo.g.num!=0)  // 牌9
//	{
//		if(p.p3_cardsInfo.g.num==1)
//			cardsPoints-=1;
//		if(p.p3_cardsInfo.g.num==4)
//			cardsPoints+=6;
//	}
//	if(p.p3_cardsInfo.h.num!=0)  // 牌10
//	{
//		if(p.p3_cardsInfo.h.num==4)
//			cardsPoints+=6;
//	}
//	if(p.p3_cardsInfo.i.num!=0)
//	{
//		if(p.p3_cardsInfo.i.num==3)
//			cardsPoints+=1;
//		if(p.p3_cardsInfo.i.num==4)  // 牌J
//			cardsPoints+=6;
//	}
//	if(p.p3_cardsInfo.j.num!=0)
//	{
//		if(p.p3_cardsInfo.j.num==3)
//				cardsPoints+=1;
//		if(p.p3_cardsInfo.j.num==4)  // 牌Q
//				cardsPoints+=6;
//	}
//	if(p.p3_cardsInfo.k.num!=0)
//	{
//		if(p.p3_cardsInfo.k.num==3)
//				cardsPoints+=1;
//		if(p.p3_cardsInfo.k.num==4)  // 牌K
//				cardsPoints+=6;
//	}
//	if(p.p3_cardsInfo.l.num!=0)
//	{
//		if(p.p3_cardsInfo.l.num==3)
//			cardsPoints+=1;
//		if(p.p3_cardsInfo.l.num==4)  // 牌A
//			cardsPoints+=6;
//	}
//		
//
//	if(cardsPoints>=8)
//		bid+=3;                  // 叫3分
//	else if(cardsPoints>=4)
//		bid+=2;                  // 叫2分
//	else if(cardsPoints>=2)
//		bid+=1;                  // 叫1分
//	return bid;
//}


int CThinkTable::getBid()
{
	int bid = 0;
	unsigned cards[15];
	int cardsNum = 0;
	memcpy(cards, Player::p3_EachCardNum, sizeof(cards));
	cardsNum = Player::p3_cardsNum;
	CMoveGenerator ddz_MG = CMoveGenerator();
	DDZMoveManager ddz_MM = DDZMoveManager();
	vector<CARDSMOVE> moves = ddz_MG.getMovesByMyself(cardsNum, cards);
	vector<CARDSMOVE> combs =ddz_MM.getCombBySocre();
	

	bid =getBidByComb(combs.size());//根据组合步数叫分

	int level = getBidByTopCards(cards);

	updateBidByCombsWithTopCards(&bid, &level);

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
* @level    大牌级数
*/
void CThinkTable::updateBidByCombsWithTopCards(int *bid, const int *level)
{
	switch(*bid)
	{
	case 0:
		{
			if( *level ==1)
			{
				*bid=1;
			}

			break;
		}
	case 1:
		{
			if( *level ==1)
			{
				*bid = 2;
			}
			break;
		}
	case 2:
		{
			if( *level == 1)
			{
				*bid = 3;
			}
			break;
		}
	case 3:
		{
			if( *level ==3)
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


