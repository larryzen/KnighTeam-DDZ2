#include "SplitCards.h"
#include "MoveGenerator.h"
#include "DDZMoveManager.h"

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

CARDSMOVE SplitCards::GetASplitMove(CARDSMOVE lastMove)
{
	CARDSMOVE splitMove = CARDSMOVE();
	CMoveGenerator ddz_MG = CMoveGenerator();
	DDZMoveManager ddz_MM = DDZMoveManager();

	vector<CARDSMOVE> moves = ddz_MG.getMovesByOthers(lastMove, split_cards);
	
	if (moves.empty())
	{
		NULL_MOVE(splitMove);
		return splitMove;
	}
	
	int cardsType = lastMove.cardsType;
	int value = -1; 
	int maxIndex = ddz_MM.getMaximumMoveIndex(moves, cardsType);//排除了火箭、炸弹管普通牌型的情况
	if (maxIndex != -1)
	{
		splitMove = moves[maxIndex];
		value = splitMove.cards[0];
	}

	if (value >= K)
	{
		return splitMove;
	}
	else
	{
		NULL_MOVE(splitMove);
		return splitMove;
	}	
}
