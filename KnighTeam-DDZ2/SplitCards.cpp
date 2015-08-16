#include "SplitCards.h"
#include "MoveGenerator.h"

SplitCards::SplitCards()
{

}


SplitCards::~SplitCards()
{
}

SplitCards::SplitCards(unsigned int *cards, vector<CARDSMOVE> moves)
{
	memcpy(split_cards, cards, sizeof(split_cards));
	this->splitMoves = moves;
}

CARDSMOVE SplitCards::GetASplitMove(CARDSMOVE lastMove)
{
	CARDSMOVE splitMove = CARDSMOVE();
	CMoveGenerator ddz_MG = CMoveGenerator();
	vector<CARDSMOVE> moves = ddz_MG.getMovesByOthers(lastMove, split_cards);
	
	if (moves.empty())
	{
		NULL_MOVE(splitMove);
		return splitMove;
	}
	
	int cardsType = lastMove.cardsType;
	
}
