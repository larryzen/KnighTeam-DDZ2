#pragma once
#include "Player.h"
#include "DDZPlayer.h"
#include "set"

using namespace std;
class CMoveGenerator
{
public: 
	CMoveGenerator(void);
	~CMoveGenerator(void);
	vector<CARDSMOVE>  getMoves(int whoseGo);


	// 主动出牌
	vector<CARDSMOVE> getMovesByMyself(unsigned cardsNum,unsigned *EachCardsNum);
	vector<CARDSMOVE> getMovesByCardsNum(unsigned n,unsigned *cards);// 主动出牌

	


	// 被动出牌
	vector<CARDSMOVE> getMovesByOthers(CARDSMOVE lastMove,unsigned *cards);

	


	//vector<vector<int>> getBasicMovesByMyself(Player p,int basicIndex);

	
	

	
	vector<int> subVector(vector<int> v,unsigned start,unsigned end);
	bool IsValidMove(CARDSMOVE m1, CARDSMOVE m2);
private:
	vector<CARDSMOVE> getMovesByCombMovesForOneMove(CARDSMOVE key, vector<CARDSMOVE> moves);
	void FinalMovesDeal(vector<CARDSMOVE> *moves, int turn);
	void GeneralSplitDeal(vector<CARDSMOVE> *moves,  unsigned *EachCardsNum);
	bool HaveSingleJunko(unsigned start, unsigned JunkoNum, unsigned *cards);
	bool HaveDualJunko(unsigned start, unsigned JunkoNum, unsigned *cards);
	bool HaveThreeJunko(unsigned start, unsigned JunkoNum, unsigned *cards);

	
	bool IsValidJunkoMove(vector<unsigned> cardsInfo1, vector<unsigned> cardsInfo2);

	vector<CARDSMOVE> getMovesBy01Card(unsigned *cards);
	vector<CARDSMOVE> getMovesBy02Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy03Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy04Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy05Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy06Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy07Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy08Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy09Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy10Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy11Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy12Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy14Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy15Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy16Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy17Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy18Cards(unsigned *cards);
	vector<CARDSMOVE> getMovesBy20Cards(unsigned *cards);

	vector<CARDSMOVE> getMovesByType02ZhaDan(unsigned ZhaDanValue, unsigned *cards);
	vector<CARDSMOVE> getMovesByType03SingleCard(unsigned SingleCard, unsigned *cards);
	vector<CARDSMOVE> getMovesByType04CoupleCards(unsigned CoupleCards, unsigned *cards);
	vector<CARDSMOVE> getMovesByType05ThreeTiaoCards(unsigned ThreeCards, unsigned *cards);
	vector<CARDSMOVE> getMovesByType06Three_One(unsigned Three_OneCards, unsigned *cards);
	vector<CARDSMOVE> getMovesByType07Three_Two(unsigned Three_TwoCards, unsigned *cards);
	vector<CARDSMOVE> getMovesByType08SingleJunko(unsigned cardBeginValue, unsigned cardEndValue, unsigned *cards);
	vector<CARDSMOVE> getMovesByType09DualJunko(unsigned cardBeginValue, unsigned cardEndValue, unsigned *cards);
	vector<CARDSMOVE> getMovesByType10Three_Shun(unsigned cardBeginValue, unsigned cardEndValue, unsigned *cards);
	vector<CARDSMOVE> getMovesByType11Three_Shun_One(unsigned cardBeginValue, unsigned cardEndValue, unsigned *cards);
	vector<CARDSMOVE> getMovesByType12Three_Shun_Couple(unsigned cardBeginValue, unsigned cardEndValue, unsigned *cards);
	vector<CARDSMOVE> getMovesByType13Four_Two(unsigned Four_TwoValue, unsigned *cards);
	vector<CARDSMOVE> getMovesByType14Four_TwoCouple(unsigned Four_TwoCoupleValue, unsigned *cards);


	bool IsSplit(int turn);
};