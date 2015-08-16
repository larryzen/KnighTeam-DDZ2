#include "UCTSearch.h"
#include "DDZMoveManager.h"
#include "DDZCombFactory.h"
#include "time.h"
#include "math.h"
#include "FileWriter.h"

UCTSearch::UCTSearch()
{
	uctNodes = vector<CARDSMOVE>();
	parent_times = 0;
	bestMove = CARDSMOVE();
	ddz_MG = CMoveGenerator();
	ddz_Eve = CEveluation();
}


UCTSearch::~UCTSearch()
{
}

int UCTSearch::SearchAGoodMove(int nDepth)
{
	SearchAGoodMove(nDepth, -1);
	int currentIndex = getMaxUCBValue();
	while (currentIndex != -1)
	{
		parent_times+=1;
		uctNodes[currentIndex].current_times+=1;
		dealUctMove(uctNodes[currentIndex], 0, currentIndex);
		
		UCBNodesValue();//计算所有UCT结点值
		
		currentIndex = getMaxUCBValue();
		if (parent_times == UCT_Depth)
		{
			/*int times = 0;
			for (int i = 0; i < uctNodes.size(); i++)
			{
				if (uctNodes[i].current_times > times)
				{
					bestMove = uctNodes[i];
					times = uctNodes[i].current_times;
				}
			}*/
			int win = -1;
			for (int i = 0; i < uctNodes.size(); i++)
			{
				if (uctNodes[i].win > win)
				{
					bestMove = uctNodes[i];
					win = uctNodes[i].win;
				}
			}
			break;
		}
			
	}
	return 0;
}


int UCTSearch::getMaxUCBValue()
{
	int currentIndex = -1;
	double maxUCBValue = -1;
	for (int i = 0; i < uctNodes.size(); i++)
	{
		if (uctNodes[i].UCBValue > maxUCBValue)
		{
			maxUCBValue = uctNodes[i].UCBValue;
			currentIndex = i;
		}
	}

	return currentIndex;
}

//计算所有UCT结点值
void UCTSearch::UCBNodesValue()
{
	for (int i = 0; i < uctNodes.size(); i++)
	{
		uctNodes[i].UCBValue = UCBValue(uctNodes[i].win, uctNodes[i].current_times, parent_times);
	}
}
int UCTSearch::SearchAGoodMove(int nDepth, int  nodeIndex)
{
	int result = IsGameOver(nDepth);
	if (result)
	{
		
		if (result > 0)
		{
			uctNodes[nodeIndex].win += 1;
		}
		return 0;
	}
	
	ddz_Eve.PositiveSimulatedMenu();
	vector<CARDSMOVE> moves = ddz_MG.getMoves(nDepth);
	
	
	if (nDepth == 0)
	{
		uctNodes = moves;
		if (moves.size() == 1 && moves[0].outWay==0)
		{
			bestMove = moves[0];
			return 0;
		}
		for (int i = 0; i < uctNodes.size(); i++)
		{
			uctNodes[i].current_times=0;
			uctNodes[i].win=0;
			uctNodes[i].UCBValue=0;
		}
		parent_times = moves.size();

		for (int i = 0; i < moves.size(); i++)
		{
		
			dealUctMove(moves[i], nDepth, i);
			uctNodes[i].current_times += 1;
		}

		UCBNodesValue();//计算所有UCT结点值
	}
	else
	{
		/*int p1 = Player::p1_cardsNum;
		int p2 = Player::p2_cardsNum;
		int p3 = Player::p3_cardsNum;
		unsigned cpyP1_EachCardNum[TOTAL_KINDS];
		unsigned cpyP2_EachCardNum[TOTAL_KINDS];
		unsigned cpyP3_EachCardNum[TOTAL_KINDS];
		memcpy(cpyP1_EachCardNum, Player::p1_EachCardNum, sizeof(cpyP1_EachCardNum));
		memcpy(cpyP2_EachCardNum, Player::p2_EachCardNum, sizeof(cpyP2_EachCardNum));
		memcpy(cpyP3_EachCardNum, Player::p3_EachCardNum, sizeof(cpyP3_EachCardNum));*/
		
		
		/*cout << moves[randNum].side << "方:"<<nDepth<<endl;
		for (int i = 0; i < moves[randNum].cards.size(); i++)
		{
			cout << moves[randNum].cards[i] << " ";
		}
		cout<<endl;*/
		int randNum = UctRand(moves.size());
		if (randNum != -1)
		{
			dealUctMove(moves[randNum], nDepth, nodeIndex);

		}
		
		
	}
	return 0;
}
void UCTSearch::dealUctMove(CARDSMOVE move, int nDepth, int nodeIndex)
{
	/*double cpyP1_membership[54], cpyP2_membership[54];
	vector<CARDSMOVE> cpy_cardsMoveRecords = vector<CARDSMOVE>();
	unsigned cpyP1_EachCardNum[TOTAL_KINDS];
	unsigned cpyP2_EachCardNum[TOTAL_KINDS];
	unsigned cpyP3_EachCardNum[TOTAL_KINDS];
	int cpyP1_cardsNum,cpyP2_cardsNum,cpyP3_cardsNum;
	int cpy_Remaining[TOTAL_KINDS];*/
	if (nDepth == 0)
	{	
		memcpy(cpyP1_membership, Player::p1_Membership, sizeof(cpyP1_membership));
		memcpy(cpyP2_membership, Player::p2_Membership, sizeof(cpyP2_membership));

		memcpy(cpyP1_EachCardNum, Player::p1_EachCardNum, sizeof(cpyP1_EachCardNum));
		memcpy(cpyP2_EachCardNum, Player::p2_EachCardNum, sizeof(cpyP2_EachCardNum));
		memcpy(cpyP3_EachCardNum, Player::p3_EachCardNum, sizeof(cpyP3_EachCardNum));

		cpyP1_cardsNum = Player::p1_cardsNum;
		cpyP2_cardsNum = Player::p2_cardsNum;
		cpyP3_cardsNum = Player::p3_cardsNum;

		cpy_cardsMoveRecords = Player::cardsMoveRecords;

		memcpy(cpy_Remaining, Player::remaining, sizeof(cpy_Remaining));
	}
	

	MakeMove(move, nDepth);

	ddz_Eve.ClearedByPlayCards(move.cards);
	if (nDepth % 3 != 0)
	{
		ddz_Eve.UpdateByPlayCards(nDepth % 3, move.cardsType, move.cards);
	}
	
	//cout << nDepth<< " "<<nodeIndex << endl;
	SearchAGoodMove(nDepth + 1, nodeIndex);


	//UnMakeMove(move, nDepth);

	if (nDepth == 0)
	{
		memcpy(Player::p1_Membership, cpyP1_membership, sizeof(Player::p1_Membership));
		memcpy(Player::p2_Membership, cpyP2_membership, sizeof(Player::p2_Membership));

		memcpy(Player::p1_EachCardNum, cpyP1_EachCardNum, sizeof(cpyP1_EachCardNum));
		memcpy(Player::p2_EachCardNum, cpyP2_EachCardNum, sizeof(cpyP2_EachCardNum));
		memcpy(Player::p3_EachCardNum, cpyP3_EachCardNum, sizeof(cpyP3_EachCardNum));

		Player::p1_cardsNum = cpyP1_cardsNum;
		Player::p2_cardsNum = cpyP2_cardsNum;
		Player::p3_cardsNum = cpyP3_cardsNum;
		
		Player::cardsMoveRecords = cpy_cardsMoveRecords;

		memcpy(Player::remaining, cpy_Remaining, sizeof(cpy_Remaining));
	}
}

int UCTSearch::UctRand(int max)
{
	srand(time(NULL));
	
	if (max != 0)
	{
		return (int)(rand() % max);
	}
	
	return -1;
}

double UCTSearch::UCBValue(int CURRENT_WIN, int CURRENT_TIMES,int PARENT_TIMES)
{
	return ((double)CURRENT_WIN / CURRENT_TIMES) + UCB_c * sqrt(2 * log(PARENT_TIMES)/CURRENT_TIMES);
}