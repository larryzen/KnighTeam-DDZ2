#pragma once
#include "define.h"
class SplitCards
{
public:
	SplitCards();
	~SplitCards();
	SplitCards(unsigned int *cards);

	CARDSMOVE GetASplitMove(CARDSMOVE lastMove, bool IsMustPlay);
private:
	unsigned int split_cards[15];
	vector<CARDSMOVE> splitMoves;
};

