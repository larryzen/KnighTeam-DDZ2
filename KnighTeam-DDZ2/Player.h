#pragma once
#include<vector>
#include "define.h"

using namespace std;

class Player
{
	

public:
	Player(void);
	virtual ~Player(void);

	/*******************玩家p1 ********************/
	static vector<CARDSMOVE> p1_yiChu;  //	  已出的牌
	static vector<CARDSMOVE> p1_general;//	  已出的牌化为常规牌面值并存储牌型
	static int p1_front;									//    上家
	static int p1_next;										//    下家
	static vector<unsigned> p1_cardsList;		//    手中牌的信息
	static int p1_bid;										//    叫分
	static int p1_pos;										//	  玩家位置	
	static int p1_cardsNum;							//	  玩家手中牌的数量
	static bool p1_IsLandlord;						//	  玩家是否为地主
	static unsigned p1_EachCardNum[15];     //    玩家手中3~K、A、2、小王、大王15种牌型数量
	static double p1_Membership[54];			//    隶属度表
	static int p1_manyCards[14];					//    较多的牌
	static int p1_fewCards[14];						//	  较少的牌
	/***********************************************/


	/********************玩家p2 ********************/
	static vector<CARDSMOVE> p2_yiChu;   //	    已出的牌
	static vector<CARDSMOVE> p2_general;//	    已出的牌化为常规牌面值并存储牌型
	static int p2_front;									 //    上家
	static int p2_next;										 //    下家
	static vector<unsigned> p2_cardsList;	     //    手中牌的信息
	static int p2_bid;										 //    叫分
	static int p2_pos;									     //	   玩家位置	
	static int p2_cardsNum;							 //	   玩家手中牌的数量
	static bool p2_IsLandlord;						 //	   玩家是否为地主
	static unsigned p2_EachCardNum[15];      //    玩家手中3~K、A、2、小王、大王15种牌型数量
	static double p2_Membership[54];			 //    隶属度表
	static int p2_manyCards[14];					//     较多的牌
	static int p2_fewCards[14];						//	   较少的牌
	/************************************************/


	/*****************玩家p3,即我方 **************/
	static vector<CARDSMOVE> p3_yiChu;   //	    已出的牌
	static vector<CARDSMOVE> p3_general; //	    已出的牌化为常规牌面值并存储牌型
	static int p3_front;									  //    上家
	static int p3_next;										  //    下家
	static vector<unsigned> p3_cardsList;	      //    手中牌的信息
	static int p3_bid;										  //    叫分
	static int p3_pos;										  //	玩家位置	
	static int p3_cardsNum;						      //	玩家手中牌的数量
	static bool p3_IsLandlord;						  //	玩家是否为地主
	static CardsInfo p3_cardsInfo;
	static unsigned p3_EachCardNum[15];       //    我方手中3~K、A、2、小王、大王15种牌型数量
	static int p3_manyCards[14];					  //     较多的牌
	static int p3_fewCards[14];						  //	 较少的牌
	
	static unsigned remaining[15];							   //     剩余牌型表
	static vector<CARDSMOVE> p3_allMoves;//     将手中牌存在的走步存储起来
	static int p3_IsExistCardsType[14];				//    手中是否存在14种牌型
	static CombsLib p3_combs;							//    组合库
	static CARDSMOVE firstMove;					//    发起出牌玩家
	static int firstPlayer;									   //     每一轮是哪方主动出牌：1->玩家p1;2->玩家p2; 3->我方
	static CARDSMOVE lastMove;					   //     记录上一次非空出牌
	static int lastPlayer;									   //     记录上一次非空出牌玩家
	/**************************************************/

};

