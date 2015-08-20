#pragma once
#include "define.h"

/**
*    ��Ŀ��滮��
*    ���ݲ�ͬ��Ŀ�����ɲ�ͬ�����
*/
class DDZCombFactory
{
public:
	DDZCombFactory(void);
	DDZCombFactory(unsigned cards[15], int cardsNum);
	~DDZCombFactory(void);
	void getCombLeastSingle(unsigned *cards, int *cardsNum);
	
	CombsLib getCombs();
	Comb getComb1LeastSingle();		// �ҵ����ٵ��Ƶ����
	Comb getComb2LeastMoves();		// �ҵ�������ϲ��������
	Comb getComb3MaxGain();			// �ҵ������������
	Comb getCombByCardsType(CARDSMOVE key);
	int getIndexOfSecondMinSingle(CombsLib combsLib);
	CombsLib combs;
	Comb comb;
	void setCarryCards1(vector<CARDSMOVE> *moves);
	void setCarryCards2(vector<CARDSMOVE> *moves);
	void setCarryCards3(vector<CARDSMOVE> *moves);
	void setCarryCards4(vector<CARDSMOVE> *moves);
	private:
		bool flag[13][13];//�Ż���˳�ݹ���ң���̬�滮��˼��
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

		bool IsCorrect(Comb comb, int cardsNum);
		bool IsContainsCardsMove(vector<CARDSMOVE> cms, CARDSMOVE cm);
		bool IsSameCardsMove(CARDSMOVE cm1, CARDSMOVE cm2);
		bool IsSameComb(Comb c1, Comb c2);
		bool IsContainsComb(CombsLib combs, Comb comb);

		void quickSort(int *arr, int first, int last, int len);
		
		void setSingleNum(Comb *comb);
		void setGain(Comb *cb);

		int compareSingle(CARDSMOVE single1, CARDSMOVE single2);
		int compareSingleNum(Comb cb1, Comb cb2);
		int compareMovesNum(Comb cb1, Comb cb2);
		int compareGain(Comb cb1, Comb cb2);
		int compare(int n1, int n2);
};

