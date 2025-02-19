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

	void initPlayer();		// 一局完初始化玩家信息,清空上一局比赛记录
	void setFirstAndLastPlayer();	// 每次到我方出牌更新主动出牌方

	int getCardsType(VECTORINT vector);//获取出的牌的类型，如类型为炸弹、顺子等
	VECTORINT getCardsValue(VECTORINT vector);
	int getValue(int value);	// 根据牌的下标获得牌面值
	void sortVectorLowToHigh(Player p,int low,int high);// 选择排序一手出牌

	bool IsRocket(VECTORINT vi);				//是否为 火箭		并且存值
	bool IsZhaDan(VECTORINT vi);			//是否为 炸弹		并且存值
	bool ISingleCard(VECTORINT vi);			//是否为 单牌		并且存值
	bool IsCoupleCards(VECTORINT vi);	//是否为 对牌		并且存值
	bool IsThreeCards(VECTORINT vi);		//是否为 三条		并且存值
	bool IsSingleJunko(VECTORINT vi);		//是否为 单顺		并且存值
	bool IsDualJunko(VECTORINT vi);		//是否为 双顺		并且存值
	bool IsThree_Shun(VECTORINT vi);		//是否为 三顺		并且存值
	bool IsThree_One(VECTORINT vi);		//是否为 三带一		并且存值
	bool IsThree_Two(VECTORINT vi);		//是否为 三带二		并且存值
	bool IsFour_Two(VECTORINT vi);			//是否为 四带二单牌  并且存值
	bool IsFour_TwoCouple(VECTORINT vi);	//是否为 四带二对牌  并且存值
	

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

	VECTORINT sortCardsVector(VECTORINT vector);// 选择排序
	VECTORINT CardsToIndex(VECTORINT v);

	StringUtil *stringUtil;
	
};

