#pragma once
#include "define.h"

/**
*    多目标规划类
*    根据不同的目标生成不同的组合
*/
class DDZCombFactory
{
public:
	DDZCombFactory(void);
	DDZCombFactory(unsigned cards[15], int cardsNum);
	~DDZCombFactory(void);
	void getCombLeastSingle(unsigned *cards, int *cardsNum);
	bool flag[15][15];
	CombsLib getCombs();
	Comb getComb1LeastSingle();		// 找到最少单牌的组合
	Comb getComb2LeastMoves();		// 找到最少组合步数的组合
	Comb getComb3MaxGain();			// 找到收益最大的组合
	Comb getComb4Final();
	Comb getCombByCardsType(CARDSMOVE key);
	int getIndexOfSecondMinSingle(CombsLib combsLib);
	CombsLib combs;
	Comb comb;
	void setCarryCards1ForSantiao(vector<CARDSMOVE> *moves);
	void setCarryCards1_1ForSantiao(vector<CARDSMOVE> *moves);
	void setCarryCards1_2ForSantiao(vector<CARDSMOVE> *moves);
	void setCarryCards1_3ForSantiao(vector<CARDSMOVE> *moves, int topCard);

	void setCarryCards3ForThreeJunko(vector<CARDSMOVE> *moves);
	void setCarryCards3_1ForThreeJunko(vector<CARDSMOVE> *moves);
	void setCarryCards3_2ForThreeJunko(vector<CARDSMOVE> *moves);

	void setCarryCards4(vector<CARDSMOVE> *moves);
	
	void setCarryCards5ForZhaDan(vector<CARDSMOVE> *moves);


	static bool IsCorrect(Comb comb, int cardsNum);
	static bool IsContainsCardsMove(vector<CARDSMOVE> cms, CARDSMOVE cm);
	static bool IsSameCardsMove(CARDSMOVE cm1, CARDSMOVE cm2);
	static bool IsSameComb(Comb c1, Comb c2);
	static bool IsContainsComb(CombsLib combs, Comb comb);

	static void quickSort(unsigned *arr, int first, int last, int len);
	private:
		unsigned comb_cards[15];
		int onHandCardsNum;
		void addComb();
		void removeMove(vector<CARDSMOVE> *moves, int start, int end);
		void removeMove(vector<CARDSMOVE> *moves, int index);


		void canMakeByJunko(unsigned *cards, int start, int end, int *cardsNum, int nJunko);
		void filters(unsigned *cards, int *cardsNum);
		bool IsRelateToJunko(unsigned *cards, int value, int cardsNum);
		

		int getLeastJunkoNum(int nJunko);
		int getCardsTypeByNJunko(int nJunko);
		int getCardsTypeByBasicCards(int cardsNum);
		void getBasicTypeComb(unsigned *cards, int start, int end, int cardsNum);

		

		
		
		void setSingleNum(Comb *comb);
		void setSantiaoNum(Comb *comb);
		void setDualJunkoNum(Comb *comb);
		void setGain(Comb *cb);

		void setGain2(Comb *cb);
		void setSingleNum2(Comb *cb);

		int compareSingle(CARDSMOVE single1, CARDSMOVE single2);
		int compareSingleNum(Comb cb1, Comb cb2);
		int compareMovesNum(Comb cb1, Comb cb2);
		int compareGain(Comb cb1, Comb cb2);
		int compare(int n1, int n2);
};

