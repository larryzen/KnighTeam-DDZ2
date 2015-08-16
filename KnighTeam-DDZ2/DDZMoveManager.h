#pragma once
#include "define.h"
#include "Player.h"

class DDZMoveManager
{
public:
	DDZMoveManager(void);
	~DDZMoveManager(void);

	vector<CARDSMOVE> getType1Rocket(vector<CARDSMOVE> moves);

	vector<CARDSMOVE> getType2Zhadan(vector<CARDSMOVE> moves);

	vector<CARDSMOVE> getType3Single(vector<CARDSMOVE> moves);

	vector<CARDSMOVE> getType4Couple(vector<CARDSMOVE> moves);

	vector<CARDSMOVE> getType5Santiao(vector<CARDSMOVE> moves);

	vector<CARDSMOVE> getType6Three_One(vector<CARDSMOVE> moves);

	vector<CARDSMOVE> getType7Three_Two(vector<CARDSMOVE> moves);

	vector<CARDSMOVE> getType8SingleJunko(vector<CARDSMOVE> moves);

	vector<CARDSMOVE> getType9DualJunko(vector<CARDSMOVE> moves);

	vector<CARDSMOVE> getType10ThreeJunko(vector<CARDSMOVE> moves);

	vector<CARDSMOVE> getType11ThreeJunko_One(vector<CARDSMOVE> moves);

	vector<CARDSMOVE> getType12ThreeJunko_Two(vector<CARDSMOVE> moves);

	vector<CARDSMOVE> getType13Four_Two(vector<CARDSMOVE> moves);

	vector<CARDSMOVE> getType14Four_Two_Couple(vector<CARDSMOVE> moves);


	CARDSMOVE getMiniumMove(vector<CARDSMOVE> cms, int CardsType);

	void setStatus(CARDSMOVE *move);
	void setMovesStatus(vector<CARDSMOVE> *moves);
	int getMoveValue(CARDSMOVE move);
	bool canPlayOver(vector<CARDSMOVE> comb);
	bool IsEqualsMove(CARDSMOVE m1, CARDSMOVE m2);

	int IsRelateMove(CARDSMOVE move,int n);
	int IsRelateMoves(vector<CARDSMOVE> moves,int n);



	vector<CARDSMOVE> dealMoves(vector<CARDSMOVE> moves,unsigned *cards);
	int getMaxScoreMove(vector<CARDSMOVE> moves);
	vector<CARDSMOVE> getCombBySocre();

	vector<CARDSMOVE> getGoodMove3Single(vector<CARDSMOVE> moves,
												vector<CARDSMOVE> single,unsigned *cards,int flag);

	vector<CARDSMOVE> getGoodMove4Couple(vector<CARDSMOVE> moves,
												vector<CARDSMOVE> couple,unsigned *cards,int flag);

	vector<CARDSMOVE> getGoodMove5Santiao(vector<CARDSMOVE> moves,
												vector<CARDSMOVE> couple,unsigned *cards,int flag);
private:
	vector<CARDSMOVE> getMovesByType(vector<CARDSMOVE> moves,int type);
	void JunkoFilter(vector<CARDSMOVE> *singleJunko, vector<CARDSMOVE> *dualJunko, vector<CARDSMOVE> *threeJunko,
		 CARDSMOVE *singleJunkoMove, CARDSMOVE *dualJunkoMove,  CARDSMOVE *threeJunkoMove);
	void removeCardMove(vector<CARDSMOVE> *moves, int index);
};

