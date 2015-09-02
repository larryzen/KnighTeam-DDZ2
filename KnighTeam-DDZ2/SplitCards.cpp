#include "SplitCards.h"
#include "MoveGenerator.h"
#include "DDZMoveManager.h"
#include "DDZCombFactory.h"
#include "VectorUtil.h"

SplitCards::SplitCards()
{

}


SplitCards::~SplitCards()
{
}

SplitCards::SplitCards(unsigned int *cards)
{
	memcpy(split_cards, cards, sizeof(split_cards));
}

CARDSMOVE SplitCards::GetASplitMove(CARDSMOVE lastMove, bool IsMustPlay)
{
	CARDSMOVE splitMove = CARDSMOVE();
	CMoveGenerator ddz_MG = CMoveGenerator();
	DDZMoveManager ddz_MM = DDZMoveManager();
	DDZCombFactory ddz_CF = DDZCombFactory(split_cards, Player::p3_cardsNum);
	vector<CARDSMOVE> comb_moves = ddz_CF.getComb1LeastSingle().moves;
	vector<CARDSMOVE> moves = ddz_MG.getMovesByOthers(lastMove, split_cards);

	int cardsType = lastMove.cardsType;
	
	
	
	NULL_MOVE(splitMove);
	

	if (moves.empty())
	{
		splitMove.side = 0;
		return splitMove;
	}
	
	
	int value = -1; 
	int maxIndex = ddz_MM.getMaximumMoveIndex(moves, cardsType);//排除了火箭、炸弹管普通牌型的情况
	if (maxIndex != -1)
	{
		if (cardsType == SINGLE && moves[maxIndex].cards[0] >= X)//排除掉有2,去拆火箭的情况
		{
			if (split_cards[X] == 1 && split_cards[D] == 1
					&& split_cards[T] > 0 && T > lastMove.cards[0])//若2能管上，则打2
			{
				splitMove.cards.push_back(T);
				splitMove.cardsType = SINGLE;
				value = T;
			}
		}
		else
		{
			splitMove = moves[maxIndex];////?????
			value = splitMove.cards[0];
		}
		
	}

	if (((cardsType==SINGLE || cardsType ==COUPLE) && (value >= K && value <=A 
			&& split_cards[value] != 4)|| value ==T) || IsMustPlay)
	{
		splitMove.side = 0;
		return splitMove;
	}
	else
	{

		switch (cardsType)
		{
			case SINGLE:
			{
						   for (int i = comb_moves.size() - 1; i >= 0; i--)
						   {

						   }
						   break;
			}
			case COUPLE:
			{
						   for (int i = comb_moves.size() - 1; i >= 0; i--)
						   {
							   if (comb_moves[i].cardsType == DUALJUNKO)
							   {
								   CARDSMOVE currentMove = comb_moves[i];
								   if (currentMove.cards[0] + currentMove.cards.size() > lastMove.cards[0] && currentMove.cards[currentMove.cards.size()-1]>=Q)
								   {
									   splitMove.cards = VectorUtil::subVector(currentMove.cards.size() - 2, currentMove.cards.size() - 1, comb_moves[i].cards);
									   splitMove.cardsType = COUPLE;
									   splitMove.side = 0;
									   return splitMove;
								   }
							   }
						   }
						break;
			}
			case SINGLEJUNKO:
			{
				for (int i = comb_moves.size() - 1; i >= 0; i--)
				{
					if (comb_moves[i].cardsType == SINGLEJUNKO && comb_moves[i].cards[0] > lastMove.cards[0]
							&& comb_moves[i].cards.size()>lastMove.cards.size()
							&& (comb_moves[i].cards.size() - lastMove.cards.size()<3 || comb_moves[i].cards.size() - lastMove.cards.size()>=5))
					{
						splitMove.cards = VectorUtil::subVector(0, lastMove.cards.size()-1,comb_moves[i].cards);
						splitMove.cardsType=SINGLEJUNKO;
						splitMove.side = 0;
						return splitMove;
					}
				}
				break;
			}
			default:
				break;
		}
		NULL_MOVE(splitMove);
		splitMove.side = 0;
		return splitMove;
	}	
}
