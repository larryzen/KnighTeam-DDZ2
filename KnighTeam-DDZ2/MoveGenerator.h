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
	vector<CARDSMOVE>  getMoves(Player p,int whoseGo);


	// 主动出牌
	vector<CARDSMOVE> getMovesByMyself(unsigned cardsNum,unsigned *EachCardsNum);
	vector<CARDSMOVE> getMovesByCardsNum(unsigned n,unsigned *cards);// 主动出牌

	vector<CARDSMOVE> getMovesByOneCard(unsigned *cards);
	vector<CARDSMOVE> getMovesByTwoCards(unsigned *cards);
	vector<CARDSMOVE> getMovesByThreeCards(unsigned *cards);
	vector<CARDSMOVE> getMovesByFourCards(unsigned *cards);
	vector<CARDSMOVE> getMovesByFiveCards(unsigned *cards);
	vector<CARDSMOVE> getMovesBySixCards(unsigned *cards);
	vector<CARDSMOVE> getMovesBySevenCards(unsigned *cards);
	vector<CARDSMOVE> getMovesByEightCards(unsigned *cards);
	vector<CARDSMOVE> getMovesByNineCards(unsigned *cards);
	vector<CARDSMOVE> getMovesByTenCards(unsigned *cards);
	vector<CARDSMOVE> getMovesByElevenCards(unsigned *cards);
	vector<CARDSMOVE> getMovesByTwelveCards(unsigned *cards);
	vector<CARDSMOVE> getMovesByForteenCards(unsigned *cards);
	vector<CARDSMOVE> getMovesByFifteenCards(unsigned *cards);
	vector<CARDSMOVE> getMovesBySixteenCards(unsigned *cards);
	vector<CARDSMOVE> getMovesBySeventeenCards(unsigned *cards);
	vector<CARDSMOVE> getMovesByEighteenCards(unsigned *cards);
	vector<CARDSMOVE> getMovesByTwentyCards(unsigned *cards);


	// 被动出牌
	vector<CARDSMOVE> getMovesByOthers(CARDSMOVE lastMove,unsigned *cards);

	vector<CARDSMOVE> getMovesByZhaDan(unsigned ZhaDanValue,unsigned *cards);
	vector<CARDSMOVE> getMovesBySingleCard(unsigned SingleCard,unsigned *cards);
	vector<CARDSMOVE> getMovesByCoupleCards(unsigned CoupleCards,unsigned *cards);
	vector<CARDSMOVE> getMovesByThreeTiaoCards(unsigned ThreeCards,unsigned *cards);
	vector<CARDSMOVE> getMovesByThree_One(unsigned Three_OneCards,unsigned *cards);
	vector<CARDSMOVE> getMovesByThree_Two(unsigned Three_TwoCards,unsigned *cards);
	vector<CARDSMOVE> getMovesBySingleJunko(unsigned cardBeginValue,unsigned cardEndValue,unsigned *cards);
	vector<CARDSMOVE> getMovesByDualJunko(unsigned cardBeginValue,unsigned cardEndValue,unsigned *cards);
	vector<CARDSMOVE> getMovesByThree_Shun(unsigned cardBeginValue,unsigned cardEndValue,unsigned *cards);
	vector<CARDSMOVE> getMovesByThree_Shun_One(unsigned cardBeginValue,unsigned cardEndValue,unsigned *cards);
	vector<CARDSMOVE> getMovesByThree_Shun_Couple(unsigned cardBeginValue,unsigned cardEndValue,unsigned *cards);
	vector<CARDSMOVE> getMovesByFour_Two(unsigned Four_TwoValue,unsigned *cards);
	vector<CARDSMOVE> getMovesByFour_TwoCouple(unsigned Four_TwoCoupleValue,unsigned *cards);


	//vector<vector<int>> getBasicMovesByMyself(Player p,int basicIndex);

	bool HaveSingleJunko(unsigned start,unsigned JunkoNum,unsigned *cards);
	bool HaveDualJunko(unsigned start,unsigned JunkoNum,unsigned *cards);
	bool HaveThreeJunko(unsigned start,unsigned JunkoNum,unsigned *cards);
	vector<unsigned> combinateCards(vector<unsigned> vector);// 组合算法

	bool IsValidMove(CARDSMOVE m1,CARDSMOVE m2);
	bool IsValidJunkoMove(vector<unsigned> cardsInfo1,vector<unsigned> cardsInfo2);
	vector<int> subVector(vector<int> v,unsigned start,unsigned end);
	void  PriorityCombinatoriaLibrary(Player p);
	Comb comb(Player p,vector<unsigned> v_cardsTypes);
	void combByCardsType(Player p,Comb *comb,int cardsType);
	void initP3_IsExistCardsType(Player p);
	int lastWhoseGo;

};