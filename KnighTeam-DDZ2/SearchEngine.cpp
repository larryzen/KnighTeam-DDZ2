#include "SearchEngine.h"
#include<vector>
#include"define.h"
#include "time.h"

using namespace std;

CSearchEngine::CSearchEngine(void)
{
	
}


CSearchEngine::~CSearchEngine(void)
{
	
}


/** �ж��ƾ��Ƿ����
**  @ nDepthΪ�������
**  ��Ϊ�ҷ�ʤ�����ؼ���ֵ����֮�򷵻ؼ�Сֵ���ƾ�δ�������� 0
**  (�ر�ע�ͣ��˳���Ĭ�������ҷ�Ϊ���p3)
*/
int CSearchEngine::IsGameOver(int nDepth)
{
	int p1_cardsNum = Player::p1_cardsNum;
	int p2_cardsNum = Player::p2_cardsNum;
	int p3_cardsNum = Player::p3_cardsNum;

	if(p3_cardsNum==0)// �ҷ������ƣ�ֱ�ӷ��ؼ���ֵ
	{
		return 1;// ���ؼ���ֵ
	}

	if(Player::p3_IsLandlord==true)// �ҷ�Ϊ����
	{
		if(p1_cardsNum==0||p2_cardsNum==0)// ���p1,p2����������
		{
			return -1;// ���ؼ�Сֵ
		}
	}
	else                     // �ҷ���Ϊ����
	{
		if(p1_cardsNum==0)// ���p1����������
		{
			if(Player::p1_IsLandlord)// ���p1Ϊ����
			{
				return -1;// ���ؼ�Сֵ
			}
			else
			{
				return 1;// ���ؼ���ֵ
			}
		}

		if(p2_cardsNum==0)// ���p2����������
		{
			if(Player::p2_IsLandlord)// ���p2Ϊ����
			{
				return -1;// ���ؼ�Сֵ
			}
			else
			{
				return 1;// ���ؼ���ֵ
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
