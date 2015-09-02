#include "SearchEngine.h"
#include<vector>
#include"define.h"
#include "time.h"
#include "DDZMoveManager.h"

using namespace std;

CSearchEngine::CSearchEngine(void)
{
	
}


CSearchEngine::~CSearchEngine(void)
{
	
}


/** �ж��ƾ��Ƿ����
**  @ nDepthΪ�������
**  ��Ϊ�ҷ�ʤ�����ؼ���ֵ����֮�򷵻ؼ�Сֵ���ƾ�δ�������� 0,����������
**  (�ر�ע�ͣ��˳���Ĭ�������ҷ�Ϊ���p3)
*/
int CSearchEngine::IsGameOver(int nDepth)
{
	int p1_cardsNum = Player::p1_cardsNum;
	int p2_cardsNum = Player::p2_cardsNum;
	int p3_cardsNum = Player::p3_cardsNum;
	int gainValue = 0;
	if(p3_cardsNum==0)// �ҷ������ƣ�ֱ�ӷ��ؼ���ֵ
	{
		gainValue = GetAllGain(0);
		return 1 * gainValue;// ���ؼ���ֵ
	}

	if(Player::p3_IsLandlord==true)// �ҷ�Ϊ����
	{
		if(p1_cardsNum==0||p2_cardsNum==0)// ���p1,p2����������
		{
			gainValue = GetAllGain(1);
			return -1 * gainValue;// ���ؼ�Сֵ
		}
	}
	else                     // �ҷ���Ϊ����
	{
		if(p1_cardsNum==0)// ���p1����������
		{
			if(Player::p1_IsLandlord)// ���p1Ϊ����
			{
				gainValue = GetAllGain(1);
				return -1 * gainValue;// ���ؼ�Сֵ
			}
			else
			{
				gainValue = GetAllGain(1);
				return 1 * gainValue;// ���ؼ���ֵ
			}
		}

		if(p2_cardsNum==0)// ���p2����������
		{
			if(Player::p2_IsLandlord)// ���p2Ϊ����
			{
				gainValue = GetAllGain(2);
				return -1 * gainValue;// ���ؼ�Сֵ
			}
			else
			{
				gainValue = GetAllGain(2);
				return 1 * gainValue;// ���ؼ���ֵ
			}
		}
	}


	return 0;
}



/** ���ݳ����Ƴ����е���
*   @ move ���� @ whoseGo �ֵ�˭����
*   whoseGo��ֵ���� ������4��3��2��1. �ҷ�ֵ%3==0
*/
void CSearchEngine::MakeMove(CARDSMOVE move,int whoseGo)
{
	Player::cardsMoveRecords.push_back(move);

	 if(whoseGo % 3 == 0)   // �ҷ�
	 {
		 if(move.cardsType!=PASS)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
					Player::p3_EachCardNum[move.cards.at(i)]--;
			 }
		 Player::p3_cardsNum-=(move.cards.size());		// ��ȥ��������
		 }
	 }
	 else if(whoseGo % 3 == 1 )  // ���p1�����ҷ��¼�
	 {
		 if(move.cardsType != PASS)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
				 Player::p1_EachCardNum[move.cards.at(i)]--;
				 Player::remaining[move.cards[i]]--;
			 }

			 Player::p1_cardsNum-=(move.cards.size());		// ��ȥ��������
		 }
	 }
	 else					// ���p2�����ҷ��ϼ�
	 {
		  if(move.cardsType != PASS)
		  {
				 for(size_t i=0;i<move.cards.size();i++)
				 {
					 Player::p2_EachCardNum[move.cards.at(i)]--;
					 Player::remaining[move.cards[i]]--;
				 }

				 Player::p2_cardsNum-=(move.cards.size());		// ��ȥ��������
		  }
	 }

	 
}

void CSearchEngine::UnMakeMove(CARDSMOVE move,int whoseGo)
{
	Player::cardsMoveRecords.erase(Player::cardsMoveRecords.end()-1);
	
	if(whoseGo % 3 == 0)  // �ҷ�
	 {
		 if(move.cardsType != PASS)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
				 Player::p3_EachCardNum[move.cards.at(i)]++;
			 }
	 
			 Player::p3_cardsNum+=(move.cards.size());		// �ָ���������
		 }
	 }
	 else if(whoseGo % 3 == 1)  // ���p1�����ҷ��¼�
	 {
		 if(move.cardsType != PASS)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
				 Player::p1_EachCardNum[move.cards.at(i)]++;
				 Player::remaining[move.cards[i]]++;
			 }

			 Player::p1_cardsNum+=(move.cards.size());		// �ָ���������
		 }
	 }
	 else					// ���p2�����ҷ��ϼ�
	 {
		 if(move.cardsType != PASS)
		 {
			 for(size_t i=0;i<move.cards.size();i++)
			 {
				 Player::p2_EachCardNum[move.cards.at(i)]++;
				 Player::remaining[move.cards[i]]++;
			 }

			 Player::p2_cardsNum+=(move.cards.size());		    // �ָ���������
		 }
	 }
}

int CSearchEngine::gain()
{
	int gain = 1;
	for (int i = Player::cardsMoveRecords.size() - 1; i >= 0; i--)
	{
		if (Player::cardsMoveRecords[i].cardsType == ROCKET ||
					 Player::cardsMoveRecords[i].cardsType == ZHADAN)
		{
			gain++;
		}
	}

	return gain;
}


int CSearchEngine::GetAllGain(int turn)
{
	return gain() + Spring(turn) + ReSpring(turn);
}

bool CSearchEngine::CanWin(vector<CARDSMOVE> FirstMoves, int outWay)
{
	DDZMoveManager ddz_MM = DDZMoveManager();
	if (outWay)
	{
		ddz_MM.setMovesStatus(&FirstMoves);
		if (ddz_MM.canPlayOver(FirstMoves))
		{
			int size = FirstMoves.size();
			if (size == 1)
			{
				bestMove = FirstMoves[0];	
			}
			else
			{
				for (int i = 0; i < size; i++)
				{
					if (FirstMoves[i].status == STATUS_MAX)
						bestMove = FirstMoves[i];
				}
			}
			return true;
		}
	}

	return false;
}

/**
*  �Ƿ��Ǵ�����߷�����
*/
int  CSearchEngine::Spring(int turn)
{
	int LandlordTurn = -1;
	if (!Player::cardsMoveRecords.empty())
	{
		LandlordTurn = Player::cardsMoveRecords[0].side;
	}
	else
	{
		return 0;
	}
	for (int i = Player::cardsMoveRecords.size() - 1; i >= 0; i--)
	{
		if (Player::cardsMoveRecords[i].side != LandlordTurn)
		{
			if (Player::cardsMoveRecords[i].cardsType != PASS)
				return 0;
		}
	}

	if (turn == LandlordTurn)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

/**
*	�Ƿ�Ϊ������
*/
int CSearchEngine::ReSpring(int turn)
{
	int LandlordTurn = -1;
	if (!Player::cardsMoveRecords.empty())
	{
		LandlordTurn = Player::cardsMoveRecords[0].side;
	}
	else
	{
		return 0;
	}

	for (int i = Player::cardsMoveRecords.size() - 1; i >= 1; i--)//����ֻ
	{
		if (Player::cardsMoveRecords[i].side == LandlordTurn)
		{
			if (Player::cardsMoveRecords[i].cardsType != PASS)
				return 0;
		}
	}

	if (turn != LandlordTurn)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}