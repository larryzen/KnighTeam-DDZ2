#include "AlphaBeta.h"
#include "FileWriter.h"
#include "sstream"
#include "DDZMoveManager.h"
#include "DDZCombFactory.h"

AlphaBeta::AlphaBeta(void)
{
	junkoNum=0;
	bestMove = CARDSMOVE();
	FirstMoves = vector<CARDSMOVE>();
};

AlphaBeta::~AlphaBeta(void)
{
}


int AlphaBeta::SearchAGoodMove(int nDepth)
{
	/*DDZMoveManager ddz_MM = DDZMoveManager();*/
	
	SearchAGoodMove(nDepth, -1);
	int i = getMaxScoreIndex(FirstMoves);
	if (i!=-1)
		bestMove = FirstMoves[i];
	else
		bestMove = FirstMoves[0];
	 
	//int canPlayOver = 0;
	//int current=-99999;
	//CMoveGenerator *ddz_MG;
	//CEveluation *ddz_Eve;
	//ddz_MG = (CMoveGenerator*)malloc(sizeof(CMoveGenerator));
	//ddz_Eve = (CEveluation*)malloc(sizeof(CEveluation));
	//vector<CARDSMOVE> moves = vector<CARDSMOVE>();

	//moves = ddz_MG->getMoves(nDepth);

	//if (nDepth == 0)
	//{
	//	FirstMoves = moves;
	//}
	//if (Player::p3_cardsNum == 20)
	//{
	//	bestMove = ddz_Eve->firstAttack(moves);
	//	return 0;
	//}
	//int result = IsGameOver(nDepth);
	//if (result)
	//{
	//	return  result * 9999;
	//}
	//
	//if (nDepth == AlphaBeta_Depth)
	//{
	//	return 0;
	//}

	////if(moves.size()>0&&moves[0].outWay==1)
	////{
	////	Player::p3_allMoves=moves;
	////}

	//ddz_Eve->PositiveSimulatedMenu();

	//ddz_Eve->EveluateMoves(&moves, nDepth);

	//for (size_t i = 0; i < moves.size(); i++)
	//{
	//	MakeMove(moves[i], nDepth);
	//	SearchAGoodMove(nDepth + 1);
	//	UnMakeMove(moves[i], nDepth);
	//}

	//ddz_Eve->EveluateMoves(&Player::p3_allMoves, nDepth);
	//

	////vector<CARDSMOVE> comb = ddz_MM.getCombBySocre();
	////ddz_MM.setMovesStatus(&comb);

	////// 判断是否达到必胜状态
	////Player::p3_canPlayOver = ddz_MM.canPlayOver(comb);
	////if (Player::p3_canPlayOver)
	////{
	////	Player::p3_comb = comb;
	////}

	//for (size_t i = 0; i<moves.size(); i++)
	//{
	//	if (nDepth == 0)
	//	{
	//		if (current<moves[i].score)
	//		{
	//			bestMove = moves[i];
	//			current = moves[i].score;
	//		}
	//	}
	//}


	//int way = moves[0].outWay;
	//if(way)//主动出牌
	//{
	//	//// 当到终局时，需特殊处理
	//	//if ((Player::p1_IsLandlord && Player::p1_cardsNum == 1) || (Player::p2_IsLandlord && Player::p2_cardsNum == 1)
	//	//	|| Player::p3_IsLandlord && (Player::p1_cardsNum == 1 || Player::p2_cardsNum == 1))
	//	//{
	//	//	vector<CARDSMOVE> single = ddz_MM.getGoodMove3Single(Player::p3_allMoves, moves, Player::p3_EachCardNum, 0);
	//	//	if (single.size()>1)
	//	//	{
	//	//		bestMove = single[single.size() - 1];
	//	//		return 0;
	//	//	}
	//	//}
	//	//if (Player::p3_canPlayOver)//到达必胜状态
	//	//{
	//	//	if (Player::p3_comb.size() == 1)
	//	//	{
	//	//		bestMove = Player::p3_comb[0];
	//	//		return 0;
	//	//	}

	//	//	for (size_t i = 0; i < Player::p3_comb.size(); i++)
	//	//	{
	//	//		if (Player::p3_comb[i].status == STATUS_MAX)
	//	//		{
	//	//			bestMove = Player::p3_comb[i];
	//	//			vector<CARDSMOVE>::iterator it = Player::p3_comb.begin() + i;
	//	//			Player::p3_comb.erase(it);
	//	//			return 0;
	//	//		}
	//	//	}
	//	//}

	//	/*int k =ddz_MM.getMaxScoreMove(Player::p3_allMoves);
	//	if (ddz_Eve->IsPlay3Single(moves, Player::p3_EachCardNum))
	//	{
	//			vector<CARDSMOVE> tmp_single;
	//			vector<CARDSMOVE> single = ddz_MM.getGoodMove3Single(moves,tmp_single,Player::p3_EachCardNum,1);
	//			bestMove=single[0];
	//			return 0;
	//	}
	//	else if (ddz_Eve->IsPlay4Couple(moves, Player::p3_EachCardNum))
	//	{
	//		vector<CARDSMOVE> tmp_couple;
	//		vector<CARDSMOVE> couple = ddz_MM.getGoodMove4Couple(moves,tmp_couple,Player::p3_EachCardNum,1);
	//		bestMove=couple[0];
	//		return 0;
	//	}
	//	else if (ddz_Eve->IsPlay5Santiao(moves, Player::p3_EachCardNum))
	//	{
	//		vector<CARDSMOVE> tmp;
	//		vector<CARDSMOVE> single = ddz_MM.getGoodMove3Single(moves, tmp, Player::p3_EachCardNum, 1);
	//		vector<CARDSMOVE> couple = ddz_MM.getGoodMove4Couple(moves, tmp, Player::p3_EachCardNum, 1);
	//		vector<CARDSMOVE> threeJunko = ddz_MM.getGoodMove5Santiao(moves,tmp,Player::p3_EachCardNum,1);
	//		
	//		bestMove=threeJunko[0];
	//		if (single.size() > 0)
	//		{
	//			bestMove.cards.push_back(single[0].cards[0]);
	//		}
	//		else if (couple.size() > 0)
	//		{
	//			bestMove.cards.push_back(couple[0].cards[0]);
	//			bestMove.cards.push_back(couple[0].cards[0]);
	//		}
	//		return 0;
	//	}*/
	//	/*if(k>=0)
	//		bestMove=Player::p3_allMoves[k];
	//	else
	//		bestMove=Player::p3_allMoves[0];
	//	return 0;*/
	//}
	//else        //被动出牌
	//{
	//	/*bestMove = moves[0];*/
	//	//if (Player::p3_canPlayOver)
	//	//{
	//	//	for (size_t i = 0; i < Player::p3_comb.size(); i++)
	//	//	{
	//	//		for (size_t j = 0; j < moves.size(); j++)
	//	//		{
	//	//			if (ddz_MM.IsEqualsMove(Player::p3_comb[i],moves[j]) 
	//	//				&& Player::p3_comb[i].status == STATUS_MAX)
	//	//			{
	//	//				bestMove = Player::p3_comb[i];
	//	//				vector<CARDSMOVE>::iterator it = Player::p3_comb.begin() + i;
	//	//				Player::p3_comb.erase(it);
	//	//				return 0;
	//	//			}
	//	//			else if (Player::p3_comb.size() == 1 
	//	//				&& ddz_MM.IsEqualsMove(Player::p3_comb[0], moves[j]))
	//	//			{
	//	//				bestMove = Player::p3_comb[0];
	//	//				vector<CARDSMOVE>::iterator it = Player::p3_comb.begin();
	//	//				Player::p3_comb.erase(it);
	//	//				return 0;
	// //				}
	//	//		}
	//	//		
	//	//	}
	//	//}
	//	//if (ddz_Eve->IsAiPass())	//让同伴权
	//	//{
	//	//	NULL_MOVE(bestMove);
	//	//	return 0;
	//	//}

	//	//PassivePlay(moves);

	//	//if (!Player::p3_canPlayOver &&(bestMove.cardsType == ZHADAN || bestMove.cardsType == ROCKET))
	//	//{
	//	//	NULL_MOVE(bestMove);
	//	//}

	//	return 0;
	//}


	////fw.WriteToFile(moves,bestMove);写入文件
	////if(moves.size()==1)
	////{
	////	bestMove.cards.clear();
	////	bestMove.cardsType=-1;
	////}
	//delete(ddz_Eve);
	//delete(ddz_MG);
	//ddz_Eve = NULL;
	//ddz_MG = NULL;
	return 0;
}

int AlphaBeta::SearchAGoodMove(int nDepth, int nodeIndex)
{
	DDZMoveManager ddz_MM = DDZMoveManager();




	int canPlayOver = 0;
	int current = -99999;
	CMoveGenerator *ddz_MG;
	CEveluation *ddz_Eve;
	ddz_MG = (CMoveGenerator*)malloc(sizeof(CMoveGenerator));
	ddz_Eve = (CEveluation*)malloc(sizeof(CEveluation));
	vector<CARDSMOVE> moves = vector<CARDSMOVE>();

	moves = ddz_MG->getMoves(nDepth);

	if (nDepth == 0)
	{
		FirstMoves = moves;
	}
	/*if (Player::p3_cardsNum == 20)
	{
		bestMove = ddz_Eve->firstAttack(moves);
		return 0;
	}*/
	int result = IsGameOver(nDepth);
	if (result)
	{
		FirstMoves[nodeIndex].score += (result * 9999);
		return  0;
	}

	if (nDepth == AlphaBeta_Depth)
	{
		return 0;
	}

	//if(moves.size()>0&&moves[0].outWay==1)
	//{
	//	Player::p3_allMoves=moves;
	//}

	ddz_Eve->PositiveSimulatedMenu();

	ddz_Eve->EveluateMoves(&moves, nDepth);

	if (nDepth ==0)
	{
		FirstMoves = moves;
		for (size_t i = 0; i < moves.size(); i++)
		{
			MakeMove(moves[i], nDepth);

			if (nDepth==0)
				SearchAGoodMove(nDepth + 1, i);
			else
				SearchAGoodMove(nDepth + 1, nodeIndex);

			UnMakeMove(moves[i], nDepth);
		}
		
	}
	else
	{
		int index = getMaxScoreIndex(moves);
		if (index != -1)
		{
			MakeMove(moves[index], nDepth);
			if (nDepth % 3 != 0)
			{
				ddz_Eve->ClearedByPlayCards(moves[index].cards);
				ddz_Eve->UpdateByPlayCards(nDepth % 3, moves[index].cardsType, moves[index].cards);
			}
		
			SearchAGoodMove(nDepth + 1, nodeIndex);
			UnMakeMove(moves[index], nDepth);
		}
	}


	//vector<CARDSMOVE> comb = ddz_MM.getCombBySocre();
	//ddz_MM.setMovesStatus(&comb);

	//// 判断是否达到必胜状态
	//Player::p3_canPlayOver = ddz_MM.canPlayOver(comb);
	//if (Player::p3_canPlayOver)
	//{
	//	Player::p3_comb = comb;
	//}

	/*for (size_t i = 0; i<moves.size(); i++)
	{
		if (nDepth == 0)
		{
			if (current<moves[i].score)
			{
				bestMove = moves[i];
				current = moves[i].score;
			}
		}
	}*/
	return 0;
}

/** 被动出牌查找走步 */
int AlphaBeta::PassivePlay(vector<CARDSMOVE> moves)
{
	DDZMoveManager ddz_MM = DDZMoveManager();
	//被动出牌
	int cardsType = moves[0].cardsType;
	switch(cardsType)
	{
	case SINGLE:
		{
		    
			
			break;
		}
	case COUPLE:
		{
			
			break;
		}
	case THREE_ONE:	
		{
			vector<CARDSMOVE> tmp_single;
			vector<CARDSMOVE> single = ddz_MM.getGoodMove3Single(Player::p3_allMoves, tmp_single, Player::p3_EachCardNum, 1);
			//vector<CARDSMOVE> santiaos = ddz_MM.getGoodMove5Santiao(Player::p3_allMoves, tmp_single, Player::p3_EachCardNum, 1);
			if(single.size()>0)
			{
				int index = 0;
				while (index < moves.size())
				{
					if (moves[index].cardsType != ZHADAN
						&& Player::p3_EachCardNum[moves[index].cards[0]] != 4
						&& moves[index].score>0)
					{
						
						for (int i = moves[index].cards.size() - 1; i > 2; i--)
						{
							vector<unsigned>::iterator it = moves[index].cards.begin();
							moves[index].cards.erase(it + i);
						}
						if (moves[index].cards[0] != single[0].cards[0])
						{
							moves[index].cards.push_back(single[0].cards[0]);
							bestMove = moves[index];
							break;
						}
						else
						{
							index++;
						}
					}
					else
					{
						// 若为炸弹，则不拆
						index++;
					}
				}
			}
			else
				NULL_MOVE(bestMove);
			
			break;
		}
	case THREE_TWO:
		{
			vector<CARDSMOVE> tmp_couple;
			vector<CARDSMOVE> couple= ddz_MM.getGoodMove4Couple(Player::p3_allMoves,
				tmp_couple,Player::p3_EachCardNum,1);
			if(couple.size()>0)
			{
				int index = 0;
				while (index < moves.size())
				{
					if (moves[index].cardsType != ZHADAN 
						&& Player::p3_EachCardNum[moves[index].cards[0]]!= 4)
					{
						for (int i = moves[index].cards.size() - 1; i > 2; i--)
						{
							vector<unsigned>::iterator it = moves[index].cards.begin();
							moves[index].cards.erase(it + i);
						}
						if (moves[index].cards[0] != couple[0].cards[0])
						{
							moves[index].cards.push_back(couple[0].cards[0]);
							moves[index].cards.push_back(couple[0].cards[0]);
							bestMove = moves[index];
							break;
						}
						else
						{
							index++;
						}
							
						
						
					}
					else
					{
						// 若为炸弹，则不拆
						index++;
					}
				}
			}
			else
				NULL_MOVE(bestMove);
			
			break;
		}
	default:
		{
			if (currentScore < 0)
				NULL_MOVE(bestMove);

			break;
		}

	}	

	return 0;
}

int AlphaBeta::someoneLastOne(vector<CARDSMOVE> moves)
{
	// 当到终局时，需特殊处理
	if ((Player::p1_IsLandlord && Player::p1_cardsNum == 1) || (Player::p2_IsLandlord && Player::p2_cardsNum == 1)
		|| Player::p3_IsLandlord && (Player::p1_cardsNum == 1 || Player::p2_cardsNum == 1))
	{
		if (moves.size()>0)
		{
			bestMove = moves[moves.size() - 1];
			return 0;
		}
	}
}


int	AlphaBeta::getMaxScoreIndex(vector<CARDSMOVE> moves)
{
	int index = -1;
	int current = -9999;

	for (size_t i = 0; i < moves.size(); i++)
	{
		if (moves[i].score>current)
		{
			current = moves[i].score;
			index = i;
		}
	}

	return index;
}
int AlphaBeta::IsFinal()
{
	if (IsFinal1LastOne())
	{
		return 1;
	}
	else if (IsFinal2LastTwo())
	{
		return 2;
	}
		
	else if (IsFinal3LastThree())
	{
		return 3;
	}
	else
	{
		return 4;
	}
		
}

/*
*   终局其他玩家只剩下一张牌
*/
bool AlphaBeta::IsFinal1LastOne()
{
	return IsFinalByLastCardsNum(1);
}

/*
*   终局其他玩家只剩下两张牌
*/
bool AlphaBeta::IsFinal2LastTwo()
{
	return IsFinalByLastCardsNum(2);
}

/*
*   终局其他玩家只剩下三张牌
*/
bool AlphaBeta::IsFinal3LastThree()
{
	return IsFinalByLastCardsNum(3);
}


bool AlphaBeta::IsFinalByLastCardsNum(int lastCardsNum)
{
	if ((Player::p1_IsLandlord && Player::p1_cardsNum == lastCardsNum) || (Player::p2_IsLandlord && Player::p2_cardsNum == lastCardsNum)
		|| Player::p3_IsLandlord && (Player::p1_cardsNum == lastCardsNum || Player::p2_cardsNum == lastCardsNum))
	{
		return true;
	}

	return false;
}

//CARDSMOVE firstAttack(vector<CARDSMOVE> moves)
//{
//	
//}