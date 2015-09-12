#include "SearchEngine.h"
#include<vector>
#include"define.h"
#include "time.h"
#include "DDZMoveManager.h"
#include "FileWriter.h"
#include "DDZCombFactory.h"

using namespace std;

CSearchEngine::CSearchEngine(void)
{
	ddz_MM = DDZMoveManager();
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

/**
*	�������Ʊ�ʤ��ֻ����ը���������
*/
bool CSearchEngine::CanWin2(vector<CARDSMOVE> moves)
{
	ddz_MM.setMovesStatus(&moves);
	int maxMovesNum = 0;
	vector<CARDSMOVE> maxMoves = vector<CARDSMOVE>();
	for (int i = 0; i < moves.size(); i++)
	{
		if (moves[i].status == STATUS_MAX && (moves[i].cardsType == ZHADAN || moves[i].cardsType==ROCKET))
		{
			maxMovesNum++;
			maxMoves.push_back(moves[i]);
		}
	}

	if (maxMovesNum >= (moves.size() - maxMovesNum))
	{
		for (int k = 0; k < maxMoves.size(); k++)
		{
			bool canPlay = CMoveGenerator::IsValidMove(Player::lastMove, maxMoves[k]);
			if (canPlay)
			{
				bestMove = maxMoves[k];
				return true;
			}
		}
	}

	return false;
}

/*
*	���������£�����22(MAX)������(MAX)��66��J��A���ﵽ��ʤ״̬
*   maxsize >= (othersize - 1)
*/
bool CSearchEngine::CanWin3(vector<CARDSMOVE> moves)
{
	ddz_MM.setMovesStatus(&moves);
	vector<CARDSMOVE> maxMoves = vector<CARDSMOVE>();//��statusΪSTATUS_MAX���߲��洢
	vector<CARDSMOVE> otherMoves = vector<CARDSMOVE>();
	for (int i = moves.size() - 1; i >= 0; i--)
	{
		if (IsMaxOnlyOne(moves[i]))
		{
			maxMoves.push_back(moves[i]);
		}
		else
		{
			otherMoves.push_back(moves[i]);
		}
	}
	vector<CARDSMOVE> oneToOne = vector<CARDSMOVE>();
	if (maxMoves.empty())//û��max�߲�
	{
		return false;
	}
	if (maxMoves.size() < otherMoves.size() - 1)
	{
		return false;
	}
	else
	{
		CARDSMOVE goodMove = CARDSMOVE();
		goodMove.cardsType = PASS;
		for (int k = maxMoves.size() - 1; k >= 0; k--)
		{
			CARDSMOVE maxMove = maxMoves[k];
			if (maxMove.cardsType == ZHADAN || maxMove.cardsType == ROCKET)
			{
				CARDSMOVE m = CARDSMOVE();
				m.cardsType = PASS;
				oneToOne.push_back(m);//���Ϊը��������Ȳ�ѡ���Ӧ��С��
			}
			else
			{
				for (int t = otherMoves.size() - 1; t >= 0; t--)
				{
					if (CMoveGenerator::IsValidMove(otherMoves[t], maxMove))
					{
						goodMove = otherMoves[t];
						oneToOne.push_back(otherMoves[t]);
						vector<CARDSMOVE>::iterator it = otherMoves.begin();
						otherMoves.erase(it+t);
					}
				}
			}
			
		}

		for (int i = oneToOne.size() - 1; i >= 0; i--)
		{
			if (oneToOne[i].cardsType == PASS)
			{
				if (otherMoves.size() > 0)
				{
					oneToOne[i] = otherMoves[0];
					otherMoves.erase(otherMoves.begin());
				}
			}
		}

		if (otherMoves.size() <= 1)
		{
			if (goodMove.cardsType != PASS)
			{
				bestMove = goodMove;//����ը��ӦС����Ϊ�߲�
				return true;
			}
			else 
			{
				for (int tmp = 0; tmp < oneToOne.size(); tmp++)
				{
					if (oneToOne[tmp].cardsType != PASS)
					{
						bestMove = oneToOne[tmp];// û�з�ը��ӦС�ƣ�ѡһ��ը��ӦС��
						return true;
					}		
				}

				bestMove = maxMoves[0];//û��С�ƣ�ѡһ��������Ϊ�߲�
				return true;
				
			}
			
		}
		
		return false;
	}

	return false;
}

/**
*	�������Ʊ�ʤ��7777�����ƣ���3��������Max)��66��22��Max)��J
*	maxsize >= othersize
*/
bool CSearchEngine::CanWin4(vector<CARDSMOVE> moves)
{
	ddz_MM.setMovesStatus(&moves);
	vector<CARDSMOVE> maxMoves = vector<CARDSMOVE>();//��statusΪSTATUS_MAX���߲��洢
	vector<CARDSMOVE> otherMoves = vector<CARDSMOVE>();//���г�ȥstatus_Max�Ժ����
	vector<CARDSMOVE> canPlayMoves = vector<CARDSMOVE>();//�ܹ���������ҳ����Ƶ�status_Max����
	vector<CARDSMOVE> oneToOne = vector<CARDSMOVE>();
	for (int i = moves.size() - 1; i >= 0; i--)
	{
		if (moves[i].status == STATUS_MAX)
		{
			maxMoves.push_back(moves[i]);
		}
		else
		{
			otherMoves.push_back(moves[i]);
		}
	}

	


	if (maxMoves.size() < otherMoves.size() )
	{
		return false;
	}
	else
	{
		CARDSMOVE goodMove = CARDSMOVE();
		goodMove.cardsType = PASS;
		for (int i = maxMoves.size() - 1; i >= 0; i--)
		{
			if (CMoveGenerator::IsValidMove(Player::lastMove, maxMoves[i]))
			{
				goodMove = maxMoves[i];
				canPlayMoves.push_back(maxMoves[i]);
			}
		}
		if (canPlayMoves.empty())//���д��Ʋ��ܹ���������ҳ��ƣ�����false��
		{
			return false;
		}
		
		for (int i = canPlayMoves.size() - 1; i >= 0; i--)
		{
			for (int k = maxMoves.size() - 1; k >= 0; k--)
			{
				CARDSMOVE maxMove = maxMoves[k];
				if (DDZCombFactory::IsSameCardsMove(canPlayMoves[i], maxMoves[k]))
				{
					CARDSMOVE m = CARDSMOVE();
					m.cardsType = -2;//���Ϊ��������ҵ��ƣ��Ȳ�ѡ���Ӧ��С�ƣ�cardsType=-2
					oneToOne.push_back(m);
					continue;
				}
				if (maxMove.cardsType == ZHADAN || maxMove.cardsType == ROCKET)
				{
					CARDSMOVE m = CARDSMOVE();
					m.cardsType = PASS;//���Ϊը��������Ȳ�ѡ���Ӧ��С�ƣ�cardsType=Pass
					oneToOne.push_back(m);
				}
				else
				{
					for (int t = otherMoves.size() - 1; t >= 0; t--)
					{
						if (CMoveGenerator::IsValidMove(otherMoves[t], maxMove))
						{
							goodMove = otherMoves[t];
							oneToOne.push_back(otherMoves[t]);
							vector<CARDSMOVE>::iterator it = otherMoves.begin();
							otherMoves.erase(it + t);
						}
					}
				}

				for (int n = oneToOne.size() - 1; n >= 0; n--)
				{
					if (oneToOne[n].cardsType == PASS)
					{
						if (otherMoves.size() > 0)
						{
							oneToOne[n] = otherMoves[0];
							otherMoves.erase(otherMoves.begin());
						}
					}
				}

				if (otherMoves.size() <= 1)
				{
					bestMove = canPlayMoves[i];
					return true;
				}

				return false;
			}
		}
		
		return false;
	}
}

/*
*
*	�߲���ǰ����ƣ����ڶ�������û�д���,Ψһ�����
*/
bool CSearchEngine::IsMaxOnlyOne(CARDSMOVE move)
{
	bool onlyOne = true;
	/*if (move.cardsType == SINGLE || move.cardsType == COUPLE)
	{
		unsigned cards[15] = { 0 };
		for (int i = move.cards.size() - 1; i >= 0; i--)
		{
			cards[move.cards[i]]++;
		}

		for (int j = 0; j < 15; j++)
		{
			if (cards[j] != 0)
			{
				if (Player::remaining[j] >= cards[j])
				{
					onlyOne = false;
				}
			}
		}
	}*/
	
	return move.status == STATUS_MAX && onlyOne;
}
bool CSearchEngine::CanWin(vector<CARDSMOVE> FirstMoves, int outWay)
{
	if (outWay==1 || outWay == 2)
	{
		ddz_MM.setMovesStatus(&FirstMoves);
		if (ddz_MM.canPlayOver(FirstMoves))
		{
			int size = FirstMoves.size();
			if (size == 1 && outWay == 1)
			{
				bestMove = FirstMoves[0];	
			}
			else
			{
				for (int i = 0; i < size; i++)
				{
					if (FirstMoves[i].status == STATUS_MAX && outWay == 1)
					{
						bestMove = FirstMoves[i];
						bestMove.side = 0;
					}
						
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