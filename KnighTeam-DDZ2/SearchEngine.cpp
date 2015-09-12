#include "SearchEngine.h"
#include<vector>
#include"define.h"
#include "time.h"
#include "DDZMoveManager.h"
#include "FileWriter.h"
#include "DDZCombFactory.h"

using namespace std;

CSearchEngine::CSearchEngine(void)
{
	ddz_MM = DDZMoveManager();
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

/**
*	被动出牌必胜，只考虑炸、火箭管牌
*/
bool CSearchEngine::CanWin2(vector<CARDSMOVE> moves)
{
	ddz_MM.setMovesStatus(&moves);
	int maxMovesNum = 0;
	vector<CARDSMOVE> maxMoves = vector<CARDSMOVE>();
	for (int i = 0; i < moves.size(); i++)
	{
		if (moves[i].status == STATUS_MAX && (moves[i].cardsType == ZHADAN || moves[i].cardsType==ROCKET))
		{
			maxMovesNum++;
			maxMoves.push_back(moves[i]);
		}
	}

	if (maxMovesNum >= (moves.size() - maxMovesNum))
	{
		for (int k = 0; k < maxMoves.size(); k++)
		{
			bool canPlay = CMoveGenerator::IsValidMove(Player::lastMove, maxMoves[k]);
			if (canPlay)
			{
				bestMove = maxMoves[k];
				return true;
			}
		}
	}

	return false;
}

/*
*	主动出牌下，存在22(MAX)、大王(MAX)、66、J、A；达到必胜状态
*   maxsize >= (othersize - 1)
*/
bool CSearchEngine::CanWin3(vector<CARDSMOVE> moves)
{
	ddz_MM.setMovesStatus(&moves);
	vector<CARDSMOVE> maxMoves = vector<CARDSMOVE>();//对status为STATUS_MAX的走步存储
	vector<CARDSMOVE> otherMoves = vector<CARDSMOVE>();
	for (int i = moves.size() - 1; i >= 0; i--)
	{
		if (IsMaxOnlyOne(moves[i]))
		{
			maxMoves.push_back(moves[i]);
		}
		else
		{
			otherMoves.push_back(moves[i]);
		}
	}
	vector<CARDSMOVE> oneToOne = vector<CARDSMOVE>();
	if (maxMoves.empty())//没有max走步
	{
		return false;
	}
	if (maxMoves.size() < otherMoves.size() - 1)
	{
		return false;
	}
	else
	{
		CARDSMOVE goodMove = CARDSMOVE();
		goodMove.cardsType = PASS;
		for (int k = maxMoves.size() - 1; k >= 0; k--)
		{
			CARDSMOVE maxMove = maxMoves[k];
			if (maxMove.cardsType == ZHADAN || maxMove.cardsType == ROCKET)
			{
				CARDSMOVE m = CARDSMOVE();
				m.cardsType = PASS;
				oneToOne.push_back(m);//如果为炸弹，火箭先不选择对应的小牌
			}
			else
			{
				for (int t = otherMoves.size() - 1; t >= 0; t--)
				{
					if (CMoveGenerator::IsValidMove(otherMoves[t], maxMove))
					{
						goodMove = otherMoves[t];
						oneToOne.push_back(otherMoves[t]);
						vector<CARDSMOVE>::iterator it = otherMoves.begin();
						otherMoves.erase(it+t);
					}
				}
			}
			
		}

		for (int i = oneToOne.size() - 1; i >= 0; i--)
		{
			if (oneToOne[i].cardsType == PASS)
			{
				if (otherMoves.size() > 0)
				{
					oneToOne[i] = otherMoves[0];
					otherMoves.erase(otherMoves.begin());
				}
			}
		}

		if (otherMoves.size() <= 1)
		{
			if (goodMove.cardsType != PASS)
			{
				bestMove = goodMove;//将非炸对应小牌作为走步
				return true;
			}
			else 
			{
				for (int tmp = 0; tmp < oneToOne.size(); tmp++)
				{
					if (oneToOne[tmp].cardsType != PASS)
					{
						bestMove = oneToOne[tmp];// 没有非炸对应小牌，选一个炸对应小牌
						return true;
					}		
				}

				bestMove = maxMoves[0];//没有小牌，选一个大牌作为走步
				return true;
				
			}
			
		}
		
		return false;
	}

	return false;
}

/**
*	被动出牌必胜，7777（管牌）、3、大王（Max)、66、22（Max)、J
*	maxsize >= othersize
*/
bool CSearchEngine::CanWin4(vector<CARDSMOVE> moves)
{
	ddz_MM.setMovesStatus(&moves);
	vector<CARDSMOVE> maxMoves = vector<CARDSMOVE>();//对status为STATUS_MAX的走步存储
	vector<CARDSMOVE> otherMoves = vector<CARDSMOVE>();//手中除去status_Max以后的牌
	vector<CARDSMOVE> canPlayMoves = vector<CARDSMOVE>();//能管上其他玩家出的牌的status_Max的牌
	vector<CARDSMOVE> oneToOne = vector<CARDSMOVE>();
	for (int i = moves.size() - 1; i >= 0; i--)
	{
		if (moves[i].status == STATUS_MAX)
		{
			maxMoves.push_back(moves[i]);
		}
		else
		{
			otherMoves.push_back(moves[i]);
		}
	}

	


	if (maxMoves.size() < otherMoves.size() )
	{
		return false;
	}
	else
	{
		CARDSMOVE goodMove = CARDSMOVE();
		goodMove.cardsType = PASS;
		for (int i = maxMoves.size() - 1; i >= 0; i--)
		{
			if (CMoveGenerator::IsValidMove(Player::lastMove, maxMoves[i]))
			{
				goodMove = maxMoves[i];
				canPlayMoves.push_back(maxMoves[i]);
			}
		}
		if (canPlayMoves.empty())//手中大牌不能管上其他玩家出牌，返回false；
		{
			return false;
		}
		
		for (int i = canPlayMoves.size() - 1; i >= 0; i--)
		{
			for (int k = maxMoves.size() - 1; k >= 0; k--)
			{
				CARDSMOVE maxMove = maxMoves[k];
				if (DDZCombFactory::IsSameCardsMove(canPlayMoves[i], maxMoves[k]))
				{
					CARDSMOVE m = CARDSMOVE();
					m.cardsType = -2;//如果为管其他玩家的牌，先不选择对应的小牌，cardsType=-2
					oneToOne.push_back(m);
					continue;
				}
				if (maxMove.cardsType == ZHADAN || maxMove.cardsType == ROCKET)
				{
					CARDSMOVE m = CARDSMOVE();
					m.cardsType = PASS;//如果为炸弹，火箭先不选择对应的小牌，cardsType=Pass
					oneToOne.push_back(m);
				}
				else
				{
					for (int t = otherMoves.size() - 1; t >= 0; t--)
					{
						if (CMoveGenerator::IsValidMove(otherMoves[t], maxMove))
						{
							goodMove = otherMoves[t];
							oneToOne.push_back(otherMoves[t]);
							vector<CARDSMOVE>::iterator it = otherMoves.begin();
							otherMoves.erase(it + t);
						}
					}
				}

				for (int n = oneToOne.size() - 1; n >= 0; n--)
				{
					if (oneToOne[n].cardsType == PASS)
					{
						if (otherMoves.size() > 0)
						{
							oneToOne[n] = otherMoves[0];
							otherMoves.erase(otherMoves.begin());
						}
					}
				}

				if (otherMoves.size() <= 1)
				{
					bestMove = canPlayMoves[i];
					return true;
				}

				return false;
			}
		}
		
		return false;
	}
}

/*
*
*	走步当前最大牌，且在对手手中没有此牌,唯一最大牌
*/
bool CSearchEngine::IsMaxOnlyOne(CARDSMOVE move)
{
	bool onlyOne = true;
	/*if (move.cardsType == SINGLE || move.cardsType == COUPLE)
	{
		unsigned cards[15] = { 0 };
		for (int i = move.cards.size() - 1; i >= 0; i--)
		{
			cards[move.cards[i]]++;
		}

		for (int j = 0; j < 15; j++)
		{
			if (cards[j] != 0)
			{
				if (Player::remaining[j] >= cards[j])
				{
					onlyOne = false;
				}
			}
		}
	}*/
	
	return move.status == STATUS_MAX && onlyOne;
}
bool CSearchEngine::CanWin(vector<CARDSMOVE> FirstMoves, int outWay)
{
	if (outWay==1 || outWay == 2)
	{
		ddz_MM.setMovesStatus(&FirstMoves);
		if (ddz_MM.canPlayOver(FirstMoves))
		{
			int size = FirstMoves.size();
			if (size == 1 && outWay == 1)
			{
				bestMove = FirstMoves[0];	
			}
			else
			{
				for (int i = 0; i < size; i++)
				{
					if (FirstMoves[i].status == STATUS_MAX && outWay == 1)
					{
						bestMove = FirstMoves[i];
						bestMove.side = 0;
					}
						
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