#include "MoveGenerator.h"
#include "FileWriter.h"
#include "string.h"
#include "stdlib.h"
#include "vector"

using namespace std;
CMoveGenerator::CMoveGenerator(void)
{
	lastWhoseGo=0;
}

CMoveGenerator::~CMoveGenerator(void)
{
}

/**
*   走法生成器入口，分为主动和被动出牌
*  (1)我方为地主，且三个玩家都尚未出牌; (2)上下两家均pass;->主动出牌，代号1
*  (1)上家未pass,按上家出牌; (2)上家pass,下家未pass,按下家出牌;->被动出牌，代号2
*/
vector<CARDSMOVE> CMoveGenerator::getMoves(Player p,int whoseGo)
{
	vector<CARDSMOVE> p1 =p.p1_general;
	vector<CARDSMOVE> p2 =p.p2_general;
	vector<CARDSMOVE> p3 =p.p3_general;

	vector<CARDSMOVE> moves;
	bool IsCanGo = false;
	int turn =whoseGo%3;														 // 轮到哪方出牌，则进行该方主动、被动出牌判断
	unsigned tmp_EachCardNum[15];										 // 根据哪方的牌来获得走步
	int cardsNum;																	 //  玩家手中牌数量
	CARDSMOVE p1_lastMove,p2_lastMove,p3_lastMove;	 //  若玩家已走牌不为空，则取出其最后一步出牌
	CARDSMOVE lastMove;													 //	 当前玩家的上家上次出牌牌面值

	if(turn==0)  // 获得我方走步
	{
		if(p.p3_IsLandlord==true)// 我方为地主，第一轮出牌
		{
			if(p1.empty()&&p2.empty()&&p3.empty())
			{
				IsCanGo =true;
			}
		}

		if(!p2.empty()&&!p1.empty()) // 上家下家均pass,此轮为我方主动出牌
		{
			if((p2.at(p2.size()-1)).cardsType==-1 && (p1.at(p1.size()-1)).cardsType==-1)
			{
				IsCanGo = true;
			}
		}
		memcpy(tmp_EachCardNum,p.p3_EachCardNum,sizeof(tmp_EachCardNum));

		if(IsCanGo)
		{
			cardsNum =p.p3_cardsList.size();	

			NULL_MOVE(p.lastMove);
			p.lastPlayer=-1;
		}
		else 
		{
			if(!p2.empty())
			{
				p2_lastMove = p2.at(p2.size()-1);

				if(p2_lastMove.cardsType!=-1)
				{
					lastMove=p2_lastMove;

					p.lastMove=p2_lastMove;
					p.lastPlayer=2;
				}
				else
				{
					if(!p1.empty())
					{
						p1_lastMove=p1.at(p1.size()-1);

						if(p1_lastMove.cardsType!=-1)
						{
							lastMove=p1_lastMove;

							p.lastMove=p1_lastMove;
							p.lastPlayer=1;
						}
					}
				}
			}
		}
	}
	else if(turn ==2)  //获得玩家1走步
	{
		if(!p2.empty()&&!p3.empty())// 玩家1判断是否为自己主动出牌，不存在判断玩家1、2开始为地主主动出牌
		{
			if((p2.at(p2.size()-1)).cardsType==-1 && (p3.at(p3.size()-1)).cardsType==-1)
			{
				IsCanGo = true;
			}
		}
		memcpy(tmp_EachCardNum,p.p1_EachCardNum,sizeof(tmp_EachCardNum));

		if(IsCanGo)
		{
			cardsNum =p.p1_cardsList.size();	

			NULL_MOVE(p.lastMove);
			p.lastPlayer=-1;
		}
		else
		{
			if(!p3.empty())
			{
				p3_lastMove = p3.at(p3.size()-1);

				if(p3_lastMove.cardsType!=-1)
				{
					lastMove=p3_lastMove;

					
					p.lastMove=p3_lastMove;
					p.lastPlayer=3;
				}
				else
				{
					if(!p2.empty())
					{
						p2_lastMove=p2.at(p2.size()-1);

						if(p2_lastMove.cardsType!=-1)
						{
							lastMove=p2_lastMove;
							
							p.lastMove=p2_lastMove;
							p.lastPlayer=2;
						}
					}
				}
			}
		}
	}
	else				//获得玩家2走步
	{
		if(!p1.empty()&&!p3.empty())// 玩家2判断是否为自己主动出牌，不存在判断玩家1、2开始为地主主动出牌
		{
			if((p1.at(p1.size()-1)).cardsType==-1 && (p3.at(p3.size()-1)).cardsType==-1)
			{
				IsCanGo = true;
			}
		}
		memcpy(tmp_EachCardNum,p.p2_EachCardNum,sizeof(tmp_EachCardNum));

		if(IsCanGo)
		{
			cardsNum =p.p2_cardsList.size();	

			NULL_MOVE(p.lastMove);
			p.lastPlayer=-1;
		}
		else
		{
			if(!p1.empty())
			{
				p1_lastMove = p1.at(p1.size()-1);

				if(p1_lastMove.cardsType!=-1)
				{
					lastMove=p1_lastMove;

					
					p.lastMove=p1_lastMove;
					p.lastPlayer=1;
				}
				else
				{
					if(!p3.empty())
					{
						p3_lastMove=p3.at(p3.size()-1);

						if(p3_lastMove.cardsType!=-1)
						{
							lastMove=p3_lastMove;

							
							p.lastMove=p1_lastMove;
							p.lastPlayer=1;
						}
					}
				}
			}
		}
	}


	if(IsCanGo)	// 主动出牌
	{		
		vector<CARDSMOVE> tmp_moves =(vector<CARDSMOVE>)getMovesByMyself(cardsNum,tmp_EachCardNum);// 获取持有牌可能的所有走步
		for(size_t j=0;j<tmp_moves.size();j++)
		{
			moves.push_back(tmp_moves.at(j));
		}

		//VECTORINT method;// 将元素为1或2的vector加入走法器，判断为主动还是被动走步
		//method.push_back(1);// method=>为1:主动出牌
		//moves.push_back(method);
	}
	else	   // 被动出牌(按其他玩家上一轮出牌走步)
	{
		vector<CARDSMOVE> tmp_moves = (vector<CARDSMOVE>) getMovesByOthers(lastMove,tmp_EachCardNum);// 根据其他玩家牌获得我方手中所有走步
		for(size_t j=0;j<tmp_moves.size();j++)
		{
			tmp_moves.at(j).outWay=0;
			moves.push_back(tmp_moves.at(j));
		}

		//VECTORINT method;// 将元素为1或2的vector加入走法器，判断为主动还是被动走步
		//method.push_back(2);// method=>为2:被动出牌
		//moves.push_back(method);
	}
	
	return moves;
}

/**
*   根据其他玩家出牌获取我方所有走步
*  @CardsType 出牌类型  @v 出牌详细，如：3个4带5->4445
*/
vector<CARDSMOVE> CMoveGenerator::getMovesByOthers(CARDSMOVE lastMove,unsigned *cards)
{
	vector<CARDSMOVE> moves;
	vector<CARDSMOVE> tmp_moves;
	DDZPlayer *play;
	play = (DDZPlayer*)malloc(sizeof(DDZPlayer));
	int CardsType = lastMove.cardsType;
	vector<unsigned> v = lastMove.cards;
	switch(CardsType)
	{
		case 1:// 火箭最大牌型，无走步。
			break;

		case 2:// 炸弹
			{
				unsigned ZhaDanValue = play->getZhaDanValue(v);// 获取炸弹的牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByZhaDan(ZhaDanValue,cards);
				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}				
			break;
			}
				
		case 3:// 单牌
			{
				int SingleCardValue = play->getSingleValue(v);// 获取单牌的牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesBySingleCard(SingleCardValue,cards);
				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}			
			break;
			}

		case 4:// 对牌
			{
				int CoupleCardsValue = play->getCoupleValue(v);// 获取对牌的牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByCoupleCards(CoupleCardsValue,cards);
				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}				
			break;
			}

		case 5:// 三条
			{
				int ThreeCardsValue = play->getSanTiaoValue(v);// 获取三条的牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByThreeTiaoCards(ThreeCardsValue,cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}

		case 6:// 三带单
			{
				int Three_OneCardsValue = play->getThree_OneValue(v).at(0);// 获取三带单的牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByThree_One(Three_OneCardsValue,cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}

		case 7:// 三带双
			{
				int Three_TwoCardsValue = play->getThree_TwoValue(v).at(0);// 获取三带双的牌面值
				tmp_moves =(vector<CARDSMOVE>)getMovesByThree_Two(Three_TwoCardsValue,cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}

		case 8:// 单顺
			{
				int cardBeginValue = play->getSingleJunkoValue(v).at(0);// 获取单顺开始牌面值
				int cardEndValue = play->getSingleJunkoValue(v).at(1);	// 获取单顺结束牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesBySingleJunko(cardBeginValue,cardEndValue,cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}

		case 9: // 双顺
			{
				int cardBeginValue = play->getDualJunkoValue(v).at(0);// 获取双顺开始牌面值
				int cardEndValue = play->getDualJunkoValue(v).at(1);  // 获取双顺结束牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByDualJunko(cardBeginValue,cardEndValue,cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}

		case 10:// 三顺
			{
				int cardBeginValue = play->getThree_ShunValue(v).at(0);// 获取三顺开始牌面值
				int cardEndValue = play->getThree_ShunValue(v).at(1);  // 获取三顺结束牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByThree_Shun(cardBeginValue,cardEndValue,cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}

		case 11:// 三顺
			{
				int cardBeginValue = play->getThree_OneValue(v).at(0);// 获取三顺带单开始牌面值
				int cardEndValue = play->getThree_OneValue(v).at(1);  // 获取三顺带单结束牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByThree_Shun_One(cardBeginValue,cardEndValue,cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}

		case 12: // 三顺带双
			{
				int cardBeginValue = play->getThree_TwoValue(v).at(0); // 获取三顺带双开始牌面值
				int cardEndValue = play->getThree_TwoValue(v).at(1);   // 获取三顺带双结束牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByThree_Shun_Couple(cardBeginValue,cardEndValue,cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}

		case 13:// 四带二
			{
				int Four_TwoCardsValue = play->getFour_TwoValue(v).at(0);	    // 获取四带二单的牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByFour_Two(Four_TwoCardsValue,cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}
		case 14:// 四带二对
			{
				int Four_TwoCoupleCardsValue = play->getFour_TwoCoupleValue(v).at(0);  // 获取四带二对的牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByFour_TwoCouple(Four_TwoCoupleCardsValue,cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}
		case -1:
			break;
		}

		if(CardsType!=ZHADAN || CardsType!=ROCKET)// 炸弹和火箭管一般牌型
		{
			for(size_t i=0;i<13;i++)
			{
				if(cards[i]==4)
				{
					CARDSMOVE bomb;
					for(size_t k=0;k<4;k++)
					{
						bomb.cards.push_back(i);
					}
					bomb.cardsType=ZHADAN;
					bomb.outWay=0;
					moves.push_back(bomb);
				}
			}

			if(cards[13]==1 && cards[14]==1)
			{
				CARDSMOVE rocket;
				rocket.cards.push_back(13);
				rocket.cards.push_back(14);
				rocket.cardsType=ROCKET;
				rocket.outWay=0;
				moves.push_back(rocket);
			}
		}		
		return moves;
}

/**
*   获取主动出牌的所有走步
*/
vector<CARDSMOVE> CMoveGenerator::getMovesByMyself(unsigned cardsNum,unsigned *EachCardNum)
{
	vector<CARDSMOVE> moves;
	for(size_t i=1;i<=cardsNum;i++)// 根据我方牌的张数从1张——>最多张数获取所有走步
	{
			vector<CARDSMOVE>  tmp_moves = (vector<CARDSMOVE>) getMovesByCardsNum(i,EachCardNum);
			for(size_t j=0;j<tmp_moves.size();j++)
			{
				tmp_moves.at(j).outWay=1;
				moves.push_back(tmp_moves.at(j));
				
			}
	}

	return moves;
}
/** 获取指定牌张数的走步*/
/** 如@ n=3 ,则获取牌数量为3张的所有走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByCardsNum(unsigned n,unsigned *cards)
{
	vector<CARDSMOVE> tmp_moves;
	switch(n)
	{
	case 1:// 获取牌张数为1的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesByOneCard(cards);
		break;

	case 2:// 获取牌张数为2的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesByTwoCards(cards);
		break;

	case 3:// 获取牌张数为3的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesByThreeCards(cards);
		break;

	case 4:// 获取牌张数为4的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesByFourCards(cards);
		break;

	case 5:// 获取牌张数为5的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesByFiveCards(cards);
		break;

	case 6:// 获取牌张数为6的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBySixCards(cards);
		break;

	case 7:// 获取牌张数为7的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBySevenCards(cards);
		break;

	case 8:// 获取牌张数为8的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesByEightCards(cards);
		break;

	case 9:// 获取牌张数为9的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesByNineCards(cards);
		break;

	case 10:// 获取牌张数为10的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesByTenCards(cards);
		break;

	case 11:// 获取牌张数为11的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesByElevenCards(cards);
		break;

	case 12:// 获取牌张数为12的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesByTwelveCards(cards);
		break;

	case 13:// 不存在13张的牌型
		
	case 14:// 获取牌张数为14的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesByForteenCards(cards);
		break;
	case 15:// 获取牌张数为15的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesByFifteenCards(cards);
		break;

	case 16:// 获取牌张数为16的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBySixteenCards(cards);
		break;

	case 17:// 不存在17张的牌型
	
	case 18:// 获取牌张数为18的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesByEighteenCards(cards);
		break;

	case 19:// 不存在19张的牌型
		break;
	case 20:// 获取牌张数为20的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesByTwentyCards(cards);
		break;
	}

	return tmp_moves;
}


/**获取我方出牌为一张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesByOneCard(unsigned *cards)
{
	vector<CARDSMOVE> OneCard_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<15;i++)
	{
		if(tmp_cards[i]>=1)
		{
			CARDSMOVE tmp_vector;
			tmp_vector.cards.push_back(i);
			tmp_vector.cardsType=3;// 在走步后面追加牌型，单牌牌型为3
			OneCard_moves.push_back(tmp_vector);
		}
	}

	return OneCard_moves;
}

/**获取我方出牌为两张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesByTwoCards(unsigned *cards)
{
	vector<CARDSMOVE> TwoCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(tmp_cards[i]>=2)
		{
			CARDSMOVE tmp_vector;
			tmp_vector.cards.push_back(i);
			tmp_vector.cards.push_back(i);
			tmp_vector.cardsType=4;// 在走步后面追加牌型，对牌牌型为4
			TwoCards_moves.push_back(tmp_vector);
		}
	}

	if(tmp_cards[13]==1&&tmp_cards[14]==1)
	{
		CARDSMOVE tmp_vector;
		tmp_vector.cards.push_back(13);
		tmp_vector.cards.push_back(14);
		tmp_vector.cardsType=1;// 在走步后面追加牌型，火箭牌型为1
		TwoCards_moves.push_back(tmp_vector);
	}

	return TwoCards_moves;
}

/**获取我方出牌为三张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesByThreeCards(unsigned *cards)
{
	vector<CARDSMOVE> ThreeCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(tmp_cards[i]>=3)
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=0;tmp<3;tmp++)
			{
				tmp_vector.cards.push_back(i);
			}
			tmp_vector.cardsType=5;// 在走步后面追加牌型，三条牌型为5
			ThreeCards_moves.push_back(tmp_vector);
		}
	}

	return ThreeCards_moves;
}

/**获取我方出牌为四张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesByFourCards(unsigned *cards)
{
	vector<CARDSMOVE> FourCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(tmp_cards[i]==4)// 炸弹
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=0;tmp<4;tmp++)
			{
				tmp_vector.cards.push_back(i);
			}
			tmp_vector.cardsType=2;// 在走步后面追加牌型，炸弹牌型为2
			FourCards_moves.push_back(tmp_vector);
		}

		if(tmp_cards[i]>=3)// 三带一
		{
			CARDSMOVE tmp_vector;
			for(size_t k=0;k<3;k++)
			{
				tmp_vector.cards.push_back(i);
			}
			for(size_t j=0;j<15;j++)
			{
				if(tmp_cards[j]>=1&&j!=i) // 查找手中数量大于等于1的牌型，作为带牌
				{
					tmp_vector.cards.push_back(j);			
				}			
			}
			if(tmp_vector.cards.size()<4)
			{
				continue;
			}
			else
			{
				tmp_vector.cardsType=6;// 在走步后面追加牌型，三带单牌型为6
				FourCards_moves.push_back(tmp_vector);
			}
		}
	}

	return FourCards_moves;
}

/**获取我方出牌为五张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesByFiveCards(unsigned *cards)
{
	vector<CARDSMOVE> FiveCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(HaveSingleJunko(i,5,tmp_cards))// 有i->i+5的单顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+5;tmp++)
			{
				tmp_vector.cards.push_back(tmp);
			}
			tmp_vector.cardsType=8;// 在走步后面追加牌型，单顺牌型为8
			FiveCards_moves.push_back(tmp_vector);
		}

		if(tmp_cards[i]>=3)// 三带一对
		{
			CARDSMOVE tmp_vector;
			for(size_t k=0;k<3;k++)
			{
				tmp_vector.cards.push_back(i);
			}
			for(size_t j=0;j<15;j++)
			{
				if(tmp_cards[j]>=2&&j!=i) // 查找手中数量大于等于1的牌型，作为带牌
				{
					
					tmp_vector.cards.push_back(j);
					tmp_vector.cards.push_back(j);			
				}
			}
			if(tmp_vector.cards.size()<5)
			{
				continue;
			}
			else
			{
				tmp_vector.cardsType=7;// 在走步后面追加牌型，三带双牌型为7
				FiveCards_moves.push_back(tmp_vector);
			}
		}
	}

	return FiveCards_moves;
}

/**获取我方出牌为六张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesBySixCards(unsigned *cards)
{
	vector<CARDSMOVE> SixCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(HaveSingleJunko(i,6,tmp_cards))// 有i->i+6的单顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+6;tmp++)
			{
				tmp_vector.cards.push_back(tmp);
			}
			tmp_vector.cardsType=8;// 在走步后面追加牌型，单顺牌型为8
			SixCards_moves.push_back(tmp_vector);
		}

		if(HaveDualJunko(i,3,tmp_cards))// 有i->i+3的双顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+3;tmp++)
			{
				tmp_vector.cards.push_back(tmp);
				tmp_vector.cards.push_back(tmp);
			}
			tmp_vector.cardsType=9;// 在走步后面追加牌型，双顺牌型为9
			SixCards_moves.push_back(tmp_vector);
		}

		if(tmp_cards[i]==4)// 四带二单
		{
			VECTORINT carryCards;

			for(size_t i1=0;i1<15;i1++)
			{
				if(i1!=i&&tmp_cards[i1]>0)// 对牌以上
				{
					carryCards.push_back(i1);
				}
			}
			if(carryCards.size()<2)
			{
				continue;// 带牌不够
			}

			CARDSMOVE tmp_vector;
			for(size_t tmp=0;tmp<4;tmp++)
			{
				tmp_vector.cards.push_back(i);
			}
				
			for(size_t m=0;m<carryCards.size();m++)
			{
				tmp_vector.cards.push_back(carryCards[m]);
			}

			tmp_vector.cardsType=FOUR_TWO;// 在走步后面追加牌型，四带二对牌型为14
			SixCards_moves.push_back(tmp_vector);
		}
	}

	return SixCards_moves;
}


/**获取我方出牌为七张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesBySevenCards(unsigned *cards)
{
	vector<CARDSMOVE> SevenCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(HaveSingleJunko(i,7,tmp_cards))// 有i->i+7的单顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+7;tmp++)
			{
				tmp_vector.cards.push_back(tmp);
			}
			tmp_vector.cardsType=8;// 在走步后面追加牌型，单顺牌型为8
			SevenCards_moves.push_back(tmp_vector);
		}
	}

	return SevenCards_moves;
}


/**获取我方出牌为八张的走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByEightCards(unsigned *cards)
{
	vector<CARDSMOVE> EightCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(HaveSingleJunko(i,8,tmp_cards))// 有i->i+8的单顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+8;tmp++)
			{
				tmp_vector.cards.push_back(tmp);
			}
			tmp_vector.cardsType=8;// 在走步后面追加牌型，单顺牌型为8
			EightCards_moves.push_back(tmp_vector);
		}

		if(HaveDualJunko(i,4,tmp_cards))// 有i->i+4的双顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+4;tmp++)
			{
				tmp_vector.cards.push_back(tmp);
				tmp_vector.cards.push_back(tmp);
			}
			tmp_vector.cardsType=9;// 在走步后面追加牌型，双顺牌型为9
			EightCards_moves.push_back(tmp_vector);
		}
		if(HaveThreeJunko(i,2,tmp_cards))// 有i->i+2的三顺带单
		{
			
			VECTORINT carryCards;

			for(size_t i1=0;i1<15;i1++)
			{
				if((i1<i||i1>i+1)&&tmp_cards[i1]>0)
				{	
					carryCards.push_back(i1);		
				}
			}
			if(carryCards.size()<2)
			{
				continue;// 带牌不够
			}
			
			CARDSMOVE tmp_vector;
			for(size_t k=i;k<i+2;k++)
			{
				for(size_t tmp=0;tmp<3;tmp++)
				{
					tmp_vector.cards.push_back(k);
				}
			}
			for(size_t m=0;m<carryCards.size();m++)
			{
				tmp_vector.cards.push_back(carryCards[m]);
			}

			tmp_vector.cardsType=11;// 在走步后面追加牌型，三顺带单牌型为11
			EightCards_moves.push_back(tmp_vector);
			
		}

		if(tmp_cards[i]==4)// 四带二对
		{
			VECTORINT carryCards;

			for(size_t i1=0;i1<15;i1++)
			{
				if(i1!=i&&tmp_cards[i1]>1)// 对牌以上
				{
					carryCards.push_back(i1);
				}
			}
			if(carryCards.size()<2)
			{
				continue;// 带牌不够
			}

			CARDSMOVE tmp_vector;
			for(size_t tmp=0;tmp<4;tmp++)
			{
				tmp_vector.cards.push_back(i);
			}
				
			for(size_t m=0;m<carryCards.size();m++)
			{
				tmp_vector.cards.push_back(carryCards[m]);
				tmp_vector.cards.push_back(carryCards[m]);
			}

			tmp_vector.cardsType=14;// 在走步后面追加牌型，四带二对牌型为14
			EightCards_moves.push_back(tmp_vector);
		}
	}
	return EightCards_moves;
}


/**获取我方出牌为九张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesByNineCards(unsigned *cards)
{
	vector<CARDSMOVE> NineCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(HaveSingleJunko(i,9,tmp_cards))// 有i->i+9的单顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+9;tmp++)
			{
				tmp_vector.cards.push_back(tmp);
			}

			tmp_vector.cardsType=8;// 在走步后面追加牌型，单顺牌型为8
			NineCards_moves.push_back(tmp_vector);
		}

		if(HaveThreeJunko(i,3,tmp_cards))// 有i->i+3的三顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+3;tmp++)
			{
				for(size_t k=0;k<3;k++)
				{
					tmp_vector.cards.push_back(i);
				}
			}

			tmp_vector.cardsType=10;// 在走步后面追加牌型，三顺牌型为10
			NineCards_moves.push_back(tmp_vector);
		}
	}

	return NineCards_moves;
}


/**获取我方出牌为十张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesByTenCards(unsigned *cards)
{
	vector<CARDSMOVE> TenCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(HaveSingleJunko(i,10,tmp_cards))// 有i->i+10的单顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+10;tmp++)
			{
				tmp_vector.cards.push_back(tmp);
			}

			tmp_vector.cardsType=8;// 在走步后面追加牌型，单顺牌型为8
			TenCards_moves.push_back(tmp_vector);
		}

		if(HaveDualJunko(i,5,tmp_cards))// 有i->i+5的双顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+5;tmp++)
			{
				for(size_t k=0;k<2;k++)
				{
					tmp_vector.cards.push_back(i);
				}
			}

			tmp_vector.cardsType=9;// 在走步后面追加牌型，双顺牌型为9
			TenCards_moves.push_back(tmp_vector);
		}

		if(HaveThreeJunko(i,2,tmp_cards))// 有i->i+2的三顺带双
		{
			
			VECTORINT carryCards;

			for(size_t i1=0;i1<15;i1++)
			{
				if((i1<i||i1>i+1)&&tmp_cards[i1]>1)// 对牌以上
				{
					carryCards.push_back(i1);
				}
			}

			if(carryCards.size()<2)
			{
				continue;// 带牌不够
			}
			
			CARDSMOVE tmp_vector;
			for(size_t k=i;k<i+2;k++)
			{
				for(size_t tmp=0;tmp<3;tmp++)
				{
					tmp_vector.cards.push_back(k);
				}
			}
			for(size_t m=0;m<carryCards.size();m++)
			{
				tmp_vector.cards.push_back(carryCards[m]);
				tmp_vector.cards.push_back(carryCards[m]);
			}
			tmp_vector.cardsType=12;// 在走步后面追加牌型，三顺带双牌型为12
			TenCards_moves.push_back(tmp_vector);			
		}
	}

	return TenCards_moves;
}


/**获取我方出牌为十一张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesByElevenCards(unsigned *cards)
{
	vector<CARDSMOVE> ElevenCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(HaveSingleJunko(i,11,tmp_cards))// 有i->i+11的单顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+5;tmp++)
			{
				tmp_vector.cards.push_back(tmp);
			}

			tmp_vector.cardsType=8;// 在走步后面追加牌型，单顺牌型为8
			ElevenCards_moves.push_back(tmp_vector);
		}
	}

	return ElevenCards_moves;
}

/**获取我方出牌为十二张的走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByTwelveCards(unsigned *cards)
{
	vector<CARDSMOVE> TwelveCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(HaveSingleJunko(i,12,tmp_cards))// 有i->i+12的单顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+12;tmp++)
			{
				tmp_vector.cards.push_back(tmp);
			}

			tmp_vector.cardsType=8;// 在走步后面追加牌型，单顺牌型为8
			TwelveCards_moves.push_back(tmp_vector);
		}

		if(HaveDualJunko(i,6,tmp_cards))// 有i->i+6的双顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+4;tmp++)
			{
				for(size_t k=0;k<2;k++)
				{
				tmp_vector.cards.push_back(tmp);
				}
			}

			tmp_vector.cardsType=9;// 在走步后面追加牌型，双顺牌型为9
			TwelveCards_moves.push_back(tmp_vector);
		}

		if(HaveThreeJunko(i,4,tmp_cards))// 有i->i+4的三顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+4;tmp++)
			{
				for(size_t k=0;k<3;k++)
				{
					tmp_vector.cards.push_back(tmp);
				}
			}

			tmp_vector.cardsType=10;// 在走步后面追加牌型，三顺牌型为10
			TwelveCards_moves.push_back(tmp_vector);
		}


		if(HaveThreeJunko(i,3,tmp_cards))// 有i->i+3的三顺带单
		{
			
			VECTORINT carryCards;

			for(size_t i1=0;i1<15;i1++)
			{
				if((i1<i||i1>i+2)&&tmp_cards[i1]>0)
				{	
					carryCards.push_back(i1);		
				}
			}
			if(carryCards.size()<3)
			{
				continue;// 带牌不够
			}
			
			CARDSMOVE tmp_vector;
			for(size_t k=i;k<i+3;k++)
			{
				for(size_t tmp=0;tmp<3;tmp++)
				{
					tmp_vector.cards.push_back(k);
				}
			}
			for(size_t m=0;m<carryCards.size();m++)
			{
				tmp_vector.cards.push_back(carryCards[m]);
			}

			tmp_vector.cardsType=11;// 在走步后面追加牌型，三顺带单牌型为11
			TwelveCards_moves.push_back(tmp_vector);
			
		}
	}

	return TwelveCards_moves;
}

/**获取我方出牌为十四张的走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByForteenCards(unsigned *cards)
{
	vector<CARDSMOVE> ForteenCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(HaveDualJunko(i,7,tmp_cards))// 有i->i+7的双顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+7;tmp++)
			{
				tmp_vector.cards.push_back(tmp);
				tmp_vector.cards.push_back(tmp);
			}

			tmp_vector.cardsType=9;// 在走步后面追加牌型，双顺牌型为9
			ForteenCards_moves.push_back(tmp_vector);
		}	

		
	}

	return ForteenCards_moves;
}

/**获取我方出牌为十五张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesByFifteenCards(unsigned *cards)
{
	vector<CARDSMOVE> FifteenCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(HaveThreeJunko(i,5,tmp_cards))// 有i->i+5的三顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+7;tmp++)
			{
				for(size_t k=0;k<3;k++)
				{
				tmp_vector.cards.push_back(tmp);
				}
			}

			tmp_vector.cardsType=10;// 在走步后面追加牌型，三顺牌型为10
			FifteenCards_moves.push_back(tmp_vector);
		}	

		if(HaveThreeJunko(i,3,tmp_cards))// 有i->i+3的三顺带双
		{
			
			VECTORINT carryCards;

			for(size_t i1=0;i1<15;i1++)
			{
				if((i1<i||i1>i+2)&&tmp_cards[i1]>1)// 对牌以上
				{
					carryCards.push_back(i1);
				}
			}

			if(carryCards.size()<3)
			{
				continue;// 带牌不够
			}
			
			CARDSMOVE tmp_vector;
			for(size_t k=i;k<i+3;k++)
			{
				for(size_t tmp=0;tmp<3;tmp++)
				{
					tmp_vector.cards.push_back(k);
				}
			}
			for(size_t m=0;m<carryCards.size();m++)
			{
				tmp_vector.cards.push_back(carryCards[m]);
				tmp_vector.cards.push_back(carryCards[m]);
			}
			tmp_vector.cardsType=12;// 在走步后面追加牌型，三顺带双牌型为12
			FifteenCards_moves.push_back(tmp_vector);			
		}
	}

	return FifteenCards_moves;
}

/**获取我方出牌为十六张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesBySixteenCards(unsigned *cards)
{
	vector<CARDSMOVE> SixteenCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(HaveDualJunko(i,8,tmp_cards))// 有i->i+8的双顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+8;tmp++)
			{
				tmp_vector.cards.push_back(tmp);
				tmp_vector.cards.push_back(tmp);
			}

			tmp_vector.cardsType=9;// 在走步后面追加牌型，双顺牌型为9
			SixteenCards_moves.push_back(tmp_vector);
		}	

		if(HaveThreeJunko(i,4,tmp_cards))// 有i->i+4的三顺带单
		{
			
			VECTORINT carryCards;

			for(size_t i1=0;i1<15;i1++)
			{
				if((i1<i||i1>i+3)&&tmp_cards[i1]>0)
				{	
					carryCards.push_back(i1);		
				}
			}
			if(carryCards.size()<4)
			{
				continue;// 带牌不够
			}
			
			CARDSMOVE tmp_vector;
			for(size_t k=i;k<i+4;k++)
			{
				for(size_t tmp=0;tmp<3;tmp++)
				{
					tmp_vector.cards.push_back(k);
				}
			}
			for(size_t m=0;m<carryCards.size();m++)
			{
				tmp_vector.cards.push_back(carryCards[m]);
			}

			tmp_vector.cardsType=11;// 在走步后面追加牌型，三顺带单牌型为11
			SixteenCards_moves.push_back(tmp_vector);
			
		}
	}

	return SixteenCards_moves;
}


/**获取我方出牌为十八张的走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByEighteenCards(unsigned *cards)
{
	vector<CARDSMOVE> EighteenCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(HaveThreeJunko(i,9,tmp_cards))// 有i->i+9的双顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+9;tmp++)
			{
				tmp_vector.cards.push_back(tmp);
				tmp_vector.cards.push_back(tmp);
			}

			tmp_vector.cardsType=9;// 在走步后面追加牌型，双顺牌型为9
			EighteenCards_moves.push_back(tmp_vector);
		}

		if(HaveThreeJunko(i,6,tmp_cards))// 有i->i+6的三顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+6;tmp++)
			{
				for(size_t k=0;k<3;k++)
				{
					tmp_vector.cards.push_back(tmp);
				}
			}

			tmp_vector.cardsType=10;// 在走步后面追加牌型，三顺牌型为10
			EighteenCards_moves.push_back(tmp_vector);
		}	
	}

	return EighteenCards_moves;
}

/**获取我方出牌为二十张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesByTwentyCards(unsigned *cards)
{
	vector<CARDSMOVE> TwentyCards_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)// 3-A、2
	{
		if(HaveThreeJunko(i,10,tmp_cards))// 有i->i+10的双顺
		{
			CARDSMOVE tmp_vector;
			for(size_t tmp=i;tmp<i+9;tmp++)
			{
				tmp_vector.cards.push_back(tmp);
				tmp_vector.cards.push_back(tmp);
			}

			tmp_vector.cardsType=9;// 在走步后面追加牌型，双顺牌型为9
			TwentyCards_moves.push_back(tmp_vector);
		}

		if(HaveThreeJunko(i,5,tmp_cards))// 有i->i+5的三顺带单
		{
			
			VECTORINT carryCards;

			for(size_t i1=0;i1<15;i1++)
			{
				if((i1<i||i1>i+4)&&tmp_cards[i1]>0)
				{	
					carryCards.push_back(i1);		
				}
			}
			if(carryCards.size()<5)
			{
				continue;// 带牌不够
			}
			
			CARDSMOVE tmp_vector;
			for(size_t k=i;k<i+5;k++)
			{
				for(size_t tmp=0;tmp<3;tmp++)
				{
					tmp_vector.cards.push_back(k);
				}
			}
			for(size_t m=0;m<carryCards.size();m++)
			{
				tmp_vector.cards.push_back(carryCards[m]);
			}

			tmp_vector.cardsType=11;// 在走步后面追加牌型，三顺带单牌型为11
			TwentyCards_moves.push_back(tmp_vector);
			
		}

		if(HaveThreeJunko(i,4,tmp_cards))// 有i->i+4的三顺带双
		{
			
			VECTORINT carryCards;

			for(size_t i1=0;i1<15;i1++)
			{
				if((i1<i||i1>i+3)&&tmp_cards[i1]>1)// 对牌以上
				{
					carryCards.push_back(i1);
				}
			}

			if(carryCards.size()<4)
			{
				continue;// 带牌不够
			}
			
			CARDSMOVE tmp_vector;
			for(size_t k=i;k<i+4;k++)
			{
				for(size_t tmp=0;tmp<3;tmp++)
				{
					tmp_vector.cards.push_back(k);
				}
			}
			for(size_t m=0;m<carryCards.size();m++)
			{
				tmp_vector.cards.push_back(carryCards[m]);
				tmp_vector.cards.push_back(carryCards[m]);
			}
			tmp_vector.cardsType=12;// 在走步后面追加牌型，三顺带双牌型为12
			TwentyCards_moves.push_back(tmp_vector);			
		}
	}

	return TwentyCards_moves;
}

/**		判断是否一副牌有从指定位置的指定数量的单顺			*/
/**   @start 开始位置,最小为0 @ JunkoNum 单顺数量  @ cards[] 牌	*/
bool CMoveGenerator::HaveSingleJunko(unsigned start,unsigned JunkoNum,unsigned *cards)
{
	bool flag = true;

	while(JunkoNum--)
	{
		if(cards[start]<1||start>=12)// 不能超过牌A
		{
			flag = false;
			break;
		}
		
		start++;
	}

	return flag;
}


/**		判断是否一副牌有从指定位置的指定数量的双顺			*/
/**   @start 开始位置,最小为0 @ JunkoNum 双顺数量  @ cards[] 牌	*/
bool CMoveGenerator::HaveDualJunko(unsigned start,unsigned JunkoNum,unsigned *cards)
{
	bool flag = true;

	while(JunkoNum--)
	{
		if(cards[start]<2||start>=12)// 不能超过牌A
		{
			flag = false;
			break;
		}
		
		start++;
	}

	return flag;
}

/**		判断是否一副牌有从指定位置的指定数量的三顺			*/
/**   @start 开始位置,最小为0 @ JunkoNum 三顺数量  @ cards[] 牌	*/
bool CMoveGenerator::HaveThreeJunko(unsigned start,unsigned JunkoNum,unsigned *cards)
{
	bool flag = true;

	while(JunkoNum--)
	{
		if(cards[start]<3||start>=12)// 不能超过牌A
		{
			flag = false;
			break;
		}
		
		start++;
	}

	return flag;
}

/**根据对方炸弹获得所有炸弹走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesByZhaDan(unsigned ZhaDanValue,unsigned *cards)
{
	vector<CARDSMOVE> ZhaDan_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t j=ZhaDanValue+1;j<13;j++)
	{
		if(tmp_cards[j]==4)
		{
			CARDSMOVE tmp_vector;
			for(size_t i=0;i<4;i++)
			{
				tmp_vector.cards.push_back(j);
			}
			tmp_vector.cardsType=2;// 在走步后面追加牌型，炸弹牌型为2
			ZhaDan_moves.push_back(tmp_vector);
		}	
	}

	if(tmp_cards[13]==1&&tmp_cards[14]==1)
	{
		CARDSMOVE tmp_vector;
		tmp_vector.cards.push_back(13);// 小王
		tmp_vector.cards.push_back(14);// 大王

		tmp_vector.cardsType=1;// 在走步后面追加牌型，火箭牌型为1
		ZhaDan_moves.push_back(tmp_vector);
	}

	return ZhaDan_moves;
}

/** 根据对方单牌获得我方所有单牌走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesBySingleCard(unsigned SingleCard,unsigned *cards)
{
	vector<CARDSMOVE> SingleCard_moves;
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=SingleCard+1;i<15;i++)
	{
		if(tmp_cards[i]>=1)
		{
			CARDSMOVE tmp_vector;
			tmp_vector.cards.push_back(i);

			tmp_vector.cardsType=3;// 在走步后面追加牌型，单牌牌型为3
			SingleCard_moves.push_back(tmp_vector);			
		}
	}
	return SingleCard_moves;
}

/** 根据对方对牌获得我方所有对牌走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByCoupleCards(unsigned CoupleCards,unsigned *cards)
{
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));
	vector<CARDSMOVE> CoupleCards_moves;
	for(size_t i=CoupleCards+1;i<15;i++)
	{
		if(tmp_cards[i]>=2)
		{
			CARDSMOVE tmp_vector;
			tmp_vector.cards.push_back(i);
			tmp_vector.cards.push_back(i);

			tmp_vector.cardsType=4;// 在走步后面追加牌型，对牌牌型为4
			CoupleCards_moves.push_back(tmp_vector);
		}
	}

	return CoupleCards_moves;
}

/** 根据对方三条获得我方所有三条走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByThreeTiaoCards(unsigned ThreeCards,unsigned *cards)
{
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));
	vector<CARDSMOVE> ThreeCards_moves;
	for(size_t i=ThreeCards+1;i<15;i++)
	{
		if(tmp_cards[i]>=3)
		{
			CARDSMOVE tmp_vector;
			for(size_t j=0;j<3;j++)
			{
				tmp_vector.cards.push_back(i);
			}

			tmp_vector.cardsType=5;// 在走步后面追加牌型，对牌牌型为5
			ThreeCards_moves.push_back(tmp_vector);
		}
	}

	return ThreeCards_moves;
}

/** 根据对方三带一获得我方所有三带一走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByThree_One(unsigned Three_OneCards,unsigned *cards)
{
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));
	vector<CARDSMOVE> Three_OneCards_moves;
	for(size_t i=Three_OneCards+1;i<15;i++)
	{
		if(tmp_cards[i]>=3)
		{
			CARDSMOVE tmp_vector;
			for(size_t k=0;k<3;k++)
			{
				tmp_vector.cards.push_back(i);
			}
			for(size_t j=0;j<15;j++)
			{
				if(tmp_cards[j]>=1&&j!=i) // 查找手中数量大于等于1的牌型，作为带牌
				{
					tmp_vector.cards.push_back(j);	
				}
			}
			tmp_vector.cardsType=6;// 在走步后面追加牌型，三带单牌型为6
			Three_OneCards_moves.push_back(tmp_vector);
		}
	}

	return Three_OneCards_moves;
}

/** 根据对方三带二获得我方所有三带二走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByThree_Two(unsigned Three_TwoCards,unsigned *cards)
{
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));
	vector<CARDSMOVE> Three_TwoCards_moves;
	for(size_t i=Three_TwoCards+1;i<15;i++)
	{
		if(tmp_cards[i]>=3)
		{
			CARDSMOVE tmp_vector;
			for(size_t k=0;k<3;k++)
			{
				tmp_vector.cards.push_back(i);
			}
			for(size_t j=0;j<15;j++)
			{
				if(tmp_cards[j]>=2&&j!=i) // 查找手中数量大于2的牌型，作为带牌
				{
					tmp_vector.cards.push_back(j);	
					tmp_vector.cards.push_back(j);
				}
			}

			tmp_vector.cardsType=7;// 在走步后面追加牌型，三带双牌型为7
			Three_TwoCards_moves.push_back(tmp_vector);
		}
	}

	return Three_TwoCards_moves;
}

/** 根据对方单顺获得我方所有单顺走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesBySingleJunko(unsigned cardBeginValue,unsigned cardEndValue,unsigned *cards)
{
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));
	vector<CARDSMOVE> SingleJunko_moves;
	unsigned JunkoNum = cardEndValue-cardBeginValue+1;// 单顺牌张数

	for(size_t i=cardBeginValue+1;i+JunkoNum<=11;i++) // 单顺只能开始数最大为10,且从单顺开始加上单顺数量不得大于牌A(11)
	{
		bool flag = true;
		for(size_t j=i;j<i+JunkoNum;j++)
		{
			if(tmp_cards[j]==0)
			{
				flag = false;
				break;
			}
		}
		if(flag)
		{
			CARDSMOVE tmp_vector;
			for(size_t k=i;k<i+JunkoNum;k++)
			{
				tmp_vector.cards.push_back(k);
			}

			tmp_vector.cardsType=8;// 在走步后面追加牌型，单顺牌型为8
			SingleJunko_moves.push_back(tmp_vector);
		}
	}


	return SingleJunko_moves;
}

/** 根据对方双顺获得我方所有双顺走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByDualJunko(unsigned cardBeginValue,unsigned cardEndValue,unsigned *cards)
{
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));
	vector<CARDSMOVE> DualJunko_moves;
	unsigned JunkoNum = cardEndValue-cardBeginValue+1;// 双顺牌对数
	//vector<bool> tmp_vector3;

	for(int i=cardBeginValue+1;i+JunkoNum<=11;i++)// 双顺最大为12( Q )，且从双顺开始加上双顺数量不得大于牌A(11)
	{
		bool flag = true;
		for(size_t j=i;j<i+JunkoNum;j++)
		{
			if(tmp_cards[j]<2)
			{
				flag = false;
				break;
			}
		}
		//tmp_vector3.push_back(flag);

		if(flag)
		{
			CARDSMOVE tmp_vector;
			for(size_t k=i;k<i+JunkoNum;k++)
			{
				for(size_t tmp=0;tmp<2;tmp++)
				{
					tmp_vector.cards.push_back(k);
				}
			}

			tmp_vector.cardsType=9;// 在走步后面追加牌型，双顺牌型为9
			DualJunko_moves.push_back(tmp_vector);
		}
	}


	return DualJunko_moves;
}

/** 根据对方三顺获得我方所有三顺走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByThree_Shun(unsigned cardBeginValue,unsigned cardEndValue,unsigned *cards)
{
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));
	vector<CARDSMOVE> Three_Shun_moves;
	int JunkoNum = cardEndValue-cardBeginValue+1;// 三顺牌个数

	for(int i=cardBeginValue+1;i+JunkoNum<=11;i++)// 三顺开始最大为13（K）如：KKKAAA
	{
		bool flag = true;
		for(size_t j=i;j<i+JunkoNum;j++)
		{
			if(tmp_cards[j]<3)
			{
				flag = false;
				break;
			}
		}
		if(flag)
		{
			CARDSMOVE tmp_vector;
			for(size_t k=i;k<i+JunkoNum;k++)
			{
				for(size_t tmp=0;tmp<3;tmp++)
				{
					tmp_vector.cards.push_back(k);
				}
			}

			tmp_vector.cardsType=10;// 在走步后面追加牌型，三顺牌型为10
			Three_Shun_moves.push_back(tmp_vector);
		}
	}


	return Three_Shun_moves;
}

/** 根据对方三顺带单获得我方所有三顺带单走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByThree_Shun_One(unsigned cardBeginValue,unsigned cardEndValue,unsigned *cards)
{
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));
	vector<CARDSMOVE> Three_Shun_One_moves;
	unsigned JunkoNum = cardEndValue-cardBeginValue+1;// 三顺带单牌个数

	for(size_t i=cardBeginValue+1;i+JunkoNum<=11;i++)// 三顺带单开始最大为13（K）如：KKKAAA34
	{
		bool flag = true;
		for(size_t j=i;j<i+JunkoNum;j++)
		{
			if(tmp_cards[j]<3)
			{
				flag = false;
				break;
			}
		}
		if(flag)
		{ 
			
			VECTORINT carryCards;// 可带牌存储

			for(size_t i1=0;i1<15;i1++)
			{
				if((i1<i||i1>i+JunkoNum-1)&&tmp_cards[i1]>0)
				{
					carryCards.push_back(i1);		
				}
			}

			if(carryCards.size()<JunkoNum)
			{
				continue;// 带牌不够
			}

			CARDSMOVE tmp_vector;
			for(size_t k=i;k<i+JunkoNum;k++)
			{
				for(size_t tmp=0;tmp<3;tmp++)
				{
					tmp_vector.cards.push_back(k);
				}
			}

			for(size_t k=0;k<carryCards.size();k++)
			{
				tmp_vector.cards.push_back(carryCards[k]);
			}
			

			tmp_vector.cardsType=11;// 在走步后面追加牌型，三顺带单牌型为11
			Three_Shun_One_moves.push_back(tmp_vector);
		}
	}

	return Three_Shun_One_moves;
}


/**组合算法，将指定牌进行排列组合 */
vector<unsigned> CMoveGenerator::combinateCards(vector<unsigned> tmp)
{
	for(size_t i=0;i<tmp.size();i++)
	{
		if(tmp.at(i)==0)
		{
			if(i>0)
			{
				if(tmp.at(i-1)==1)
				{
					tmp.at(i)=1;
					tmp.at(i-1)=0;
					int b=0;
					for(size_t j=0;j<i-1;j++)
					{	
						if(tmp.at(j)==1)
						{
							if(tmp.at(b)==0)
							{
								tmp.at(b)=1;
								b++;
								tmp.at(j)=0;
							}
							else
							{
								b++;
							}
						}
					}
					break;
				}
			}
		}
	}

	return tmp;
}

/** 根据对方三顺带双获得我方所有三顺带双走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByThree_Shun_Couple(unsigned cardBeginValue,unsigned cardEndValue,unsigned *cards)
{
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));
	vector<CARDSMOVE> Three_Shun_Couple_moves;
	unsigned JunkoNum = cardEndValue-cardBeginValue+1;// 三顺牌个数

	for(int i=cardBeginValue+1;i<=10&&i+JunkoNum<=12;i++)// 三顺开始最大为13（K）如：KKKAAA3344
	{
		bool flag = true;
		for(size_t j=i;j<i+JunkoNum;j++)
		{
			if(tmp_cards[j]<3)
			{
				flag = false;
				break;
			}
		}
		if(flag)
		{	
			VECTORINT carryCards;// 可带牌存储
			
			for(size_t i1=0;i1<15;i1++)
			{
				if((i1<i||i1>i+JunkoNum-1)&&tmp_cards[i1]>1)
				{
					carryCards.push_back(i1);
				}
			}

			if(carryCards.size()<JunkoNum)
			{
				continue;// 带牌不够
			}

			CARDSMOVE tmp_vector;
			for(size_t k=i;k<i+JunkoNum;k++)
			{
				for(size_t tmp=0;tmp<3;tmp++)
				{
					tmp_vector.cards.push_back(k);
				}
			}
			for(size_t k=0;k<carryCards.size();k++)
			{
				tmp_vector.cards.push_back(carryCards[k]);
				tmp_vector.cards.push_back(carryCards[k]);
			}

			tmp_vector.cardsType=12;// 在走步后面追加牌型，三顺带双牌型为12
			Three_Shun_Couple_moves.push_back(tmp_vector);
			}
	}


	return Three_Shun_Couple_moves;
}

/** 根据对方四带二单获得我方所有四带二单走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByFour_Two(unsigned Four_TwoCards,unsigned *cards)
{
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));
	vector<CARDSMOVE> Four_Two_moves;
	for(size_t i=Four_TwoCards+1;i<13;i++)
	{
		if(tmp_cards[i]==4)
		{
			VECTORINT carryCards;// 可带牌存储


			for(size_t i1=0;i1<15;i1++)
			{
				if(i1!=i&&tmp_cards[i1]>0)// 单牌及以上
				{
					for(size_t tmp=0;tmp<tmp_cards[i1];tmp++)
					{
						carryCards.push_back(i1);
					}
				}
			}

			if(carryCards.size()<2)
			{
				continue;// 带牌不够
			}

			CARDSMOVE tmp_vector;
			for(size_t tmp=0;tmp<4;tmp++)
			{
				tmp_vector.cards.push_back(i);
			}

			for(size_t k=0;k<carryCards.size();k++)
			{
				tmp_vector.cards.push_back(carryCards[k]);
			}

			tmp_vector.cardsType=13;// 在走步后面追加牌型，四带二单牌型为13
			Four_Two_moves.push_back(tmp_vector);		
		}
	}

	return Four_Two_moves;
}

/** 根据对方四带二双获得我方所有四带二双走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByFour_TwoCouple(unsigned Four_TwoCoupleCards,unsigned *cards)
{
	unsigned tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));
	vector<CARDSMOVE> Four_TwoCouple_moves;
	for(size_t i=Four_TwoCoupleCards+1;i<15;i++)
	{
		if(tmp_cards[i]==4)
		{
			VECTORINT carryCards;// 可带牌存储

			for(size_t i1=0;i1<15;i1++)
			{
				if(i1!=i&&tmp_cards[i1]>1)
				{
					carryCards.push_back(i1);
				}
			}

			if(carryCards.size()<2)
			{
				continue;// 带牌不够
			}
		
			CARDSMOVE tmp_vector;
			for(size_t tmp=0;tmp<4;tmp++)
			{
				tmp_vector.cards.push_back(i);
			}
						
			for(size_t k=0;k<carryCards.size();k++)
			{
				tmp_vector.cards.push_back(carryCards[k]);
				tmp_vector.cards.push_back(carryCards[k]);
			}

			tmp_vector.cardsType=14;// 在走步后面追加牌型，四带二双牌型为14
			Four_TwoCouple_moves.push_back(tmp_vector);
		}
	}
	return Four_TwoCouple_moves;
}


//MOVES CMoveGenerator::getBasicMovesByMyself(Player p,int basicIndex)
//{
//	MOVES moves =getMovesByMyself(p.p3_cardsList.size(),p.p3_EachCardNum);
//	MOVES singleOrSingleJunko;
//	MOVES coupleOrDualJunko;
//	MOVES threeOrThreeJunko;
//
//	for(size_t i=0;i<moves.size();i++)
//	{
//		vector<int> tmp_move=moves.at(i);
//
//		if(tmp_move.at(tmp_move.size()-1)==3||tmp_move.at(tmp_move.size()-1)==8)
//		{
//			singleOrSingleJunko.push_back(tmp_move);
//		}
//		else if(tmp_move.at(tmp_move.size()-1)==4||tmp_move.at(tmp_move.size()-1)==9)
//		{
//			coupleOrDualJunko.push_back(tmp_move);
//		}
//		else if(tmp_move.at(tmp_move.size()-1)==5||tmp_move.at(tmp_move.size()-1)==6||tmp_move.at(tmp_move.size()-1)==7||tmp_move.at(tmp_move.size()-1)==11||tmp_move.at(tmp_move.size()-1)==12)
//		{
//			threeOrThreeJunko.push_back(tmp_move);
//		}
//	}
//
//	if(basicIndex ==1)
//	{
//		return singleOrSingleJunko;
//	}
//	else if(basicIndex ==2)
//	{
//		return coupleOrDualJunko;
//	}
//	else
//	{
//		return threeOrThreeJunko;
//	}
//}

/**
*	两种走步能否管上（即一种走步能否大过另一种走步）
*	@m1 一种走步
*  @m2 能否管上m1的走步
*
*/
bool CMoveGenerator::IsValidMove(CARDSMOVE m1,CARDSMOVE m2)
{
	DDZPlayer* play;
	play=(DDZPlayer*)malloc(sizeof(DDZPlayer));

	int cardsType1 = m1.cardsType;
	int cardsType2 = m2.cardsType;

	vector<unsigned> cards1 = m1.cards;
	vector<unsigned> cards2 = m2.cards;

	if(cardsType1==cardsType2)
	{
		switch(cardsType1)
		{
		case INVALID:
			{
				return false;
				break;
			}
		case ROCKET:
			{
				return false;
			break;
			}
		case ZHADAN:
			{
				unsigned zhaDanValue1 =play->getZhaDanValue(cards1);
				unsigned zhaDanValue2 =play->getZhaDanValue(cards2);

				if(zhaDanValue1<zhaDanValue2)
				{
					return true;
				}
				else
				{
					return false;
				}
			break;
			}
		case SINGLE:
			{
				unsigned singleValue1 =play->getSingleValue(cards1);
				unsigned singleValue2 =play->getSingleValue(cards2);

				if(singleValue1<singleValue2)
				{
					return true;
				}
				else
				{
					return false;
				}
			break;
			}
		
		case COUPLE:
			{
				unsigned coupleValue1 =play->getCoupleValue(cards1);
				unsigned coupleValue2 =play->getCoupleValue(cards2);

				if(coupleValue1<coupleValue2)
				{
					return true;
				}
				else
				{
					return false;
				}
			break;
			} 
	
		case SANTIAO:
			{
				unsigned threeValue1 =play->getSanTiaoValue(cards1);
				unsigned threeValue2 =play->getSanTiaoValue(cards2);

				if(threeValue1<threeValue2)
				{
					return true;
				}
				else
				{
					return false;
				}
			break;
			}
	
		case SINGLEJUNKO:
			{
					vector<unsigned> cardsInfo1=play->getSingleJunkoValue(cards1);
					vector<unsigned> cardsInfo2=play->getSingleJunkoValue(cards2);
					
					return IsValidJunkoMove(cardsInfo1,cardsInfo2);
				break;
				}
		
		case DUALJUNKO:
			{
					vector<unsigned> cardsInfo1=play->getDualJunkoValue(cards1);
					vector<unsigned> cardsInfo2=play->getDualJunkoValue(cards2);
					
					return IsValidJunkoMove(cardsInfo1,cardsInfo2);
				break;
				}
		
		case THREEJUNKO:
			{
					vector<unsigned> cardsInfo1=play->getThree_ShunValue(cards1);
					vector<unsigned> cardsInfo2=play->getThree_ShunValue(cards2);

					return IsValidJunkoMove(cardsInfo1,cardsInfo2);
				break;
				}

		case THREEJUNKO_ONE:
			{
					vector<unsigned> cardsInfo1=play->getThree_OneValue(cards1);
					vector<unsigned> cardsInfo2=play->getThree_OneValue(cards2);
					
					return IsValidJunkoMove(cardsInfo1,cardsInfo2);
					break;
					}
		case THREEJUNKO_TWO:
			{
				vector<unsigned> cardsInfo1=play->getThree_TwoValue(cards1);
				vector<unsigned> cardsInfo2=play->getThree_TwoValue(cards2);

				return IsValidJunkoMove(cardsInfo1,cardsInfo2);
				break;
			}
		case FOUR_TWO:
			{
				vector<unsigned> cardsInfo1 = play->getFour_TwoValue(cards1);
				vector<unsigned> cardsInfo2= play->getFour_TwoValue(cards2);

				unsigned fourValue1=cardsInfo1.at(0);
				unsigned fourValue2=cardsInfo2.at(0);

				if(fourValue1<fourValue2)
				{
					return true;
				}
				else
				{
					return false;
				}
				break;
			}

		case FOUR_TWO_COUPLE:
			{
				vector<unsigned> cardsInfo1 = play->getFour_TwoCoupleValue(cards1);
				vector<unsigned> cardsInfo2= play->getFour_TwoCoupleValue(cards2);

				unsigned fourValue1=cardsInfo1.at(0);
				unsigned fourValue2=cardsInfo2.at(0);

				if(fourValue1<fourValue2)
				{
					return true;
				}
				else
				{
					return false;
				}

				break;
			}

		default:
			return false;

		}
	}

	if(cardsType1!=2&&cardsType1!=1&&(cardsType2==2||cardsType2==1))
	{
		return true;
	}

	return false;
}

bool CMoveGenerator::IsValidJunkoMove(vector<unsigned> cardsInfo1,vector<unsigned> cardsInfo2)
{
	unsigned startValue1 = cardsInfo1[0];
	unsigned endValue1 = cardsInfo1[1];

	unsigned startValue2 = cardsInfo2[0];
	unsigned endValue2 = cardsInfo2[1];

	unsigned junkoNum1 = endValue1-startValue1+1;
	unsigned junkoNum2 = endValue2-startValue2+1;

	if(junkoNum1!=junkoNum2)
	{
			return false;
	}
	else if(startValue1>startValue2)
	{
			return false;
	}
	else
	{
		return true;
	}
}


/**
*	组合库入口
*   按牌型优先级进行组合
*	目前引入 8 种组合
*/
void CMoveGenerator::PriorityCombinatoriaLibrary(Player p)
{

	CombsLib combs;
	int len;
	initP3_IsExistCardsType(p);
	vector<CARDSMOVE> p3_moves = p.p3_allMoves;
	int cardsNum=p.p3_cardsNum;
	unsigned tmp_EachCardsNum[15];
	memcpy(tmp_EachCardsNum,p.p3_EachCardNum,sizeof(tmp_EachCardsNum));

	vector<unsigned> v_cardsTypes;
		#pragma region  组合1
	v_cardsTypes.push_back(ROCKET); 
	v_cardsTypes.push_back(ZHADAN); 
	v_cardsTypes.push_back(THREEJUNKO_ONE); 
	v_cardsTypes.push_back(THREEJUNKO_TWO); 
	v_cardsTypes.push_back(DUALJUNKO); 
	v_cardsTypes.push_back(SINGLEJUNKO); 
	v_cardsTypes.push_back(THREE_ONE); 
	v_cardsTypes.push_back(THREE_TWO); 
	v_cardsTypes.push_back(THREEJUNKO); 
	v_cardsTypes.push_back(SANTIAO); 
	v_cardsTypes.push_back(COUPLE); 
	v_cardsTypes.push_back(SINGLE); 
	v_cardsTypes.push_back(FOUR_TWO); 
	v_cardsTypes.push_back(FOUR_TWO_COUPLE); 
	combs.push_back(comb(p,v_cardsTypes));
	v_cardsTypes.clear();
	p.p3_cardsNum=cardsNum;
	p.p3_allMoves=p3_moves;
	memcpy(p.p3_EachCardNum,tmp_EachCardsNum,sizeof(p.p3_EachCardNum));
	#pragma endregion

		#pragma region  组合2
	v_cardsTypes.push_back(ROCKET); 
	v_cardsTypes.push_back(ZHADAN); 
	v_cardsTypes.push_back(THREEJUNKO_TWO); 
	v_cardsTypes.push_back(THREEJUNKO_ONE); 
	v_cardsTypes.push_back(DUALJUNKO); 
	v_cardsTypes.push_back(SINGLEJUNKO); 
	v_cardsTypes.push_back(THREE_ONE); 
	v_cardsTypes.push_back(THREE_TWO); 
	v_cardsTypes.push_back(THREEJUNKO); 
	v_cardsTypes.push_back(SANTIAO); 
	v_cardsTypes.push_back(COUPLE); 
	v_cardsTypes.push_back(SINGLE); 
	v_cardsTypes.push_back(FOUR_TWO); 
	v_cardsTypes.push_back(FOUR_TWO_COUPLE); 
	combs.push_back(comb(p,v_cardsTypes));
	v_cardsTypes.clear();
	p.p3_cardsNum=cardsNum;
	p.p3_allMoves=p3_moves;
	memcpy(p.p3_EachCardNum,tmp_EachCardsNum,sizeof(p.p3_EachCardNum));
	#pragma endregion

		#pragma region 组合3
	v_cardsTypes.push_back(ROCKET); 
	v_cardsTypes.push_back(ZHADAN); 
	v_cardsTypes.push_back(SINGLEJUNKO); 
	v_cardsTypes.push_back(THREEJUNKO_ONE); 
	v_cardsTypes.push_back(THREEJUNKO_TWO); 
	v_cardsTypes.push_back(DUALJUNKO); 
	v_cardsTypes.push_back(THREE_ONE); 
	v_cardsTypes.push_back(THREE_TWO); 
	v_cardsTypes.push_back(THREEJUNKO); 
	v_cardsTypes.push_back(SANTIAO); 
	v_cardsTypes.push_back(COUPLE); 
	v_cardsTypes.push_back(SINGLE); 
	v_cardsTypes.push_back(FOUR_TWO); 
	v_cardsTypes.push_back(FOUR_TWO_COUPLE); 
	combs.push_back(comb(p,v_cardsTypes));
	v_cardsTypes.clear();
	p.p3_cardsNum=cardsNum;
	p.p3_allMoves=p3_moves;
	memcpy(p.p3_EachCardNum,tmp_EachCardsNum,sizeof(p.p3_EachCardNum));
	#pragma endregion

		#pragma region 组合4
	v_cardsTypes.push_back(ROCKET); 
	v_cardsTypes.push_back(ZHADAN); 
	v_cardsTypes.push_back(DUALJUNKO); 
	v_cardsTypes.push_back(SINGLEJUNKO); 
	v_cardsTypes.push_back(THREEJUNKO_TWO); 
	v_cardsTypes.push_back(THREEJUNKO_ONE); 
	v_cardsTypes.push_back(THREE_ONE); 
	v_cardsTypes.push_back(THREE_TWO); 
	v_cardsTypes.push_back(THREEJUNKO); 
	v_cardsTypes.push_back(SANTIAO); 
	v_cardsTypes.push_back(COUPLE); 
	v_cardsTypes.push_back(SINGLE); 
	v_cardsTypes.push_back(FOUR_TWO); 
	v_cardsTypes.push_back(FOUR_TWO_COUPLE); 
	combs.push_back(comb(p,v_cardsTypes));
	v_cardsTypes.clear();
	p.p3_cardsNum=cardsNum;
	p.p3_allMoves=p3_moves;
	memcpy(p.p3_EachCardNum,tmp_EachCardsNum,sizeof(p.p3_EachCardNum));
	#pragma endregion

		#pragma region 组合5
	v_cardsTypes.push_back(ROCKET); 
	v_cardsTypes.push_back(ZHADAN); 
	v_cardsTypes.push_back(THREEJUNKO_ONE); 
	v_cardsTypes.push_back(THREEJUNKO_TWO); 
	v_cardsTypes.push_back(THREE_ONE); 
	v_cardsTypes.push_back(THREE_TWO); 
	v_cardsTypes.push_back(DUALJUNKO); 
	v_cardsTypes.push_back(SINGLEJUNKO);
	v_cardsTypes.push_back(THREEJUNKO); 
	v_cardsTypes.push_back(SANTIAO); 
	v_cardsTypes.push_back(FOUR_TWO); 
	v_cardsTypes.push_back(FOUR_TWO_COUPLE); 
	v_cardsTypes.push_back(COUPLE); 
	v_cardsTypes.push_back(SINGLE); 
	
	combs.push_back(comb(p,v_cardsTypes));
	v_cardsTypes.clear();
	p.p3_cardsNum=cardsNum;
	p.p3_allMoves=p3_moves;
	memcpy(p.p3_EachCardNum,tmp_EachCardsNum,sizeof(p.p3_EachCardNum));
	#pragma endregion

		#pragma region 组合6
	v_cardsTypes.push_back(ROCKET); 
	v_cardsTypes.push_back(ZHADAN); 
	v_cardsTypes.push_back(THREEJUNKO_TWO); 
	v_cardsTypes.push_back(THREEJUNKO_ONE); 
	v_cardsTypes.push_back(DUALJUNKO); 
	v_cardsTypes.push_back(SINGLEJUNKO); 
	v_cardsTypes.push_back(THREE_ONE); 
	v_cardsTypes.push_back(THREE_TWO); 
	v_cardsTypes.push_back(THREEJUNKO); 
	v_cardsTypes.push_back(SANTIAO); 
	v_cardsTypes.push_back(COUPLE); 
	v_cardsTypes.push_back(SINGLE); 
	v_cardsTypes.push_back(FOUR_TWO); 
	v_cardsTypes.push_back(FOUR_TWO_COUPLE); 
	combs.push_back(comb(p,v_cardsTypes));
	v_cardsTypes.clear();
	p.p3_cardsNum=cardsNum;
	p.p3_allMoves=p3_moves;
	memcpy(p.p3_EachCardNum,tmp_EachCardsNum,sizeof(p.p3_EachCardNum));
	#pragma endregion

		#pragma region 组合7
	v_cardsTypes.push_back(ROCKET); 
	v_cardsTypes.push_back(FOUR_TWO); 
	v_cardsTypes.push_back(ZHADAN); 
	v_cardsTypes.push_back(THREEJUNKO_ONE);
	v_cardsTypes.push_back(THREE_ONE);
	v_cardsTypes.push_back(SINGLEJUNKO);
	v_cardsTypes.push_back(THREEJUNKO_TWO); 
	v_cardsTypes.push_back(THREEJUNKO); 
	 v_cardsTypes.push_back(THREE_TWO); 
	v_cardsTypes.push_back(DUALJUNKO); 
	v_cardsTypes.push_back(SANTIAO); 
	v_cardsTypes.push_back(COUPLE); 
	v_cardsTypes.push_back(SINGLE); 
	v_cardsTypes.push_back(FOUR_TWO_COUPLE); 
	combs.push_back(comb(p,v_cardsTypes));
	v_cardsTypes.clear();
	p.p3_cardsNum=cardsNum;
	p.p3_allMoves=p3_moves;
	memcpy(p.p3_EachCardNum,tmp_EachCardsNum,sizeof(p.p3_EachCardNum));
	#pragma endregion

		#pragma region 组合8
	v_cardsTypes.push_back(ROCKET); 
	v_cardsTypes.push_back(ZHADAN); 
	v_cardsTypes.push_back(THREEJUNKO_ONE); 
	v_cardsTypes.push_back(THREEJUNKO_TWO); 
	v_cardsTypes.push_back(THREE_TWO); 
	v_cardsTypes.push_back(THREE_ONE); 
	v_cardsTypes.push_back(DUALJUNKO); 
	v_cardsTypes.push_back(SINGLEJUNKO);
	v_cardsTypes.push_back(THREEJUNKO); 
	v_cardsTypes.push_back(SANTIAO); 
	v_cardsTypes.push_back(FOUR_TWO); 
	v_cardsTypes.push_back(FOUR_TWO_COUPLE); 
	v_cardsTypes.push_back(COUPLE); 
	v_cardsTypes.push_back(SINGLE); 
	
	combs.push_back(comb(p,v_cardsTypes));
	v_cardsTypes.clear();
	p.p3_cardsNum=cardsNum;
	p.p3_allMoves=p3_moves;
	memcpy(p.p3_EachCardNum,tmp_EachCardsNum,sizeof(p.p3_EachCardNum));
	#pragma endregion
	


	FileWriter fw = FileWriter("comb.txt");
	fw.CombsToFile(combs);

	p.p3_combs.clear();
	p.p3_combs=combs;
}

/**
*	根据一种牌型优先级序列获得一种组合
*   @v_cardsTypes  牌型优先级序列
*/
Comb CMoveGenerator::comb(Player p,vector<unsigned> v_cardsTypes)
{
	Comb comb;
	
	for(int i=0;i<v_cardsTypes.size();i++)
	{
		if(p.p3_IsExistCardsType[v_cardsTypes.at(i)-1])
		{
			combByCardsType(p,&comb,v_cardsTypes.at(i));
		}
	}
	
	return comb;
}

/**
*	按指定牌型获取走步中指定牌型所有走步
*   @cardsType 指定牌型
*/
void CMoveGenerator::combByCardsType(Player p,Comb *comb,int cardsType)
{
	for(int i=p.p3_allMoves.size()-1;i>=0;i--)
	{
		if(i>=0&&i<p.p3_allMoves.size()&&p.p3_allMoves.at(i).cardsType==cardsType)
		{
			comb->moves.push_back(p.p3_allMoves[i]);
			for(size_t j=0;j<p.p3_allMoves[i].cards.size();j++)
			{
				p.p3_EachCardNum[p.p3_allMoves[i].cards[j]]--;
			}
			p.p3_cardsNum-=p.p3_allMoves[i].cards.size();
			p.p3_allMoves=getMovesByMyself(p.p3_cardsNum,p.p3_EachCardNum);
			if(p.p3_allMoves.size()>0&&cardsType!=ROCKET)
			{
				i=p.p3_allMoves.size();
				continue;
			}
		}
	}
}

void CMoveGenerator::initP3_IsExistCardsType(Player p)
{
	vector<CARDSMOVE> moves = p.p3_allMoves;

	for(size_t i=0;i<moves.size();i++)
	{
		if(moves.at(i).cardsType==ROCKET)
		{
			p.p3_IsExistCardsType[ROCKET-1]=1;
		}
		else if(moves.at(i).cardsType==ZHADAN)
		{
			p.p3_IsExistCardsType[ZHADAN-1]=1;
		}
		else if(moves.at(i).cardsType==SINGLE)
		{
			p.p3_IsExistCardsType[SINGLE-1]=1;
		}
		else if(moves.at(i).cardsType==COUPLE)
		{
			p.p3_IsExistCardsType[COUPLE-1]=1;
		}
		else if(moves.at(i).cardsType==SANTIAO)
		{
			p.p3_IsExistCardsType[SANTIAO-1]=1;
		}
		else if(moves.at(i).cardsType==THREE_ONE)
		{
			p.p3_IsExistCardsType[THREE_ONE-1]=1;
		}
		else if(moves.at(i).cardsType==THREE_TWO)
		{
			p.p3_IsExistCardsType[THREE_TWO-1]=1;
		}
		else if(moves.at(i).cardsType==SINGLEJUNKO)
		{
			p.p3_IsExistCardsType[SINGLEJUNKO-1]=1;
		}
		else if(moves.at(i).cardsType==DUALJUNKO)
		{
			p.p3_IsExistCardsType[DUALJUNKO-1]=1;
		}
		else if(moves.at(i).cardsType==THREEJUNKO)
		{
			p.p3_IsExistCardsType[THREEJUNKO-1]=1;
		}
		else if(moves.at(i).cardsType==THREEJUNKO_ONE)
		{
			p.p3_IsExistCardsType[THREEJUNKO_ONE-1]=1;
		}
		else if(moves.at(i).cardsType==THREEJUNKO_TWO)
		{
			p.p3_IsExistCardsType[THREEJUNKO_TWO-1]=1;
		}
		else if(moves.at(i).cardsType==FOUR_TWO)
		{
			p.p3_IsExistCardsType[FOUR_TWO-1]=1;
		}
		else
		{
			p.p3_IsExistCardsType[FOUR_TWO_COUPLE-1]=1;
		}
	}
}