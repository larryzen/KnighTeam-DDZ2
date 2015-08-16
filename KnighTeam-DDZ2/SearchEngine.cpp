#include "SearchEngine.h"
#include<vector>
#include"define.h"
#include "time.h"

using namespace std;

CSearchEngine::CSearchEngine(void)
{
	
}


CSearchEngine::~CSearchEngine(void)
{
	
}


/** 判断牌局是否结束
**  @ nDepth为搜索深度
**  若为我方胜利返回极大值，反之则返回极小值。牌局未结束返回 0
**  (特别注释：此程序默认设置我方为玩家p3)
*/
int CSearchEngine::IsGameOver(int nDepth)
{
	int p1_cardsNum = Player::p1_cardsNum;
	int p2_cardsNum = Player::p2_cardsNum;
	int p3_cardsNum = Player::p3_cardsNum;

	if(p3_cardsNum==0)// 我方打完牌，直接返回极大值
	{
		return 1;// 返回极大值
	}

	if(Player::p3_IsLandlord==true)// 我方为地主
	{
		if(p1_cardsNum==0||p2_cardsNum==0)// 玩家p1,p2出完手中牌
		{
			return -1;// 返回极小值
		}
	}
	else                     // 我方不为地主
	{
		if(p1_cardsNum==0)// 玩家p1出完手中牌
		{
			if(Player::p1_IsLandlord)// 玩家p1为地主
			{
				return -1;// 返回极小值
			}
			else
			{
				return 1;// 返回极大值
			}
		}

		if(p2_cardsNum==0)// 玩家p2出完手中牌
		{
			if(Player::p2_IsLandlord)// 玩家p2为地主
			{
				return -1;// 返回极小值
			}
			else
			{
				return 1;// 返回极大值
			}
		}
	}


	return 0;
}



/** 根据出牌移除手中的牌
*   @ move 出牌 @ whoseGo 轮到谁出牌
*   whoseGo的值等于 ……、4、3、2、1. 我方值%3==0
*/
void CSearchEngine::MakeMove(CARDSMOVE move,int whoseGo)
{
	Player::cardsMoveRecords.push_back(move);

	 if(whoseGo % 3 == 0)   // 我方
	 {
		 if(move.cardsType!=PASS)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
					Player::p3_EachCardNum[move.cards.at(i)]--;
			 }
		 Player::p3_cardsNum-=(move.cards.size());		// 减去出牌数量
		 }
	 }
	 else if(whoseGo % 3 == 1 )  // 玩家p1，即我方下家
	 {
		 if(move.cardsType != PASS)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
				 Player::p1_EachCardNum[move.cards.at(i)]--;
				 Player::remaining[move.cards[i]]--;
			 }

			 Player::p1_cardsNum-=(move.cards.size());		// 减去出牌数量
		 }
	 }
	 else					// 玩家p2，即我方上家
	 {
		  if(move.cardsType != PASS)
		  {
				 for(size_t i=0;i<move.cards.size();i++)
				 {
					 Player::p2_EachCardNum[move.cards.at(i)]--;
					 Player::remaining[move.cards[i]]--;
				 }

				 Player::p2_cardsNum-=(move.cards.size());		// 减去出牌数量
		  }
	 }

	 
}

void CSearchEngine::UnMakeMove(CARDSMOVE move,int whoseGo)
{
	Player::cardsMoveRecords.erase(Player::cardsMoveRecords.end()-1);
	
	if(whoseGo % 3 == 0)  // 我方
	 {
		 if(move.cardsType != PASS)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
				 Player::p3_EachCardNum[move.cards.at(i)]++;
			 }
	 
			 Player::p3_cardsNum+=(move.cards.size());		// 恢复出牌数量
		 }
	 }
	 else if(whoseGo % 3 == 1)  // 玩家p1，即我方下家
	 {
		 if(move.cardsType != PASS)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
				 Player::p1_EachCardNum[move.cards.at(i)]++;
				 Player::remaining[move.cards[i]]++;
			 }

			 Player::p1_cardsNum+=(move.cards.size());		// 恢复出牌数量
		 }
	 }
	 else					// 玩家p2，即我方上家
	 {
		 if(move.cardsType != PASS)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
				 Player::p2_EachCardNum[move.cards.at(i)]++;
				 Player::remaining[move.cards[i]]++;
			 }

			 Player::p2_cardsNum+=(move.cards.size());		    // 恢复出牌数量
		 }
	 }
}
