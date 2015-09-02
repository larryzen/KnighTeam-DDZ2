#pragma once
#include "define.h"
#include "Player.h"
#include "DDZPlayer.h"
#include "MoveGenerator.h"
#include "VectorUtil.h"

using namespace std;

class CEveluation
{
public:
	CEveluation();
	~CEveluation();
	void initMembership();
	void ClearedByDealCards();
	void ClearedByLeftCards(vector<unsigned> leftCards);
	void ClearedByPlayCards(vector<unsigned> playCards);
	void UpdateByPlayCards(int whoseGo,int cardsType,vector<unsigned> playCards);

	void UpdateFewCards(int whoseGo);
	void UpdateManyCards(int whoseGo);
	void RefreshRemaining(vector<unsigned> list);


	void NegativeSimulatedMenu();// 悲观模拟随机菜单
	void PositiveSimulatedMenu();//乐观模拟菜单

	int EveluateMove(CARDSMOVE* move,int whoseGo);
	void EveluateMoves(vector<CARDSMOVE> *moves, int whoseGo);

	int Evelute(CARDSMOVE move, int whoseGo);

	int getMinIndexOfCombs(CombsLib combs);
	void cutCarryCards(CARDSMOVE *move);
	void cutCarryCardsMoves( vector<CARDSMOVE> *moves);

	void cutCarryCards1(CARDSMOVE *move,
		vector<unsigned> carryCards,int shouldCarryCards_num,int startIndex,int flag);

	vector<unsigned> getMaxElementIndex(int *arr,unsigned len,int top_num);

	void FilterForMoves(vector<CARDSMOVE> *moves,unsigned *cards);

	int IsPlay3Single(vector<CARDSMOVE> moves, unsigned *cards);
	int IsPlay4Couple(vector<CARDSMOVE> moves, unsigned *cards);
	int IsPlay5Santiao(vector<CARDSMOVE> moves, unsigned *cards);

	int IsAiPass();

	vector<unsigned> haveZhaDanOutside();
	vector<unsigned> haveZhaDanP2();
	vector<unsigned> haveZhaDanP1();
	CARDSMOVE firstAttack(vector<CARDSMOVE> moves);
private:
	void init_p1_p2_EachCardNum();


	void handleMoreThanHalf(int i, int *cardsNum, unsigned *remaining, int who);
	void handleNotMoreThanHalf(int i, double randNum, int *cardsNum, unsigned *remaining, int who);

	void handle1(int value, int *cardsNum, int who);
	void handle2(int value, int i, double randNum, int *cardsNum, unsigned *remaining, int who);
	int GetCardsRightIfPass();
	int IsCurrentTeam(bool current_IsLandlord, int turn);

	inline int getValue(int i);
	inline bool IsCardOriginator(int turn);

	
};