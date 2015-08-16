#pragma once
#include "vector"
#include "StringUtil.h"
#include "ThinkTable.h"
#include "Player.h"
#include "Eveluation.h"
#include "SearchEngine.h"

using namespace std;

class DDZPlayer
{
public:
	DDZPlayer(void);
	virtual ~DDZPlayer(void);
	void CalName(char *cInMessage, char *cOutMessage);
	void CalInfo(char *cInMessage, char *cOutMessage);
	void CalGam(char *cInMessage, char *cOutMessage);
	void CalErr(char *cInMessage, char *cOutMessage);

	void CalDeal(char *cInMessage, char *cOutMessage);
	void CalBid(char *cInMessage, char *cOutMessage);
	void CalLeft(char *cInMessage, char *cOutMessage);
	void CalPla(char *cInMessage, char *cOutMessage);

	void initPlayer();		// һ�����ʼ�������Ϣ,�����һ�ֱ�����¼
	void setFirstAndLastPlayer();	// ÿ�ε��ҷ����Ƹ����������Ʒ�

	int getCardsType(VECTORINT vector);//��ȡ�����Ƶ����ͣ�������Ϊը����˳�ӵ�
	VECTORINT getCardsValue(VECTORINT vector);
	int getValue(int value);	// �����Ƶ��±�������ֵ
	void sortVectorLowToHigh(Player p,int low,int high);// ѡ������һ�ֳ���

	bool IsRocket(VECTORINT vi);				//�Ƿ�Ϊ ���		���Ҵ�ֵ
	bool IsZhaDan(VECTORINT vi);			//�Ƿ�Ϊ ը��		���Ҵ�ֵ
	bool ISingleCard(VECTORINT vi);			//�Ƿ�Ϊ ����		���Ҵ�ֵ
	bool IsCoupleCards(VECTORINT vi);	//�Ƿ�Ϊ ����		���Ҵ�ֵ
	bool IsThreeCards(VECTORINT vi);		//�Ƿ�Ϊ ����		���Ҵ�ֵ
	bool IsSingleJunko(VECTORINT vi);		//�Ƿ�Ϊ ��˳		���Ҵ�ֵ
	bool IsDualJunko(VECTORINT vi);		//�Ƿ�Ϊ ˫˳		���Ҵ�ֵ
	bool IsThree_Shun(VECTORINT vi);		//�Ƿ�Ϊ ��˳		���Ҵ�ֵ
	bool IsThree_One(VECTORINT vi);		//�Ƿ�Ϊ ����һ		���Ҵ�ֵ
	bool IsThree_Two(VECTORINT vi);		//�Ƿ�Ϊ ������		���Ҵ�ֵ
	bool IsFour_Two(VECTORINT vi);			//�Ƿ�Ϊ �Ĵ�������  ���Ҵ�ֵ
	bool IsFour_TwoCouple(VECTORINT vi);	//�Ƿ�Ϊ �Ĵ�������  ���Ҵ�ֵ
	

	unsigned getSingleValue(vector<unsigned> vector);
	unsigned getCoupleValue(vector<unsigned> vector);
	unsigned getZhaDanValue(VECTORINT vector);
	unsigned getSanTiaoValue(vector<unsigned> vector);
	VECTORINT getSingleJunkoValue(VECTORINT vector);
    VECTORINT getDualJunkoValue(VECTORINT vector);
	VECTORINT getThree_ShunValue(VECTORINT vector);
	VECTORINT getThree_OneValue(VECTORINT vector);
	VECTORINT getThree_TwoValue(VECTORINT vector);
	VECTORINT getFour_TwoValue(VECTORINT vector);
	VECTORINT getFour_TwoCoupleValue(VECTORINT vector);

	VECTORINT sortCardsVector(VECTORINT vector);// ѡ������
	VECTORINT CardsToIndex(VECTORINT v);

	StringUtil *stringUtil;
	
};

