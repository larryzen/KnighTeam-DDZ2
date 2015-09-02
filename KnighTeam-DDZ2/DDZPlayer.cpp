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
#include "UCTSearch.h"
#include "SplitCards.h"


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
void DDZPlayer::CalName(char *cInMessage, char *cOutMessage)
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
void DDZPlayer::CalDeal(char *cInMessage,char *cOutMessage)
{
	CEveluation *eveluate;
	eveluate =(CEveluation*)malloc(sizeof(CEveluation));
	CThinkTable think = CThinkTable();
	// 设置默认玩家p3为我方，玩家p2为我方上家，玩家p1为我方下家
	Player::p3_pos=cInMessage[5]-'A';			// 设置我方位置  东/西/南
	
	/** 初始化玩家位置信息 */
	int pos3 = Player::p3_pos;
	switch(pos3)
	{
	case 0:
		Player::p1_pos=1;
		Player::p2_pos=2;

		Player::p1_front =0;
		Player::p1_next = 2;

		Player::p2_front=1;
		Player::p2_next =0;

		Player::p3_front=2;
		Player::p3_next=1;
		break;
	case 1:
		Player::p1_pos=2;
		Player::p2_pos=0;

		Player::p1_front =1;
		Player::p1_next = 0;

		Player::p2_front=2;
		Player::p2_next =1;

		Player::p3_front=0;
		Player::p3_next=2;
		break;
	case 2:
		Player::p1_pos=0;
		Player::p2_pos=1;

		Player::p1_front =2;
		Player::p1_next = 1;

		Player::p2_front=0;
		Player::p2_next =2;

		Player::p3_front=1;
		Player::p3_next=0;
		break;
	}

	string deal = string(cInMessage);

	string subDeal(deal.begin()+6,deal.end()); // 获取裁判所发的17张牌

	Player::p3_cardsList=stringUtil->stringSplit(subDeal,',');
	
	think.initEachCardNum();	// 初始化p3_EachCardNum
	think.updateEachCardNum(Player::p3_cardsList, 1);//更新我方牌数量（新增）

	eveluate->initMembership();    // 初始化玩家隶属度表
	eveluate->ClearedByDealCards();// 将其他玩家持有牌 为 我方持有牌隶属度清零
	eveluate->RefreshRemaining(Player::p3_cardsList);//更新剩余牌型表

	delete(eveluate);//释放指针内存
	strcpy_s(cOutMessage,80,"OK DEAL");
}


/**		收到平台命令：BID A3	/ BID WHAT					*/
/**	    回复：OK BID	   /  BID B3[(A/C)(1/2)]			*/
void DDZPlayer::CalBid(char *cInMessage,char *cOutMessage)
{
	CThinkTable think;
	char callBid[80];
	strcpy_s(callBid,80,"BID A0");
	callBid[4]+=Player::p3_pos;// 获得位置
	int bid = think.getBid();
	callBid[5]+=bid;// 叫分
	

	switch(cInMessage[4])
	{
	case 'A':
		if(Player::p1_pos==0)
		{
			Player::p1_bid=cInMessage[5]-'0';
		}
		else
		{
			Player::p2_bid=cInMessage[5]-'0';
		}
		strcpy_s(cOutMessage,80,"OK BID");
		break;
	case 'B':
		if(Player::p1_pos==1)
		{
			Player::p1_bid=cInMessage[5]-'0';
		}
		else
		{
			Player::p2_bid=cInMessage[5]-'0';
		}

		strcpy_s(cOutMessage,80,"OK BID");
		break;
	case 'C':
		if(Player::p1_pos==2)
		{
			Player::p1_bid=cInMessage[5]-'0';
		}
		else
		{
			Player::p2_bid=cInMessage[5]-'0';
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
void DDZPlayer::CalLeft(char *cInMessage,char *cOutMessage)
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
		if(Player::p1_pos==0)
		{
			Player::p1_IsLandlord=true;
			Player::p1_cardsNum+=3;
			Player::firstPlayer=1;
		}
		else if(Player::p2_pos==0)
		{
			Player::p2_IsLandlord=true;
			Player::p2_cardsNum+=3;
			Player::firstPlayer=2;
		}
		else
		{
			Player::p3_IsLandlord=true;
			Player::p3_cardsNum+=3;
			Player::firstPlayer=3;

			for(size_t i=0;i<list.size();i++)
			{
				Player::p3_cardsList.push_back(list.at(i));
			}
			think->updateEachCardNum(list, 1);
			eveluate->RefreshRemaining(list);//更新剩余牌型表
		}
		break;
	case 'B':
		if(Player::p1_pos==1)
		{
			Player::p1_IsLandlord=true;
			Player::p1_cardsNum+=3;
			Player::firstPlayer=1;
		}
		else if(Player::p2_pos==1)
		{
			Player::p2_IsLandlord=true;
			Player::p2_cardsNum+=3;
			Player::firstPlayer=2;
		}
		else
		{
			Player::p3_IsLandlord=true;
			Player::p3_cardsNum+=3;
			Player::firstPlayer=3;

			for(size_t i=0;i<list.size();i++)
			{
				Player::p3_cardsList.push_back(list.at(i));
			}
			think->updateEachCardNum(list, 1);
			eveluate->RefreshRemaining(list);//更新剩余牌型表
		}
		break;
	case 'C':
		if(Player::p1_pos==2)
		{
			Player::p1_IsLandlord=true;
			Player::p1_cardsNum+=3;
			Player::firstPlayer=1;

			
		}
		else if(Player::p2_pos==2)
		{
			Player::p2_IsLandlord=true;
			Player::p2_cardsNum+=3;
			Player::firstPlayer=2;
		}
		else
		{
			Player::p3_IsLandlord=true;
			Player::p3_cardsNum+=3;
			Player::firstPlayer=3;

			for(size_t i=0;i<list.size();i++)
			{
				Player::p3_cardsList.push_back(list.at(i));
			}
			think->updateEachCardNum(list, 1);
			eveluate->RefreshRemaining(list);//更新剩余牌型表
		}
		break;
		
	}

	Player::p3_allMoves=m_pMG->getMovesByMyself(Player::p3_cardsList.size(),Player::p3_EachCardNum);
	//m_pMG->PriorityCombinatoriaLibrary(p);
	eveluate->ClearedByLeftCards(list); // 根据底牌更新隶属度表
	
	strcpy_s(cOutMessage,80,"OK LEFTOVER");
	delete(think);
	delete(eveluate);//释放指针内存
}


/**		收到平台命令：PLAY A12，13，14，21 /  PLAY WHAT	*/
/**	    回复：OK PLAY		/    PLAY B31，32，33，0		*/
void DDZPlayer:: CalPla(char *cInMessage,char *cOutMessage)
{
	CEveluation *eveluate; 
	eveluate =(CEveluation*)malloc(sizeof(CEveluation));
	char callPass[20];
	char callPla[200];
	CMoveGenerator *move;
	move = (CMoveGenerator*)malloc(sizeof(CMoveGenerator));

	strcpy_s(callPass,20,"PLAY A-1");
	strcpy_s(callPla,200,"PLAY A");

	callPass[5]+=Player::p3_pos;
	callPla[5]+=Player::p3_pos;

	string tmp_str(cInMessage);
	
	switch(cInMessage[5])
	{
	case 'A':
		if(cInMessage[6]=='-')
		{
			CARDSMOVE null_move;
			NULL_MOVE(null_move);
			
			if(Player::p1_pos==0)
			{
				eveluate->UpdateFewCards(1);
				null_move.side = 1;
				Player::cardsMoveRecords.push_back(null_move);
		
				Player::p3_allMoves=move->getMovesByMyself(Player::p3_cardsList.size(),Player::p3_EachCardNum);
				//move->PriorityCombinatoriaLibrary(p);
			}
			else
			{
				null_move.side = 2;
				Player::cardsMoveRecords.push_back(null_move);
				eveluate->UpdateFewCards(2);
			
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


			eveluate->ClearedByPlayCards(list);//根据出牌更新玩家隶属度表
			eveluate->RefreshRemaining(list);// 更新剩余牌型表

			if(Player::p1_pos==0)
			{
				if(Player::firstPlayer==1)
				{
					Player::p1_manyCards[cardType]=1;
				}
				Player::p1_cardsNum-=list.size();// 玩家出一次牌，手中牌=原手中牌-出牌数量
				g_move.side = 1;
				Player::cardsMoveRecords.push_back(g_move);
			
				Player::p3_allMoves=move->getMovesByMyself(Player::p3_cardsList.size(),Player::p3_EachCardNum);
				//move->PriorityCombinatoriaLibrary(p);
				eveluate->UpdateByPlayCards( 1, cardType, generalCards);
			}
			else
			{
				if(Player::firstPlayer==2)
				{
					Player::p2_manyCards[cardType]=1;
				}
				Player::p2_cardsNum-=list.size();// 玩家出一次牌，手中牌=原手中牌-出牌数量
				g_move.side = 2;
				Player::cardsMoveRecords.push_back(g_move);
			
				eveluate->UpdateByPlayCards( 2, cardType, generalCards);
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
			if(Player::p1_pos==1)
			{
				eveluate->UpdateFewCards(1);
				null_move.side = 1;
				Player::cardsMoveRecords.push_back(null_move);
				Player::p3_allMoves=move->getMovesByMyself(Player::p3_cardsList.size(),Player::p3_EachCardNum);
				//move->PriorityCombinatoriaLibrary(p);
			}
			else
			{
				eveluate->UpdateFewCards(2);
				null_move.side = 2;
				Player::cardsMoveRecords.push_back(null_move);
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

			eveluate->ClearedByPlayCards(list);//根据出牌更新玩家隶属度表
			eveluate->RefreshRemaining(list);// 更新剩余牌型表

			if(Player::p1_pos==1)
			{
				if(Player::firstPlayer==1)
				{
					Player::p1_manyCards[cardType]=1;
				}
				Player::p1_cardsNum-=list.size();// 玩家出一次牌，手中牌=原手中牌-出牌数量
				g_move.side = 1;
				Player::cardsMoveRecords.push_back(g_move);
				Player::p3_allMoves=move->getMovesByMyself(Player::p3_cardsList.size(),Player::p3_EachCardNum);
				//move->PriorityCombinatoriaLibrary(p);
				eveluate->UpdateByPlayCards( 1, cardType, generalCards);
			}
			else
			{
				if(Player::firstPlayer==2)
				{
					Player::p2_manyCards[cardType]=1;
				}
				Player::p2_cardsNum-=list.size();// 玩家出一次牌，手中牌=原手中牌-出牌数量
				g_move.side = 2;
				Player::cardsMoveRecords.push_back(g_move);
				eveluate->UpdateByPlayCards( 2, cardType, generalCards);
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
			if(Player::p1_pos==2)
			{
				eveluate->UpdateFewCards(1);
				null_move.side = 1;
				Player::cardsMoveRecords.push_back(null_move);

				Player::p3_allMoves=move->getMovesByMyself(Player::p3_cardsList.size(),Player::p3_EachCardNum);
				//move->PriorityCombinatoriaLibrary(p);
			}
			else
			{
				eveluate->UpdateFewCards(2);
				null_move.side = 1;
				Player::cardsMoveRecords.push_back(null_move);
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

			eveluate->ClearedByPlayCards(list);//根据出牌更新玩家隶属度表
			eveluate->RefreshRemaining(list);// 更新剩余牌型表

			if(Player::p1_pos==2)
			{
				if(Player::firstPlayer==1)
				{
					Player::p1_manyCards[cardType]=1;
				}
				Player::p1_cardsNum-=list.size();// 玩家出一次牌，手中牌=原手中牌-出牌数量
				g_move.side = 1;
				Player::cardsMoveRecords.push_back(g_move);
				
				Player::p3_allMoves=move->getMovesByMyself(Player::p3_cardsList.size(),Player::p3_EachCardNum);
				//move->PriorityCombinatoriaLibrary(p);
				eveluate->UpdateByPlayCards( 1, cardType, generalCards);
			}
			else
			{
				if(Player::firstPlayer==2)
				{
					Player::p2_manyCards[cardType]=1;
				}
				Player::p2_cardsNum-=list.size();// 玩家出一次牌，手中牌=原手中牌-出牌数量
				g_move.side = 2;
				Player::cardsMoveRecords.push_back(g_move);
				eveluate->UpdateByPlayCards( 2, cardType, generalCards);
			}
		}
		delete(eveluate);//释放指针内存
		delete(move);	 //释放move指针内存
		strcpy_s(cOutMessage,80,"OK PLAY");
		break;
	case 'W':
		{
			setFirstAndLastPlayer();
			CThinkTable *think;
			think=(CThinkTable*)malloc(sizeof(CThinkTable));
			
			VECTORINT tmp_vector1;
			CARDSMOVE bestMove;

			CSearchEngine *ddz_SE;
			
			if (think->IsHalfGame() && START_UCT)
			{
				ddz_SE = new UCTSearch();
			}
			else
			{
				ddz_SE = new AlphaBeta();
			}
			

			ddz_SE->SearchAGoodMove(0);//获得最佳走步
			bestMove = ddz_SE->bestMove;

			if (0 &&bestMove.cardsType == PASS)//若搜索引擎未搜到走步
			{
				SplitCards ddz_SC = SplitCards(Player::p3_EachCardNum);
				//则进行必要的拆牌处理
				if (Player::p1_IsLandlord || (Player::p3_IsLandlord 
				&& (Player::lastMove.cardsType == SINGLE || Player::lastMove.cardsType == COUPLE)))
				{
					
					bestMove = ddz_SC.GetASplitMove(Player::lastMove, false);
				}
				
				
				if (Player::p1_IsLandlord)
				{
					if (Player::p1_cardsNum <= RESTCARDS && Player::lastPlayer == 1)
					{
						bestMove = ddz_SC.GetASplitMove(Player::lastMove, true);
					}
				}
				else if (Player::p2_IsLandlord)
				{
					if (Player::p2_cardsNum <= RESTCARDS && Player::lastPlayer == 2)
					{
						bestMove = ddz_SC.GetASplitMove(Player::lastMove, true);
					}
				}
				else if (Player::p3_IsLandlord)
				{
					if (Player::p1_cardsNum <= RESTCARDS || Player::p2_cardsNum <= RESTCARDS)
					{
						bestMove = ddz_SC.GetASplitMove(Player::lastMove, true);
						
					}
				}
			}

	
			Player::cardsMoveRecords.push_back(bestMove);
			if(bestMove.cardsType != PASS)
			{		
				tmp_vector1=CardsToIndex(bestMove.cards);


				Player::p3_cardsNum-=tmp_vector1.size();		// 玩家出一次牌，手中牌=原手中牌-出牌数量
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



/**
*   一局牌完重新初始化玩家信息，继续下一局
*/
void DDZPlayer::initPlayer()
{
	Player::p1_pos=-1;

	Player::p1_IsLandlord = false;

	Player::p1_cardsNum = 17;

	Player::p1_bid=0;

	Player::p1_front=-1;

	Player::p1_next=-1;

	Player::p1_cardsList.clear();

	Player::p1_general.clear();



	Player::p2_pos=-1;

	Player::p2_IsLandlord=false;

	Player::p2_cardsNum=17;

	Player::p2_bid=0;

	Player::p2_front=-1;

	Player::p2_next=-1;

	Player::p2_cardsList.clear();

	Player::p2_yiChu.clear();

	Player::p2_general.clear();



	Player::p3_pos=-1;

	Player::p3_IsLandlord=false;

	Player::p3_cardsNum=17;

	Player::p3_bid=0;

	Player::p3_front=-1;

	Player::p3_next=-1;

	Player::p3_cardsList.clear();

	Player::p3_yiChu.clear();



	Player::firstPlayer=-1;

	Player::p3_general.clear();
	
	Player::p3_canPlayOver = false;

	Player::p3_comb.clear();

	for(size_t i=0;i<15;i++)//初始化棋子剩余表
	{
		if(i>=13)
		{
			Player::remaining[i]=1;
		}
		else
		{
			Player::remaining[i]=4;
		}
	}

	Player::cardsMoveRecords.clear();
}

/**
*  根据各玩家已出牌,更新主动出牌玩家
*/
void DDZPlayer::setFirstAndLastPlayer()
{
	int size = Player::cardsMoveRecords.size()-1;
	int count = 0;
	bool flag = true;
	while (size >= 0)
	{
		if (Player::cardsMoveRecords[size].cardsType == PASS)
		{
			count++;
		}
		else
		{
			if (flag)
			{
				Player::lastMove = Player::cardsMoveRecords[size];
				Player::lastPlayer = Player::lastMove.side;
				flag = false;//记录一次后就不再记录
			}
			
			count=0;
		}
		
		if (count == 2)
		{	
			if (size + 2 < Player::cardsMoveRecords.size())
			{
				Player::firstMove = Player::cardsMoveRecords[size + 2];
			}	
			else
			{
				Player::firstMove.cardsType = INVALID;//轮到我方主动出牌
			}
			Player::firstPlayer = Player::cardsMoveRecords[size - 1].side;
			break;
		}
		size--;
	}

	if (flag)//轮到我方主动出牌
	{
		Player::lastMove.cardsType=INVALID;
		Player::lastPlayer = -1;
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

///** 快速排序 Player::p3_cardsList 升序 */
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
//	int key=Player::p3_cardsList.at(first);
//	while(first<last)
//	{
//		while(first<last&&Player::p3_cardsList.at(last)>=key)
//			--last;
//		Player::p3_cardsList.at(first)=Player::p3_cardsList.at(last);
//		while(first<last&&Player::p3_cardsList.at(first)<=key)
//			++first;
//		Player::p3_cardsList.at(last)=Player::p3_cardsList.at(first);
//	}
//	Player::p3_cardsList.at(first)=key;
//	sortVectorLowToHigh(p,low,first-1);
//	sortVectorLowToHigh(p,last+1,high);
//}

bool DDZPlayer::IsRocket(VECTORINT vi)
{
	bool flag = false;
	if (vi.size() == 2 && vi[0] == 13 && vi[1] == 14)
	{
		flag = true;
	}

	return flag;
}

/** 判断vector是否为炸弹，若是则对牌面值进行存储  */
bool DDZPlayer::IsZhaDan(VECTORINT vi)
{
	bool flag=true;
	int tmp = vi.at(0);

	for (size_t i = 1; i<vi.size(); i++)
	{
		if (tmp != vi[i])
		{
			flag=false;
		}
	}

	return flag;
}


unsigned DDZPlayer::getZhaDanValue(VECTORINT vi)
{
	if(vi.size()==4)
	{
		return vi[0];
	}
	else
	{
		return 60;
	}
}
/**判定vector是否为单牌，若是则对牌面值进行存储  */
bool DDZPlayer::ISingleCard(VECTORINT vi)
{
	bool flag = false;
	if(vi.size()==1&&vi[0]>=0&&vi[0]<=14)
	{
		flag = true;
	}

	return flag;
}

unsigned DDZPlayer::getSingleValue(vector<unsigned> vi)
{
	if(vi.size()>0)
	{
		return vi[0];
	}
	else
	{
		return 60;
	}
}

/**判定vector是否为对牌，若是则对牌面值进行存储  */
bool DDZPlayer::IsCoupleCards(VECTORINT vi)
{
	bool flag=true;
	unsigned tmp=vi[0];
	if(tmp!=vi[1])
	{
		flag = false;
	}

	return flag;
}

unsigned DDZPlayer::getCoupleValue(vector<unsigned> vi)
{
	if(vi.size()==2)
	{
		return vi[0];
	}
	else
	{
		return 60;
	}
}

/**判定vector是否为三条，若是则对牌面值进行存储 */
bool DDZPlayer::IsThreeCards(VECTORINT vi)
{
	bool flag=false;
	if(vi.size()==3&&vi[0]==vi[1]&&vi[1]==vi[2])
	{
		flag = true;
	}

	return flag;
}

unsigned DDZPlayer::getSanTiaoValue(vector<unsigned> vi)
{
	if(vi.size()>0)
	{
		return vi[0];
	}
	else
	{
		return 60;
	}
}

/** 对已排好序（升序）的vector 判定是否为单顺子，若是则对牌面值进行存储  */
bool DDZPlayer::IsSingleJunko(VECTORINT vi)
{
	bool flag=true;
	for(size_t i=0;i<vi.size()-1;i++)
	{
		if(vi[i+1]-vi[i]!=1)
		{
			flag=false;
		}
	}

	return flag;
}

vector<unsigned> DDZPlayer::getSingleJunkoValue(VECTORINT vi)
{
	VECTORINT singleJunkoValue;

	singleJunkoValue.push_back(vi[0]);
	singleJunkoValue.push_back(vi[vi.size()-1]);

	return singleJunkoValue;
}

/** 对已排好序（升序）的vector 判定是否为双顺，若是则对牌面值进行存储  */
bool DDZPlayer::IsDualJunko(VECTORINT vi)
{
	bool flag=true;

	for(size_t i=0;i<vi.size()-1;i++)
	{
		if(i%2==0)
		{
			if(vi.at(i)!=vi.at(i+1))
			{
				flag=false;
			}
		}

		if(i<vi.size()-2)
		{
			if(vi.at(i+2)-vi.at(i)!=1)
			{
				flag=false;
			}
		}
	}

	return flag;
}

VECTORINT DDZPlayer::getDualJunkoValue(VECTORINT vi)
{
	VECTORINT dualJunkoValue;

	dualJunkoValue.push_back(vi[0]);
	dualJunkoValue.push_back(vi[vi.size()-1]);

	return dualJunkoValue;
}

/** 判断vector是否为三顺，若是则对牌面值进行存储  */
bool DDZPlayer::IsThree_Shun(VECTORINT vi)
{
	VECTORINT santiaos;
	bool flag = false;
	int tmp_three[15]={0};
	for(size_t i=0;i<vi.size();i++)
	{
		tmp_three[vi.at(i)]++;//记录三条数量
	}

	for(size_t j=0;j<13;j++)
	{
		if(tmp_three[j]==3)
		{
			santiaos.push_back(j);
		}
	}

	if(vi.size()/3==santiaos.size()&&IsSingleJunko(santiaos))
	{
		flag = true;
	}

	return flag;
}


VECTORINT DDZPlayer::getThree_ShunValue(VECTORINT vi)
{
	VECTORINT three_shunValue;

	three_shunValue.push_back(vi[0]);
	three_shunValue.push_back(vi[vi.size()-1]);

	return three_shunValue;
}

/** 判断vector是否为三带一（三顺带单） ，若是则对牌面值进行存储 */
bool DDZPlayer::IsThree_One(VECTORINT vi)
{
	VECTORINT threeTiaos;//对三条牌面值进行存储
	VECTORINT carryCards;//对三条带牌牌面值进行存储
	bool flag = false;
	unsigned tmp_three[15]={0};
	for(size_t i=0;i<vi.size();i++)
	{
		tmp_three[vi.at(i)]++;
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

	if(vi.size()/4==threeTiaos.size()&&IsSingleJunko(threeTiaos)&& carryCards.size() == threeTiaos.size())
	{
		flag = true;
	}

	return flag;
}

/**
*	获得三带单（三顺带单）值
*/
VECTORINT DDZPlayer::getThree_OneValue(VECTORINT vi)
{
	VECTORINT three_OneValue;
	VECTORINT threeTiaos;//对三条牌面值进行存储
	VECTORINT carryCards;//对三条带牌牌面值进行存储
	bool flag = false;
	unsigned tmp_three[15]={0};
	for(size_t i=0;i<vi.size();i++)
	{
		tmp_three[vi[i]]++;
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
bool DDZPlayer::IsThree_Two(VECTORINT vi)
{
	VECTORINT threeTiaos;
	VECTORINT carryCards;
	bool flag = false;
	int tmp_three[15]={0};
	for(size_t i=0;i<vi.size();i++)
	{
		tmp_three[vi[i]]++;
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

	if(vi.size()/5==threeTiaos.size()&&threeTiaos.size()==carryCards.size()&&IsSingleJunko(threeTiaos))
	{
		flag = true;
	}

	return flag;
}

VECTORINT DDZPlayer::getThree_TwoValue(VECTORINT vi)
{
	VECTORINT three_TwoValue;
	VECTORINT threeTiaos;
	VECTORINT carryCards;
	bool flag = false;
	int tmp_three[15]={0};
	for(size_t i=0;i<vi.size();i++)
	{
		tmp_three[vi[i]]++;
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
bool DDZPlayer::IsFour_Two(VECTORINT vi)
{
	VECTORINT tmp_vi;
	VECTORINT tmp_vi1;
	VECTORINT tmp_vi2;
	bool flag = false;
	unsigned tmp_four[15]={0};
	for(size_t i=0;i<vi.size();i++)
	{
		tmp_four[vi.at(i)]++;
	}

	for(size_t j=0;j<15;j++)
	{
		if(tmp_four[j]==4)
		{
			tmp_vi1.push_back(j);
		}
		if(tmp_four[j]!=4&&tmp_four!=0)
		{
			for(size_t t=0;t<tmp_four[j];t++)
				tmp_vi2.push_back(j);
		}
	}

	if(vi.size()/6==tmp_vi1.size()&&(tmp_vi1.size()*2)==tmp_vi2.size()&&IsSingleJunko(tmp_vi1))
	{
		flag = true;
	}


	return flag;
}

VECTORINT DDZPlayer::getFour_TwoValue(VECTORINT vi)
{
	VECTORINT four_TwoValue;
	VECTORINT four_num;
	VECTORINT carryCards;
	unsigned tmp_four[15]={0};
	for(size_t i=0;i<vi.size();i++)
	{
		tmp_four[vi.at(i)]++;
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
bool DDZPlayer::IsFour_TwoCouple(VECTORINT vi)
{
	VECTORINT tmp_vi;
	VECTORINT four_num;
	VECTORINT carryCards;
	bool flag = false;
	int tmp_three[15]={0};
	for(size_t i=0;i<vi.size();i++)
	{
		tmp_three[vi.at(i)]++;
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

	if(vi.size()/8==four_num.size()&&(four_num.size()*2)==carryCards.size())
	{
		flag = true;
	}

	if(vi.size()==2)//四个带四个，如2222带JJ、JJ，此种牌型按大的牌算
	{
		flag=true;
	}


	return flag;
}


VECTORINT DDZPlayer::getFour_TwoCoupleValue(VECTORINT vi)
{
	VECTORINT four_TwoCoupleValue;
	VECTORINT four_num;
	VECTORINT carryCards;
	int tmp_three[15]={0};
	for(size_t i=0;i<vi.size();i++)
	{
		tmp_three[vi.at(i)]++;
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
	else if(four_num.size()==2)// 四个带相同两对，即四个带四个
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
VECTORINT DDZPlayer::sortCardsVector(VECTORINT vi)
{	
	for(size_t i=0;i<vi.size();i++)
		{
			int min=60;
			int index;
			for(size_t j=i;j<vi.size();j++)
			{
				if(min>vi.at(j))
				{
					min=vi.at(j);
					index=j;
				}
			}
			vi.at(index)=vi.at(i);
			vi.at(i)=min;
		}

	return vi;
}

VECTORINT DDZPlayer::CardsToIndex(VECTORINT v)
{
	VECTORINT tmp_vector1;
	for(size_t i=0;i<v.size();i++)
	{
		Player::p3_EachCardNum[v.at(i)]--;
		for(size_t j=0;j<Player::p3_cardsList.size();j++)
		{
			if(Player::p3_cardsList.at(j)==53)
			{
				if(v.at(i)==14)// 出牌为大王时特殊处理
				{
					VECTORINT::iterator it = Player::p3_cardsList.begin()+j;
					tmp_vector1.push_back(Player::p3_cardsList.at(j));
					Player::p3_cardsList.erase(it);
					break;
				}
			}		
			else if(Player::p3_cardsList.at(j)/4==v.at(i))
			{
				VECTORINT::iterator it = Player::p3_cardsList.begin()+j;
				tmp_vector1.push_back(Player::p3_cardsList.at(j));
				Player::p3_cardsList.erase(it);
				break;
			}
		}
	}	

	return tmp_vector1;
}