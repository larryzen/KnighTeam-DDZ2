#include "DDZPlayer.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "PublicVar.h"
#include "string"
#include "iostream"
#include "vector"
#include "ThinkTable.h"
#include "MoveGenerator.h"
#include "time.h"
#include "fstream"
#include "MCSearch.h"
#include "AlphaBeta.h"


using namespace std;

typedef vector<unsigned> VECTORINT;
typedef vector<vector<unsigned>> MOVES;


DDZPlayer::DDZPlayer(void)
{
}


DDZPlayer::~DDZPlayer(void)
{
}



/**     收到平台命令：DOUDIZHUVER 1.0					*/
/**	    回复：Name cqut									*/
void DDZPlayer::CalName(char *cInMessage,char *D_ver,char *cOutMessage)
{
	strcpy_s(cOutMessage,80,"Name cqut");
}


/**		收到平台命令：INFO 1,4,1,6,9,2100,15				*/
/**	    回复：OK INFO									*/
void DDZPlayer::CalInfo(char *cInMessage,char *cOutMessage)
{
	strcpy_s(cOutMessage,80,"OK INFO");
}


/**		收到平台命令：ERROR A(B/C)						*/
/**	    回复：OK ERROR									*/
void DDZPlayer::CalErr(char *cInMessage,char *cOutMessage)
{
	strcpy_s(cOutMessage,80,"OK ERROR");
}


/**		收到平台命令：DEAL B0,4,5,7,9,10,17,21,25,		*/
/**     33,34,39,41,43,44,45,46							*/
/**	    回复：OK DEAL									*/
void DDZPlayer::CalDeal(char *cInMessage,char *cOutMessage,Player p)
{
	CEveluation *eveluate;
	eveluate =(CEveluation*)malloc(sizeof(CEveluation));

	// 设置默认玩家p3为我方，玩家p2为我方上家，玩家p1为我方下家
	p.p3_pos=cInMessage[5]-'A';			// 设置我方位置  东/西/南
	
	/** 初始化玩家位置信息 */
	int pos3 = p.p3_pos;
	switch(pos3)
	{
	case 0:
		p.p1_pos=1;
		p.p2_pos=2;

		p.p1_front =0;
		p.p1_next = 2;

		p.p2_front=1;
		p.p2_next =0;

		p.p3_front=2;
		p.p3_next=1;
		break;
	case 1:
		p.p1_pos=2;
		p.p2_pos=0;

		p.p1_front =1;
		p.p1_next = 0;

		p.p2_front=2;
		p.p2_next =1;

		p.p3_front=0;
		p.p3_next=2;
		break;
	case 2:
		p.p1_pos=0;
		p.p2_pos=1;

		p.p1_front =2;
		p.p1_next = 1;

		p.p2_front=0;
		p.p2_next =2;

		p.p3_front=1;
		p.p3_next=0;
		break;
	}

	string deal = string(cInMessage);

	string subDeal(deal.begin()+6,deal.end()); // 获取裁判所发的17张牌

	p.p3_cardsList=stringUtil->stringSplit(subDeal,',');
	

	eveluate->initMembership(p);    // 初始化玩家隶属度表
	eveluate->ClearedByDealCards(p);// 将其他玩家持有牌 为 我方持有牌隶属度清零
	eveluate->RefreshRemaining(p,p.p3_cardsList);//更新剩余牌型表

	delete(eveluate);//释放指针内存
	strcpy_s(cOutMessage,80,"OK DEAL");
}


/**		收到平台命令：BID A3	/ BID WHAT					*/
/**	    回复：OK BID	   /  BID B3[(A/C)(1/2)]			*/
void DDZPlayer::CalBid(char *cInMessage,char *cOutMessage,Player p)
{
	CThinkTable think;
	char callBid[80];
	strcpy_s(callBid,80,"BID A0");
	callBid[4]+=p.p3_pos;// 获得位置
	callBid[5]+=3;//think.getBid(p);// 叫分
	

	switch(cInMessage[4])
	{
	case 'A':
		if(p.p1_pos==0)
		{
			p.p1_bid=cInMessage[5]-'0';
		}
		else
		{
			p.p2_bid=cInMessage[5]-'0';
		}
		strcpy_s(cOutMessage,80,"OK BID");
		break;
	case 'B':
		if(p.p1_pos==1)
		{
			p.p1_bid=cInMessage[5]-'0';
		}
		else
		{
			p.p2_bid=cInMessage[5]-'0';
		}

		strcpy_s(cOutMessage,80,"OK BID");
		break;
	case 'C':
		if(p.p1_pos==2)
		{
			p.p1_bid=cInMessage[5]-'0';
		}
		else
		{
			p.p2_bid=cInMessage[5]-'0';
		}
		strcpy_s(cOutMessage,80,"OK BID");
		break;
	case 'W':
		strcpy_s(cOutMessage,80,callBid);
		break;
	}
	
}


/**		收到平台命令：LEFTOVER B27,48,53					*/
/**	    回复：OK LEFTOVER								*/
void DDZPlayer::CalLeft(char *cInMessage,char *cOutMessage,Player p)
{
	CEveluation *eveluate;
	eveluate =(CEveluation*)malloc(sizeof(CEveluation));
	CMoveGenerator *m_pMG;
	m_pMG=(CMoveGenerator*)malloc(sizeof(m_pMG));

	CThinkTable *think;
	think=(CThinkTable*)malloc(sizeof(CThinkTable));
	string tmp_str(cInMessage);
	string str2(tmp_str.begin()+10,tmp_str.end());
	vector<unsigned> list =stringUtil->stringSplit(str2,',');// 获得3张底牌

	switch(cInMessage[9])
	{
	case 'A':
		if(p.p1_pos==0)
		{
			p.p1_IsLandlord=true;
			p.p1_cardsNum+=3;
			p.firstPlayer=1;
		}
		else if(p.p2_pos==0)
		{
			p.p2_IsLandlord=true;
			p.p2_cardsNum+=3;
			p.firstPlayer=2;
		}
		else
		{
			p.p3_IsLandlord=true;
			p.p3_cardsNum+=3;
			p.firstPlayer=3;

			for(size_t i=0;i<list.size();i++)
			{
				p.p3_cardsList.push_back(list.at(i));
			}
		}
		break;
	case 'B':
		if(p.p1_pos==1)
		{
			p.p1_IsLandlord=true;
			p.p1_cardsNum+=3;
			p.firstPlayer=1;
		}
		else if(p.p2_pos==1)
		{
			p.p2_IsLandlord=true;
			p.p2_cardsNum+=3;
			p.firstPlayer=2;
		}
		else
		{
			p.p3_IsLandlord=true;
			p.p3_cardsNum+=3;
			p.firstPlayer=3;

			for(size_t i=0;i<list.size();i++)
			{
				p.p3_cardsList.push_back(list.at(i));
			}
		}
		break;
	case 'C':
		if(p.p1_pos==2)
		{
			p.p1_IsLandlord=true;
			p.p1_cardsNum+=3;
			p.firstPlayer=1;

			
		}
		else if(p.p2_pos==2)
		{
			p.p2_IsLandlord=true;
			p.p2_cardsNum+=3;
			p.firstPlayer=2;
		}
		else
		{
			p.p3_IsLandlord=true;
			p.p3_cardsNum+=3;
			p.firstPlayer=3;

			for(size_t i=0;i<list.size();i++)
			{
				p.p3_cardsList.push_back(list.at(i));
			}
		}
		break;
		
	}
	
	think->initEachCardNum(p);	// 初始化p3_EachCardNum
	p.p3_allMoves=m_pMG->getMovesByMyself(p.p3_cardsList.size(),p.p3_EachCardNum);
	m_pMG->PriorityCombinatoriaLibrary(p);
	eveluate->ClearedByLeftCards(p,list); // 根据底牌更新隶属度表
	delete(think);
	delete(eveluate);//释放指针内存
	strcpy_s(cOutMessage,80,"OK LEFTOVER");
}


/**		收到平台命令：PLAY A12，13，14，21 /  PLAY WHAT	*/
/**	    回复：OK PLAY		/    PLAY B31，32，33，0		*/
void DDZPlayer:: CalPla(char *cInMessage,char *cOutMessage,Player p)
{
	CEveluation *eveluate; 
	eveluate =(CEveluation*)malloc(sizeof(CEveluation));
	char callPass[20];
	char callPla[200];
	CMoveGenerator *move;
	move = (CMoveGenerator*)malloc(sizeof(CMoveGenerator));

	strcpy_s(callPass,20,"PLAY A-1");
	strcpy_s(callPla,200,"PLAY A");

	callPass[5]+=p.p3_pos;
	callPla[5]+=p.p3_pos;

	string tmp_str(cInMessage);
	
	switch(cInMessage[5])
	{
	case 'A':
		if(cInMessage[6]=='-')
		{
			CARDSMOVE null_move;
			NULL_MOVE(null_move);
			if(p.p1_pos==0)
			{
				eveluate->UpdateFewCards(1,p);
				p.p1_yiChu.push_back(null_move);
				p.p1_general.push_back(null_move);
				p.p3_allMoves=move->getMovesByMyself(p.p3_cardsList.size(),p.p3_EachCardNum);
				move->PriorityCombinatoriaLibrary(p);
			}
			else
			{
				eveluate->UpdateFewCards(2,p);
				p.p2_yiChu.push_back(null_move);
				p.p2_general.push_back(null_move);
			}
		}
		else
		{
			string subPla(tmp_str.begin()+6,tmp_str.end());
			vector<unsigned> list = stringUtil->stringSplit(subPla,',');

			vector<unsigned> generalCards =getCardsValue(sortCardsVector(list));// 排好序，化为一般牌面值
			int cardType = getCardsType(generalCards);// 获得出牌类型
			//generalCards.push_back(cardType);

			CARDSMOVE g_move,y_move;
			g_move.cards=generalCards;
			g_move.cardsType=cardType;

			y_move.cards=list;
			y_move.cardsType=cardType;


			eveluate->ClearedByPlayCards(p,list);//根据出牌更新玩家隶属度表
			eveluate->RefreshRemaining(p,list);// 更新剩余牌型表

			if(p.p1_pos==0)
			{
				if(p.firstPlayer==1)
				{
					p.p1_manyCards[cardType]=1;
				}
				p.p1_cardsNum-=list.size();// 玩家出一次牌，手中牌=原手中牌-出牌数量
				p.p1_yiChu.push_back(y_move);
				p.p1_general.push_back(g_move);
				p.p3_allMoves=move->getMovesByMyself(p.p3_cardsList.size(),p.p3_EachCardNum);
				move->PriorityCombinatoriaLibrary(p);
			}
			else
			{
				if(p.firstPlayer==2)
				{
					p.p2_manyCards[cardType]=1;
				}
				p.p2_cardsNum-=list.size();// 玩家出一次牌，手中牌=原手中牌-出牌数量
				p.p2_yiChu.push_back(y_move);
				p.p2_general.push_back(g_move);
			}
		}
		delete(eveluate);//释放eveluate指针内存
		delete(move);	 //释放move指针内存
		strcpy_s(cOutMessage,80,"OK PLAY");
		break;
	case 'B':
		if(cInMessage[6]=='-')
		{
			CARDSMOVE null_move;
			NULL_MOVE(null_move);
			if(p.p1_pos==1)
			{
				eveluate->UpdateFewCards(1,p);
				p.p1_yiChu.push_back(null_move);
				p.p1_general.push_back(null_move);
				p.p3_allMoves=move->getMovesByMyself(p.p3_cardsList.size(),p.p3_EachCardNum);
				move->PriorityCombinatoriaLibrary(p);
			}
			else
			{
				eveluate->UpdateFewCards(2,p);
				p.p2_yiChu.push_back(null_move);
				p.p2_general.push_back(null_move);
			}
		}
		else
		{
			string subPla(tmp_str.begin()+6,tmp_str.end());
			vector<unsigned> list = stringUtil->stringSplit(subPla,',');

			vector<unsigned> generalCards =getCardsValue(sortCardsVector(list));// 排好序，化为一般牌面值
			int cardType = getCardsType(generalCards);// 获得出牌类型
			//generalCards.push_back(cardType);

			CARDSMOVE g_move,y_move;
			g_move.cards=generalCards;
			g_move.cardsType=cardType;

			y_move.cards=list;
			y_move.cardsType=cardType;

			eveluate->ClearedByPlayCards(p,list);//根据出牌更新玩家隶属度表
			eveluate->RefreshRemaining(p,list);// 更新剩余牌型表

			if(p.p1_pos==1)
			{
				if(p.firstPlayer==1)
				{
					p.p1_manyCards[cardType]=1;
				}
				p.p1_cardsNum-=list.size();// 玩家出一次牌，手中牌=原手中牌-出牌数量
				p.p1_yiChu.push_back(y_move);
				p.p1_general.push_back(g_move);
				p.p3_allMoves=move->getMovesByMyself(p.p3_cardsList.size(),p.p3_EachCardNum);
				move->PriorityCombinatoriaLibrary(p);
			}
			else
			{
				if(p.firstPlayer==2)
				{
					p.p2_manyCards[cardType]=1;
				}
				p.p2_cardsNum-=list.size();// 玩家出一次牌，手中牌=原手中牌-出牌数量
				p.p2_yiChu.push_back(y_move);
				p.p2_general.push_back(g_move);
			}
		}
		delete(eveluate);//释放指针内存

		delete(move);	 //释放move指针内存
		strcpy_s(cOutMessage,80,"OK PLAY");
		break;
	case 'C':
		if(cInMessage[6]=='-')
		{
			CARDSMOVE null_move;
			NULL_MOVE(null_move);
			if(p.p1_pos==2)
			{
				eveluate->UpdateFewCards(1,p);
				p.p1_yiChu.push_back(null_move);
				p.p1_general.push_back(null_move);

				p.p3_allMoves=move->getMovesByMyself(p.p3_cardsList.size(),p.p3_EachCardNum);
				move->PriorityCombinatoriaLibrary(p);
			}
			else
			{
				eveluate->UpdateFewCards(2,p);
				p.p2_yiChu.push_back(null_move);
				p.p2_general.push_back(null_move);
			}
		}
		else
		{
			string subPla(tmp_str.begin()+6,tmp_str.end());
			vector<unsigned> list = stringUtil->stringSplit(subPla,',');

			vector<unsigned> generalCards =getCardsValue(sortCardsVector(list));// 排好序，化为一般牌面值
			int cardType = getCardsType(generalCards);// 获得出牌类型

			CARDSMOVE g_move,y_move;
			g_move.cards=generalCards;
			g_move.cardsType=cardType;

			y_move.cards=list;
			y_move.cardsType=cardType;

			eveluate->ClearedByPlayCards(p,list);//根据出牌更新玩家隶属度表
			eveluate->RefreshRemaining(p,list);// 更新剩余牌型表

			if(p.p1_pos==2)
			{
				if(p.firstPlayer==1)
				{
					p.p1_manyCards[cardType]=1;
				}
				p.p1_cardsNum-=list.size();// 玩家出一次牌，手中牌=原手中牌-出牌数量
				p.p1_yiChu.push_back(y_move);
				p.p1_general.push_back(g_move);
				p.p3_allMoves=move->getMovesByMyself(p.p3_cardsList.size(),p.p3_EachCardNum);
				move->PriorityCombinatoriaLibrary(p);
			}
			else
			{
				if(p.firstPlayer==2)
				{
					p.p2_manyCards[cardType]=1;
				}
				p.p2_cardsNum-=list.size();// 玩家出一次牌，手中牌=原手中牌-出牌数量
				p.p2_yiChu.push_back(y_move);
				p.p2_general.push_back(g_move);
			}
		}
		delete(eveluate);//释放指针内存
		delete(move);	 //释放move指针内存
		strcpy_s(cOutMessage,80,"OK PLAY");
		break;
	case 'W':
		{
			setFirstPlayer(p);
			CThinkTable *think;
			think=(CThinkTable*)malloc(sizeof(CThinkTable));
			
			VECTORINT tmp_vector1;
			CARDSMOVE bestMove;
			AlphaBeta m_cSE =  AlphaBeta();
			
			m_cSE.SearchAGoodMove(p,AlphaBeta_Depth);//获得最佳走步
			bestMove=m_cSE.bestMove;
			/*MCSearch *m_cSE;
			m_cSE=(MCSearch*)malloc(sizeof(MCSearch));
			bestMove=m_cSE->SearchAGoodMove(p);*/

			
			p.p3_general.push_back(bestMove);

			if(!bestMove.cards.empty()&&bestMove.cardsType!=-1)
			{		
				tmp_vector1=CardsToIndex(bestMove.cards,p);
				Update_P3_EachCardsNum(p,bestMove.cards);


				p.p3_cardsNum-=tmp_vector1.size();		// 玩家出一次牌，手中牌=原手中牌-出牌数量
				p.p3_yiChu.push_back(bestMove);				// 将已出的牌存入p3_yiChu中
				string str2=string(callPla);
				for(size_t i=0;i<tmp_vector1.size();i++)
				{
					if(i==0)
					{
						str2+=stringUtil->getString(tmp_vector1.at(i));
					}
					else
					{
					str2+=(","+stringUtil->getString(tmp_vector1.at(i)));
					}
				}

				strncpy(cOutMessage,str2.c_str(),str2.length());
				cOutMessage[str2.length()]='\0';
			}
			else 
			{
				strcpy_s(cOutMessage,200,callPass);
			}
		//delete(m_cSE);
		think->initEachCardNum(p);	// 初始化p3_EachCardNum
		delete(think);
		delete(eveluate);//释放指针内存
		delete(move);	 //释放move指针内存
		break;
		}
	}
}




/**		收到平台命令：GAMEOVER C							*/
/**	    回复：OK GAMEOVER								    */
void DDZPlayer::CalGam(char *cInMessage,char *cOutMessage)
{
	strcpy_s(cOutMessage,80,"OK GAMEOVER");
}


void DDZPlayer::initCardsInfo(Player p)
{

	cardsInfo.a.num=0;
	cardsInfo.b.num=0;
	cardsInfo.c.num=0;
	cardsInfo.d.num=0;
	cardsInfo.e.num=0;
	cardsInfo.f.num=0;
	cardsInfo.g.num=0;
	cardsInfo.h.num=0;
	cardsInfo.i.num=0;
	cardsInfo.j.num=0;
	cardsInfo.k.num=0;
	cardsInfo.l.num=0;
	cardsInfo.m.num=0;
	cardsInfo.n.num=0;

	cardsInfo.a.value=3;
	cardsInfo.b.value=4;
	cardsInfo.c.value=5;
	cardsInfo.d.value=6;
	cardsInfo.e.value=7;
	cardsInfo.f.value=8;
	cardsInfo.g.value=9;
	cardsInfo.h.value=10;
	cardsInfo.i.value=11;
	cardsInfo.j.value=12;
	cardsInfo.k.value=13;
	cardsInfo.l.value=14;
	cardsInfo.m.value=15;
	cardsInfo.n.value=16;

	cardsInfo.a.meihua=0;
	cardsInfo.b.meihua=0;
	cardsInfo.c.meihua=0;
	cardsInfo.d.meihua=0;
	cardsInfo.e.meihua=0;
	cardsInfo.f.meihua=0;
	cardsInfo.g.meihua=0;
	cardsInfo.h.meihua=0;
	cardsInfo.i.meihua=0;
	cardsInfo.j.meihua=0;
	cardsInfo.k.meihua=0;
	cardsInfo.l.meihua=0;
	cardsInfo.m.meihua=0;

	cardsInfo.a.fangkuai=0;
	cardsInfo.b.fangkuai=0;
	cardsInfo.c.fangkuai=0;
	cardsInfo.d.fangkuai=0;
	cardsInfo.e.fangkuai=0;
	cardsInfo.f.fangkuai=0;
	cardsInfo.g.fangkuai=0;
	cardsInfo.h.fangkuai=0;
	cardsInfo.i.fangkuai=0;
	cardsInfo.j.fangkuai=0;
	cardsInfo.k.fangkuai=0;
	cardsInfo.l.fangkuai=0;
	cardsInfo.m.fangkuai=0;

	cardsInfo.a.hongtao=0;
	cardsInfo.b.hongtao=0;
	cardsInfo.c.hongtao=0;
	cardsInfo.d.hongtao=0;
	cardsInfo.e.hongtao=0;
	cardsInfo.f.hongtao=0;
	cardsInfo.g.hongtao=0;
	cardsInfo.h.hongtao=0;
	cardsInfo.i.hongtao=0;
	cardsInfo.j.hongtao=0;
	cardsInfo.k.hongtao=0;
	cardsInfo.l.hongtao=0;
	cardsInfo.m.hongtao=0;

	cardsInfo.a.heitao=0;
	cardsInfo.b.heitao=0;
	cardsInfo.c.heitao=0;
	cardsInfo.d.heitao=0;
	cardsInfo.e.heitao=0;
	cardsInfo.f.heitao=0;
	cardsInfo.g.heitao=0;
	cardsInfo.h.heitao=0;
	cardsInfo.i.heitao=0;
	cardsInfo.j.heitao=0;
	cardsInfo.k.heitao=0;
	cardsInfo.l.heitao=0;
	cardsInfo.m.heitao=0;


	cardsInfo.n.small=0;
	cardsInfo.n.big=0;

}

/**
*   一局牌完重新初始化玩家信息，继续下一局
*/
void DDZPlayer::initPlayer(Player p)
{
	p.p1_pos=-1;

	p.p1_IsLandlord=false;

	p.p1_cardsNum=17;

	p.p1_bid=0;

	p.p1_front=-1;

	p.p1_next=-1;

	p.p1_cardsList.clear();

	p.p1_yiChu.clear();

	p.p1_general.clear();



	p.p2_pos=-1;

	p.p2_IsLandlord=false;

	p.p2_cardsNum=17;

	p.p2_bid=0;

	p.p2_front=-1;

	p.p2_next=-1;

	p.p2_cardsList.clear();

	p.p2_yiChu.clear();

	p.p2_general.clear();



	p.p3_pos=-1;

	p.p3_IsLandlord=false;

	p.p3_cardsNum=17;

	p.p3_bid=0;

	p.p3_front=-1;

	p.p3_next=-1;

	p.p3_cardsList.clear();

	p.p3_yiChu.clear();

	p.p3_cardsInfo;

	p.firstPlayer=-1;

	p.p3_general.clear();

	for(size_t i=0;i<15;i++)//初始化棋子剩余表
	{
		if(i>=13)
		{
			p.remaining[i]=1;
		}
		else
		{
			p.remaining[i]=4;
		}
	}

}

/**
*  根据各玩家已出牌,更新主动出牌玩家
*/
void DDZPlayer::setFirstPlayer(Player p)
{
	if(p.p1_yiChu.empty()||p.p2_yiChu.empty()||p.p3_yiChu.empty())
	{
		return ;
	}
	else 
	{
		CARDSMOVE p1_lastMove = p.p1_yiChu.at(p.p1_yiChu.size()-1);
		CARDSMOVE p2_lastMove = p.p2_yiChu.at(p.p2_yiChu.size()-1);
		CARDSMOVE p3_lastMove = p.p3_yiChu.at(p.p3_yiChu.size()-1);


		if(p1_lastMove.cardsType==-1&&p2_lastMove.cardsType==-1)
		{
			p.firstPlayer=3;
			return;
		}

		if(p1_lastMove.cardsType==-1&&p3_lastMove.cardsType==-1)
		{
			p.firstPlayer=2;
			return;
		}

		if(p2_lastMove.cardsType==-1&&p3_lastMove.cardsType==-1)
		{
			p.firstPlayer=1;
			return;
		}
	}
}

/**     0	 弃权             1	  火箭		  */
/** 	 2	 炸弹             3	  单牌		   */
/** 	 4	 对牌             5	  三条		   */
/**		 6	 三带单          7   三带双      */
/**      8	 单顺		      9   双顺         */
/** 	10 三顺		     11  三顺带单   */
/** 	12 三顺带双	                            */
/**     13	四带二单   14  四带二双	 */
/** 	-1	  非法牌型                */
int DDZPlayer::getCardsType(VECTORINT vector)// vector为已转化的牌型并排序，如1、2、3、4转化为3个牌3带个牌4
{
	int cardsNum = vector.size();
	switch(cardsNum)
	{
	case 1:
		if(vector.at(0)==-1)
		{
			return PASS;// 玩家pass
		}
		else if(ISingleCard(vector))
		{
			return SINGLE;
		}
		else
		{
			return INVALID;
		}
		break;

	case 2:
		if(IsRocket(vector))
		{
			return ROCKET;
				
		}
		else if(IsCoupleCards(vector))
		{
			return COUPLE; 
		}
		else
		{
			return INVALID;  // 非法牌型
		}
		break;

	case 3:
		if(IsThreeCards(vector))
			return SANTIAO;
		else
			return INVALID;
		break;

	case 4:
		if(IsZhaDan(vector))
		{
			return ZHADAN;
		}
		else if(IsThree_One(vector))
		{
			return THREE_ONE; 
		}
		else 
			return INVALID;
		break;
	case 5:
		if(IsSingleJunko(vector))
		{
			return SINGLEJUNKO; 
		}
		else if(IsThree_Two(vector))
		{
			return THREE_TWO; 
		}
		else
			return INVALID;
		break;

	case 6:
		if(IsSingleJunko(vector))
		{
			return SINGLEJUNKO; 
		}
		else if(IsDualJunko(vector))
		{
			return DUALJUNKO;
		}
		else if(IsThree_Shun(vector))
		{
			return THREEJUNKO; 
		}
		else if(IsFour_Two(vector))
		{
			return FOUR_TWO; 
		}
		else
			return INVALID;
		break;

	case 7:
		if(IsSingleJunko(vector))
		{
			return SINGLEJUNKO;
		}
		else
			return INVALID;
		break;

	case 8:
		if(IsSingleJunko(vector))
		{
			return SINGLEJUNKO; 
		}
		else if(IsDualJunko(vector))
		{
			return DUALJUNKO; 
		}
		else if(IsThree_One(vector))
		{
			return THREEJUNKO_ONE; 
		}
		else if(IsFour_Two(vector))
		{
			return FOUR_TWO_COUPLE; 
		}
		else
			return INVALID;
		break;

	case 9:
		if(IsSingleJunko(vector))
		{
			return SINGLEJUNKO; 
		}
		else if(IsThree_Shun(vector))
		{
			return THREEJUNKO;
		}
		break;

	case 10:
		if(IsSingleJunko(vector))
		{
			return SINGLEJUNKO;
		}
		else if(IsDualJunko(vector))
		{
			return DUALJUNKO; 
		}
		else if(IsThree_Two(vector))
		{
			return THREEJUNKO_TWO; 
		}
		else
			return INVALID;
		break;

	case 11:
		if(IsSingleJunko(vector))
		{
			return SINGLEJUNKO; 
		}
		else 
			return INVALID;
		break;

	case 12:
		if(IsSingleJunko(vector))
		{
			return SINGLEJUNKO;
		}
		else if(IsDualJunko(vector))
		{
			return DUALJUNKO; 
		}
		else if(IsThree_Shun(vector))
		{
			return THREEJUNKO; 
		}
		else if(IsThree_One(vector))
		{
			return THREEJUNKO_ONE;
		}
		else
			return INVALID;
		break;

	case 13:

		return INVALID;
		break;

	case 14:
		if(IsDualJunko(vector))
		{
			return DUALJUNKO; 
		}
		else
			return INVALID;
		break;

	case 15:
		if(IsThree_Shun(vector))
		{
			return THREEJUNKO;
		}
		else if(IsThree_Two(vector))
		{
			return THREEJUNKO_TWO; 
		}
		else
			return INVALID;
		break;

	case 16:
		if(IsDualJunko(vector))
		{
			return DUALJUNKO;
		}
		else if(IsThree_One(vector))
		{
			return THREEJUNKO_ONE; 
		}
		break;

	case 17:
		return INVALID;// 没有一次出17张的牌型
		break;

	case 18:
		if(IsDualJunko(vector))
		{
			return DUALJUNKO; 
		}
		else if(IsThree_Shun(vector))
		{
			return THREEJUNKO;
		}
		else
		{
			return INVALID;
		}
		break;

	case 19:
		return INVALID;// 没有一次出19张的牌型
		break;

	case 20:
		return INVALID;// 一次出20张要么非法，要么玩家已经走完手中牌
		break;
	}	

	return INVALID;
}

/**  根据裁判的下标返回牌面值，如 0 -> 梅花3，则返回牌面值为0 */
int DDZPlayer::getValue(int value)
{
	if(value<=52)
	{
		return value/4;
	}
	else
	{
		return 14;// 大王
	}
}

/**将下标转化为牌面值，如4,7转化为一对4/4=1,7/4=1,即1和1, */
vector<unsigned> DDZPlayer::getCardsValue(vector<unsigned> vector)
{
	VECTORINT cardsList;
	for(size_t i=0;i<vector.size();i++)
	{
		int tmp_card=vector.at(i);

		cardsList.push_back(getValue(tmp_card));
	}

	return cardsList;
}

///** 快速排序 p.p3_cardsList 升序 */
//void DDZPlayer::sortVectorLowToHigh(Player p,int low,int high)
//{
//	if(low>=high)
//	{
//		return;
//	}
//
//	int first=low;
//	int last =high;
//
//	int key=p.p3_cardsList.at(first);
//	while(first<last)
//	{
//		while(first<last&&p.p3_cardsList.at(last)>=key)
//			--last;
//		p.p3_cardsList.at(first)=p.p3_cardsList.at(last);
//		while(first<last&&p.p3_cardsList.at(first)<=key)
//			++first;
//		p.p3_cardsList.at(last)=p.p3_cardsList.at(first);
//	}
//	p.p3_cardsList.at(first)=key;
//	sortVectorLowToHigh(p,low,first-1);
//	sortVectorLowToHigh(p,last+1,high);
//}

bool DDZPlayer::IsRocket(VECTORINT vector)
{
	bool flag = false;
	if(vector.size()==2&&vector[0]==13&&vector[1]==14)
	{
		flag = true;
	}

	return flag;
}

/** 判断vector是否为炸弹，若是则对牌面值进行存储  */
bool DDZPlayer::IsZhaDan(VECTORINT vector)
{
	bool flag=true;
	int tmp=vector.at(0);

	for(size_t i=1;i<vector.size();i++)
	{
		if(tmp!=vector[i])
		{
			flag=false;
		}
	}

	return flag;
}


unsigned DDZPlayer::getZhaDanValue(VECTORINT vector)
{
	if(vector.size()==4)
	{
		return vector[0];
	}
	else
	{
		return 60;
	}
}
/**判定vector是否为单牌，若是则对牌面值进行存储  */
bool DDZPlayer::ISingleCard(VECTORINT vector)
{
	bool flag = false;
	if(vector.size()==1&&vector[0]>=0&&vector[0]<=14)
	{
		flag = true;
	}

	return flag;
}

unsigned DDZPlayer::getSingleValue(vector<unsigned> vector)
{
	if(vector.size()>0)
	{
		return vector[0];
	}
	else
	{
		return 60;
	}
}

/**判定vector是否为对牌，若是则对牌面值进行存储  */
bool DDZPlayer::IsCoupleCards(VECTORINT vector)
{
	bool flag=true;
	unsigned tmp=vector[0];
	if(tmp!=vector[1])
	{
		flag = false;
	}

	return flag;
}

unsigned DDZPlayer::getCoupleValue(vector<unsigned> vector)
{
	if(vector.size()==2)
	{
		return vector[0];
	}
	else
	{
		return 60;
	}
}

/**判定vector是否为三条，若是则对牌面值进行存储 */
bool DDZPlayer::IsThreeCards(VECTORINT vector)
{
	bool flag=false;
	if(vector.size()==3&&vector[0]==vector[1]&&vector[1]==vector[2])
	{
		flag = true;
	}

	return flag;
}

unsigned DDZPlayer::getSanTiaoValue(vector<unsigned> vector)
{
	if(vector.size()>0)
	{
		return vector[0];
	}
	else
	{
		return 60;
	}
}

/** 对已排好序（升序）的vector 判定是否为单顺子，若是则对牌面值进行存储  */
bool DDZPlayer::IsSingleJunko(VECTORINT vector)
{
	bool flag=true;
	for(size_t i=0;i<vector.size()-1;i++)
	{
		if(vector[i+1]-vector[i]!=1)
		{
			flag=false;
		}
	}

	return flag;
}

vector<unsigned> DDZPlayer::getSingleJunkoValue(VECTORINT vector)
{
	VECTORINT singleJunkoValue;

	singleJunkoValue.push_back(vector[0]);
	singleJunkoValue.push_back(vector[vector.size()-1]);

	return singleJunkoValue;
}

/** 对已排好序（升序）的vector 判定是否为双顺，若是则对牌面值进行存储  */
bool DDZPlayer::IsDualJunko(VECTORINT vector)
{
	bool flag=true;

	for(size_t i=0;i<vector.size()-1;i++)
	{
		if(i%2==0)
		{
			if(vector.at(i)!=vector.at(i+1))
			{
				flag=false;
			}
		}

		if(i<vector.size()-2)
		{
			if(vector.at(i+2)-vector.at(i)!=1)
			{
				flag=false;
			}
		}
	}

	return flag;
}

VECTORINT DDZPlayer::getDualJunkoValue(VECTORINT vector)
{
	VECTORINT dualJunkoValue;

	dualJunkoValue.push_back(vector[0]);
	dualJunkoValue.push_back(vector[vector.size()-1]);

	return dualJunkoValue;
}

/** 判断vector是否为三顺，若是则对牌面值进行存储  */
bool DDZPlayer::IsThree_Shun(VECTORINT vector)
{
	VECTORINT tmp_vector;
	VECTORINT santiaos;
	bool flag = false;
	int tmp_three[15]={0};
	for(size_t i=0;i<vector.size();i++)
	{
		tmp_three[vector.at(i)]++;//记录三条数量
	}

	for(size_t j=0;j<13;j++)
	{
		if(tmp_three[j]==3)
		{
			santiaos.push_back(j);
		}
	}

	if(vector.size()/3==santiaos.size()&&IsSingleJunko(santiaos))
	{
		flag = true;
	}

	return flag;
	/*if(flag)
	{
		tmp_vector.push_back(1);	
		tmp_vector.push_back(tmp_vector1.at(0));
		tmp_vector.push_back(tmp_vector1.at(tmp_vector1.size()-1));
	}
	else
	{
		tmp_vector.push_back(0);
	}


	return tmp_vector;*/
}


VECTORINT DDZPlayer::getThree_ShunValue(VECTORINT vector)
{
	VECTORINT three_shunValue;

	three_shunValue.push_back(vector[0]);
	three_shunValue.push_back(vector[vector.size()-1]);

	return three_shunValue;
}

//void (^fackd)(int i);
/** 判断vector是否为三带一（三顺带单） ，若是则对牌面值进行存储 */
bool DDZPlayer::IsThree_One(VECTORINT vector)
{
	VECTORINT tmp_vector;
	VECTORINT threeTiaos;//对三条牌面值进行存储
	VECTORINT carryCards;//对三条带牌牌面值进行存储
	bool flag = false;
	unsigned tmp_three[15]={0};
	for(size_t i=0;i<vector.size();i++)
	{
		tmp_three[vector.at(i)]++;
	}

	for(size_t j=0;j<15;j++)
	{
		if(tmp_three[j]==3)
		{
			threeTiaos.push_back(j);
		}
		if(tmp_three[j]!=3&&tmp_three!=0)
		{
			for(size_t k=0;k<tmp_three[j];k++)
			{
				carryCards.push_back(j);
			}
		}
	}

	if(vector.size()/4==threeTiaos.size()&&IsSingleJunko(threeTiaos))
	{
		flag = true;
	}

	return flag;
}

VECTORINT DDZPlayer::getThree_OneValue(VECTORINT vector)
{
	VECTORINT three_OneValue;
	VECTORINT threeTiaos;//对三条牌面值进行存储
	VECTORINT carryCards;//对三条带牌牌面值进行存储
	bool flag = false;
	unsigned tmp_three[15]={0};
	for(size_t i=0;i<vector.size();i++)
	{
		tmp_three[vector[i]]++;
	}

	for(size_t j=0;j<15;j++)
	{
		if(tmp_three[j]==3)
		{
			threeTiaos.push_back(j);
		}
		if(tmp_three[j]!=3&&tmp_three!=0)
		{
			for(size_t k=0;k<tmp_three[j];k++)
			{
				carryCards.push_back(j);
			}
		}
	}

	three_OneValue.push_back(threeTiaos[0]);
	three_OneValue.push_back(threeTiaos[threeTiaos.size()-1]);
	for(size_t i=0;i<carryCards.size();i++)
	{
		three_OneValue.push_back(carryCards[i]);
	}

	return three_OneValue;
}

/** 判断vector是否为三带一对(三顺带双），若是则对牌面值进行存储  */
bool DDZPlayer::IsThree_Two(VECTORINT vector)
{
	VECTORINT tmp_vector;
	VECTORINT threeTiaos;
	VECTORINT carryCards;
	bool flag = false;
	int tmp_three[15]={0};
	for(size_t i=0;i<vector.size();i++)
	{
		tmp_three[vector[i]]++;
	}

	for(size_t j=0;j<15;j++)
	{
		if(tmp_three[j]==3)
		{
			threeTiaos.push_back(j);//对三条牌面值进行存储
		}
		if(tmp_three[j]==2)
		{
			carryCards.push_back(j);//对三条带牌牌面值进行存储
		}
	}

	if(vector.size()/5==threeTiaos.size()&&threeTiaos.size()==carryCards.size()&&IsSingleJunko(threeTiaos))
	{
		flag = true;
	}

	return flag;
}

VECTORINT DDZPlayer::getThree_TwoValue(VECTORINT vector)
{
	VECTORINT three_TwoValue;
	VECTORINT threeTiaos;
	VECTORINT carryCards;
	bool flag = false;
	int tmp_three[15]={0};
	for(size_t i=0;i<vector.size();i++)
	{
		tmp_three[vector[i]]++;
	}

	for(size_t j=0;j<15;j++)
	{
		if(tmp_three[j]==3)
		{
			threeTiaos.push_back(j);//对三条牌面值进行存储
		}
		if(tmp_three[j]==2)
		{
			carryCards.push_back(j);//对三条带牌牌面值进行存储
		}
	}

	three_TwoValue.push_back(threeTiaos[0]);
	three_TwoValue.push_back(threeTiaos[threeTiaos.size()-1]);
	
	for(size_t i=0;i<carryCards.size();i++)
	{
		three_TwoValue.push_back(carryCards[i]);
		three_TwoValue.push_back(carryCards[i]);
	}


	return three_TwoValue;
}
/** 判断vector是否为四带二，若是则对牌面值进行存储  */
bool DDZPlayer::IsFour_Two(VECTORINT vector)
{
	VECTORINT tmp_vector;
	VECTORINT tmp_vector1;
	VECTORINT tmp_vector2;
	bool flag = false;
	unsigned tmp_four[15]={0};
	for(size_t i=0;i<vector.size();i++)
	{
		tmp_four[vector.at(i)]++;
	}

	for(size_t j=0;j<15;j++)
	{
		if(tmp_four[j]==4)
		{
			tmp_vector1.push_back(j);
		}
		if(tmp_four[j]!=4&&tmp_four!=0)
		{
			for(size_t t=0;t<tmp_four[j];t++)
				tmp_vector2.push_back(j);
		}
	}

	if(vector.size()/6==tmp_vector1.size()&&(tmp_vector1.size()*2)==tmp_vector2.size()&&IsSingleJunko(tmp_vector1))
	{
		flag = true;
	}


	return flag;
}

VECTORINT DDZPlayer::getFour_TwoValue(VECTORINT vector)
{
	VECTORINT four_TwoValue;
	VECTORINT four_num;
	VECTORINT carryCards;
	unsigned tmp_four[15]={0};
	for(size_t i=0;i<vector.size();i++)
	{
		tmp_four[vector.at(i)]++;
	}

	for(size_t j=0;j<15;j++)
	{
		if(tmp_four[j]==4)
		{
			four_num.push_back(j);
		}
		if(tmp_four[j]!=4&&tmp_four!=0)
		{
			for(size_t t=0;t<tmp_four[j];t++)
				carryCards.push_back(j);
		}
	}


	four_TwoValue.push_back(four_num[0]);
	for(size_t i=0;i<carryCards.size();i++)
	{
		four_TwoValue.push_back(carryCards[i]);
	}

	return four_TwoValue;
}

/** 判断vector是否为四带二对，若是则对牌面值进行存储  */
bool DDZPlayer::IsFour_TwoCouple(VECTORINT vector)
{
	VECTORINT tmp_vector;
	VECTORINT four_num;
	VECTORINT carryCards;
	bool flag = false;
	int tmp_three[15]={0};
	for(size_t i=0;i<vector.size();i++)
	{
		tmp_three[vector.at(i)]++;
	}

	for(size_t j=0;j<15;j++)
	{
		if(tmp_three[j]==4)
		{
			four_num.push_back(j);
		}
		if(tmp_three[j]==2)
		{
			carryCards.push_back(j);
		}
	}

	if(vector.size()/8==four_num.size()&&(four_num.size()*2)==carryCards.size())
	{
		flag = true;
	}

	if(vector.size()==2)//四个带四个，如2222带JJ、JJ，此种牌型按大的牌算
	{
		flag=true;
	}


	return flag;
}


VECTORINT DDZPlayer::getFour_TwoCoupleValue(VECTORINT vector)
{
	VECTORINT four_TwoCoupleValue;
	VECTORINT four_num;
	VECTORINT carryCards;
	int tmp_three[15]={0};
	for(size_t i=0;i<vector.size();i++)
	{
		tmp_three[vector.at(i)]++;
	}

	for(size_t j=0;j<15;j++)
	{
		if(tmp_three[j]==4)
		{
			four_num.push_back(j);
		}
		if(tmp_three[j]==2)
		{
			carryCards.push_back(j);
		}
	}

	if(four_num.size()==1)
	{
		four_TwoCoupleValue.push_back(four_num[four_num.size()-1]);
		for(size_t i=0;i<carryCards.size();i++)
		{
			four_TwoCoupleValue.push_back(carryCards[i]);
			four_TwoCoupleValue.push_back(carryCards[i]);
		}
	}
	else if(four_num.size()==2)
	{
		four_TwoCoupleValue.push_back(four_num[four_num.size()-1]);

		for(size_t tmp=0;tmp<4;tmp++)
		{
			four_TwoCoupleValue.push_back(four_num[four_num.size()-2]);
		}
		
		for(size_t i=0;i<carryCards.size();i++)
		{
			four_TwoCoupleValue.push_back(carryCards[i]);
			four_TwoCoupleValue.push_back(carryCards[i]);
		}
	}

	return four_TwoCoupleValue;
}




/** 选择排序 */
VECTORINT DDZPlayer::sortCardsVector(VECTORINT vector)
{	
	for(size_t i=0;i<vector.size();i++)
		{
			int min=60;
			int index;
			for(size_t j=i;j<vector.size();j++)
			{
				if(min>vector.at(j))
				{
					min=vector.at(j);
					index=j;
				}
			}
			vector.at(index)=vector.at(i);
			vector.at(i)=min;
		}

	return vector;
}

VECTORINT DDZPlayer::CardsToIndex(VECTORINT v,Player p)
{
	VECTORINT tmp_vector1;
	for(size_t i=0;i<v.size();i++)
	{
		p.p3_EachCardNum[v.at(i)]--;
		for(size_t j=0;j<p.p3_cardsList.size();j++)
		{
			if(p.p3_cardsList.at(j)==53)
			{
				if(v.at(i)==14)// 出牌为大王时特殊处理
				{
					VECTORINT::iterator it = p.p3_cardsList.begin()+j;
					tmp_vector1.push_back(p.p3_cardsList.at(j));
					p.p3_cardsList.erase(it);
					break;
				}
			}		
			else if(p.p3_cardsList.at(j)/4==v.at(i))
			{
				VECTORINT::iterator it = p.p3_cardsList.begin()+j;
				tmp_vector1.push_back(p.p3_cardsList.at(j));
				p.p3_cardsList.erase(it);
				break;
			}
		}
	}	

	return tmp_vector1;
}

/**
*  产生最佳走步更新手中牌
*   p3_EachCardsNum
*/
void DDZPlayer::Update_P3_EachCardsNum(Player p,vector<unsigned> move)
{
	for(size_t i=0;i<15;i++)
	{
		for(size_t j=0;j<move.size();j++)
		{
			p.p3_EachCardNum[move.at(j)]--;
		}
	}
}