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
int CSearchEngine::IsGameOver(Player p,int nDepth)
{
	int p1_cardsNum = p.p1_cardsNum;
	int p2_cardsNum = p.p2_cardsNum;
	int p3_cardsNum = p.p3_cardsNum;

	if(p3_cardsNum==0)// 我方打完牌，直接返回极大值
	{
		return 19990+nDepth;// 返回极大值
	}

	if(p.p3_IsLandlord==true)// 我方为地主
	{
		if(p1_cardsNum==0||p2_cardsNum==0)// 玩家p1,p2出完手中牌
		{
			return -19990-nDepth;// 返回极小值
		}
	}
	else                     // 我方不为地主
	{
		if(p1_cardsNum==0)// 玩家p1出完手中牌
		{
			if(p.p1_IsLandlord)// 玩家p1为地主
			{
				return -19990-nDepth;// 返回极小值
			}
			else
			{
				return 19990+nDepth;// 返回极大值
			}
		}

		if(p2_cardsNum==0)// 玩家p2出完手中牌
		{
			if(p.p2_IsLandlord)// 玩家p2为地主
			{
				return -19990-nDepth;// 返回极小值
			}
			else
			{
				return 19990+nDepth;// 返回极大值
			}
		}
	}


	return 0;
}



/** 根据出牌移除手中的牌
*   @ move 出牌 @ whoseGo 轮到谁出牌
*   whoseGo的值等于 ……、4、3、2、1. 我方值%3==0
*/
void CSearchEngine::MakeMove(CARDSMOVE move,Player p,int whoseGo)
{
	 if(whoseGo%3==0)   // 我方
	 {
		 if(move.cardsType!=-1)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
					p.p3_EachCardNum[move.cards.at(i)]--;
			 }
		 p.p3_cardsNum-=(move.cards.size());		// 减去出牌数量
		 }
		 p.p3_general.push_back(move);
	 }
	 else if(whoseGo%3==2)  // 玩家p1，即我方下家
	 {
		 if(move.cardsType!=-1)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
				 p.p1_EachCardNum[move.cards.at(i)]--;
			 }

			 p.p1_cardsNum-=(move.cards.size());		// 减去出牌数量
		 }
		 p.p1_general.push_back(move);
	 }
	 else					// 玩家p2，即我方上家
	 {
		  if(move.cardsType!=-1)
		  {
				 for(size_t i=0;i<move.cards.size();i++)
				 {
					 p.p2_EachCardNum[move.cards.at(i)]--;
				 }

				 p.p2_cardsNum-=(move.cards.size());		// 减去出牌数量
		  }
		 p.p2_general.push_back(move);
	 }

	 
}

void CSearchEngine::UnMakeMove(CARDSMOVE move,Player p,int whoseGo)
{
	vector<CARDSMOVE>::iterator it;
	if(whoseGo%3==0)  // 我方
	 {
		 if(move.cardsType!=-1)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
				 p.p3_EachCardNum[move.cards.at(i)]++;
			 }
	 
			 p.p3_cardsNum+=(move.cards.size());		// 恢复出牌数量
		 }
		 p.p3_general.erase(p.p3_general.end()-1);
	 }
	 else if(whoseGo%3==2)  // 玩家p1，即我方下家
	 {
		 if(move.cardsType!=-1)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
				 p.p1_EachCardNum[move.cards.at(i)]++;
			 }

			 p.p1_cardsNum+=(move.cards.size());		// 恢复出牌数量
		 }
		 p.p1_general.erase(p.p1_general.end()-1);
	 }
	 else					// 玩家p2，即我方上家
	 {
		 if(move.cardsType!=-1)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
				 p.p2_EachCardNum[move.cards.at(i)]++;
			 }

			 p.p2_cardsNum+=(move.cards.size());		    // 恢复出牌数量
		 }
		 p.p2_general.erase(p.p2_general.end()-1);//恢复走步
	 }
}
