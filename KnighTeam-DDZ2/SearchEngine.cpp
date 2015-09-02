#include "SearchEngine.h"
#include<vector>
#include"define.h"
#include "time.h"
#include "DDZMoveManager.h"

using namespace std;

CSearchEngine::CSearchEngine(void)
{
	
}


CSearchEngine::~CSearchEngine(void)
{
	
}


/** 判断牌局是否结束
**  @ nDepth为搜索深度
**  若为我方胜利返回极大值，反之则返回极小值。牌局未结束返回 0,且算上收益
**  (特别注释：此程序默认设置我方为玩家p3)
*/
int CSearchEngine::IsGameOver(int nDepth)
{
	int p1_cardsNum = Player::p1_cardsNum;
	int p2_cardsNum = Player::p2_cardsNum;
	int p3_cardsNum = Player::p3_cardsNum;
	int gainValue = 0;
	if(p3_cardsNum==0)// 我方打完牌，直接返回极大值
	{
		gainValue = GetAllGain(0);
		return 1 * gainValue;// 返回极大值
	}

	if(Player::p3_IsLandlord==true)// 我方为地主
	{
		if(p1_cardsNum==0||p2_cardsNum==0)// 玩家p1,p2出完手中牌
		{
			gainValue = GetAllGain(1);
			return -1 * gainValue;// 返回极小值
		}
	}
	else                     // 我方不为地主
	{
		if(p1_cardsNum==0)// 玩家p1出完手中牌
		{
			if(Player::p1_IsLandlord)// 玩家p1为地主
			{
				gainValue = GetAllGain(1);
				return -1 * gainValue;// 返回极小值
			}
			else
			{
				gainValue = GetAllGain(1);
				return 1 * gainValue;// 返回极大值
			}
		}

		if(p2_cardsNum==0)// 玩家p2出完手中牌
		{
			if(Player::p2_IsLandlord)// 玩家p2为地主
			{
				gainValue = GetAllGain(2);
				return -1 * gainValue;// 返回极小值
			}
			else
			{
				gainValue = GetAllGain(2);
				return 1 * gainValue;// 返回极大值
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

int CSearchEngine::gain()
{
	int gain = 1;
	for (int i = Player::cardsMoveRecords.size() - 1; i >= 0; i--)
	{
		if (Player::cardsMoveRecords[i].cardsType == ROCKET ||
					 Player::cardsMoveRecords[i].cardsType == ZHADAN)
		{
			gain++;
		}
	}

	return gain;
}


int CSearchEngine::GetAllGain(int turn)
{
	return gain() + Spring(turn) + ReSpring(turn);
}

bool CSearchEngine::CanWin(vector<CARDSMOVE> FirstMoves, int outWay)
{
	DDZMoveManager ddz_MM = DDZMoveManager();
	if (outWay)
	{
		ddz_MM.setMovesStatus(&FirstMoves);
		if (ddz_MM.canPlayOver(FirstMoves))
		{
			int size = FirstMoves.size();
			if (size == 1)
			{
				bestMove = FirstMoves[0];	
			}
			else
			{
				for (int i = 0; i < size; i++)
				{
					if (FirstMoves[i].status == STATUS_MAX)
						bestMove = FirstMoves[i];
				}
			}
			return true;
		}
	}

	return false;
}

/**
*  是否是春天或者反春天
*/
int  CSearchEngine::Spring(int turn)
{
	int LandlordTurn = -1;
	if (!Player::cardsMoveRecords.empty())
	{
		LandlordTurn = Player::cardsMoveRecords[0].side;
	}
	else
	{
		return 0;
	}
	for (int i = Player::cardsMoveRecords.size() - 1; i >= 0; i--)
	{
		if (Player::cardsMoveRecords[i].side != LandlordTurn)
		{
			if (Player::cardsMoveRecords[i].cardsType != PASS)
				return 0;
		}
	}

	if (turn == LandlordTurn)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

/**
*	是否为反春天
*/
int CSearchEngine::ReSpring(int turn)
{
	int LandlordTurn = -1;
	if (!Player::cardsMoveRecords.empty())
	{
		LandlordTurn = Player::cardsMoveRecords[0].side;
	}
	else
	{
		return 0;
	}

	for (int i = Player::cardsMoveRecords.size() - 1; i >= 1; i--)//地主只
	{
		if (Player::cardsMoveRecords[i].side == LandlordTurn)
		{
			if (Player::cardsMoveRecords[i].cardsType != PASS)
				return 0;
		}
	}

	if (turn != LandlordTurn)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}