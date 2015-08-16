#pragma once
#include "SearchEngine.h"
class UCTSearch :
	public CSearchEngine
{
public:
	UCTSearch();
	~UCTSearch();
	int SearchAGoodMove(int nDepth);
	
private:
		vector<CARDSMOVE> uctNodes;
		int UctRand(int max);
		void dealUctMove(CARDSMOVE move, int nDepth, int nodeIndex);
		int SearchAGoodMove(int nDepth, int  nodeIndex);
		double UCBValue(int CURRENT_WIN, int CURRENT_TIMES, int PARENT_TIMES);
		int parent_times;
		int getMaxUCBValue();
		void UCBNodesValue();
		CMoveGenerator ddz_MG;
		CEveluation ddz_Eve;


		double cpyP1_membership[54], cpyP2_membership[54];
		vector<CARDSMOVE> cpy_cardsMoveRecords = vector<CARDSMOVE>();
		unsigned cpyP1_EachCardNum[TOTAL_KINDS];
		unsigned cpyP2_EachCardNum[TOTAL_KINDS];
		unsigned cpyP3_EachCardNum[TOTAL_KINDS];
		int cpyP1_cardsNum, cpyP2_cardsNum, cpyP3_cardsNum;
		int cpy_Remaining[TOTAL_KINDS];
};

