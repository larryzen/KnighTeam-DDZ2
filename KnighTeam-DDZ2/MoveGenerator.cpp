#include "MoveGenerator.h"
#include "FileWriter.h"
#include "string.h"
#include "stdlib.h"
#include "vector"
#include "DDZCombFactory.h"

using namespace std;
CMoveGenerator::CMoveGenerator(void)
{
}

CMoveGenerator::~CMoveGenerator(void)
{
}

/**
*   走法生成器入口，分为主动和被动出牌
*  (1)我方为地主，且三个玩家都尚未出牌; (2)上下两家均pass;->主动出牌，代号1
*  (1)上家未pass,按上家出牌; (2)上家pass,下家未pass,按下家出牌;->被动出牌，代号2
*/
vector<CARDSMOVE> CMoveGenerator::getMoves(int whoseGo)
{
	vector<CARDSMOVE> moves = vector<CARDSMOVE>();
	bool IsCanGo = false;
	int turn =whoseGo%3;												 // 轮到哪方出牌，则进行该方主动、被动出牌判断
	unsigned tmp_EachCardNum[15];										 // 根据哪方的牌来获得走步
	int cardsNum;														 //  玩家手中牌数量
	CARDSMOVE lastMove;													 //	 当前玩家的上家上次出牌牌面值
	DDZCombFactory ddz_CF;
	int size = Player::cardsMoveRecords.size();
	
	
	if (Player::cardsMoveRecords.empty())
	{
		if (Player::p3_IsLandlord)
		{
			IsCanGo = true;
		}
	}
	else
	{
		if (size - 2 >= 0 && Player::cardsMoveRecords[size - 1].cardsType == PASS
			&& Player::cardsMoveRecords[size - 2].cardsType == PASS)
		{
			IsCanGo = true;
		}

		if (size - 1 >=0 && Player::cardsMoveRecords[size - 1].cardsType != PASS)
		{
			lastMove = Player::cardsMoveRecords[size - 1];

		}
		else if (size - 2 >= 0 && Player::cardsMoveRecords[size - 2].cardsType != PASS)
		{
			lastMove = Player::cardsMoveRecords[size - 2];
		}
	}

		
	
	if (turn == 1)
	{
		if (IsCanGo)
		{
			memcpy(tmp_EachCardNum, Player::p1_EachCardNum, sizeof(tmp_EachCardNum));
		}
		else
		{
			memcpy(tmp_EachCardNum, Player::p1_EachCardNum, sizeof(tmp_EachCardNum));
		}
		
		cardsNum = Player::p1_cardsNum;
	}
	else if (turn ==2)
	{
		if (IsCanGo)
		{
			memcpy(tmp_EachCardNum, Player::p2_EachCardNum, sizeof(tmp_EachCardNum));
		}
		else
		{
			memcpy(tmp_EachCardNum, Player::p2_EachCardNum, sizeof(tmp_EachCardNum));
		}
		
		cardsNum = Player::p2_cardsNum;
	}
	else
	{
		memcpy(tmp_EachCardNum, Player::p3_EachCardNum, sizeof(tmp_EachCardNum));
		cardsNum = Player::p3_cardsNum;
	}



	if(IsCanGo)	// 主动出牌
	{	
		ddz_CF = DDZCombFactory(tmp_EachCardNum, cardsNum);
		Comb c1 = ddz_CF.getComb1LeastSingle();

		moves = c1.moves;
		ddz_CF.setCarryCards1(&moves);
		ddz_CF.setCarryCards3(&moves);
		ddz_CF.setCarryCards4(&moves);
		if (CThinkTable::IsHalfGame())//终局模拟处理
		{
			FinalMovesDeal(&moves);
		}
		
	
		
		for (size_t j = 0; j<moves.size(); j++)
		{
			moves[j].outWay = 1;
			moves[j].side = turn;
		}
	}
	else	   // 被动出牌(按其他玩家上一轮出牌走步)
	{
		
		ddz_CF = DDZCombFactory(tmp_EachCardNum, cardsNum);
		vector<CARDSMOVE> tmp_moves = ddz_CF.getComb1LeastSingle().moves;
		GeneralSplitDeal(&tmp_moves, tmp_EachCardNum);
		ddz_CF.setCarryCards1(&tmp_moves);
		ddz_CF.setCarryCards3(&tmp_moves);
		ddz_CF.setCarryCards4(&tmp_moves);
		moves = getMovesByCombMovesForOneMove(lastMove, tmp_moves);
			
		for (size_t j = 0; j<moves.size(); j++)
		{
			moves.at(j).outWay = 0;
			moves[j].side = turn;
		}
	}
	return moves;
}

/**
*	比较常见的拆牌处理
*
*/
void CMoveGenerator::GeneralSplitDeal(vector<CARDSMOVE> *moves, unsigned *EachCardsNum)
{
	int cardsType = INVALID;
	CARDSMOVE m;
	int key_single = -1;
	int top = D;
	while (!EachCardsNum[top])
	{
		top--;
	}
	for (int i = moves->size() - 1; i >= 0; i--)
	{
		cardsType = moves->at(i).cardsType;
		// 拆火箭
		if (cardsType == ROCKET)
		{
			m = CARDSMOVE();
			VectorUtil::addElement(&m.cards, X, 1);
			m.cardsType = SINGLE;
			moves->push_back(m);

			m = CARDSMOVE();
			VectorUtil::addElement(&m.cards, D, 1);
			m.cardsType = SINGLE;
			moves->push_back(m);
		}

		if ((cardsType == COUPLE || cardsType == SANTIAO) && moves->at(i).cards[0] == top)
		{
			m = CARDSMOVE();
			VectorUtil::addElement(&m.cards, top, 1);
			m.cardsType = SINGLE;

			moves->push_back(m);

			if (cardsType == SANTIAO)
			{
				m = CARDSMOVE();
				VectorUtil::addElement(&m.cards, top, 2);

				m.cardsType = COUPLE;

				moves->push_back(m);
			}
		}
		//将2 的对牌，三条, 炸弹拆开
		if ((cardsType == COUPLE || cardsType == SANTIAO || cardsType==ZHADAN) && moves->at(i).cards[0]==T)
		{
			m = CARDSMOVE();
			VectorUtil::addElement(&m.cards, moves->at(i).cards[0], 1);
			m.cardsType = SINGLE;

			moves->push_back(m);

			if (cardsType == SANTIAO)
			{
				m = CARDSMOVE();
				VectorUtil::addElement(&m.cards, moves->at(i).cards[0], 2);
				
				m.cardsType = COUPLE;

				moves->push_back(m);
			}

			if (cardsType == ZHADAN)
			{
				m = CARDSMOVE();
				VectorUtil::addElement(&m.cards, moves->at(i).cards[0], 3);
				m.cardsType = SANTIAO;
				moves->push_back(m);
			}
		}
	}
}
void CMoveGenerator::FinalMovesDeal(vector<CARDSMOVE> *moves)
{
	int cardsType = INVALID;
	CARDSMOVE m;
	for (int i = moves->size() - 1; i >= 0; i--)
	{
		cardsType = moves->at(i).cardsType;
		if (cardsType == COUPLE || cardsType == SANTIAO)
		{
			m = CARDSMOVE();
			m.cards.push_back(moves->at(i).cards[0]);
			m.cardsType = SINGLE;

			moves->push_back(m);

			if (cardsType == SANTIAO)
			{
				m = CARDSMOVE();
				m.cards.push_back(moves->at(i).cards[0]);
				m.cards.push_back(moves->at(i).cards[0]);
				m.cardsType = COUPLE;

				moves->push_back(m);
			}
		}
	}
}

vector<CARDSMOVE> CMoveGenerator::getMovesByCombMovesForOneMove(CARDSMOVE key, vector<CARDSMOVE> moves)
{
	vector<CARDSMOVE> keyMoves = vector<CARDSMOVE>();
	for (size_t i = 0; i < moves.size(); i++)
	{
		if (IsValidMove(key, moves[i]))
		{
			moves[i].outWay=0;
			keyMoves.push_back(moves[i]);
		}
			
	}

	CARDSMOVE pass = CARDSMOVE();
	pass.cardsType = PASS;
	keyMoves.push_back(pass);

	return keyMoves;
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
				tmp_moves = (vector<CARDSMOVE>)getMovesByType02ZhaDan(ZhaDanValue, cards);
				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}				
			break;
			}
				
		case 3:// 单牌
			{
				int SingleCardValue = play->getSingleValue(v);// 获取单牌的牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByType03SingleCard(SingleCardValue, cards);
				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}			
			break;
			}

		case 4:// 对牌
			{
				int CoupleCardsValue = play->getCoupleValue(v);// 获取对牌的牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByType04CoupleCards(CoupleCardsValue, cards);
				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}				
			break;
			}

		case 5:// 三条
			{
				int ThreeCardsValue = play->getSanTiaoValue(v);// 获取三条的牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByType05ThreeTiaoCards(ThreeCardsValue, cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}

		case 6:// 三带单
			{
				int Three_OneCardsValue = play->getThree_OneValue(v).at(0);// 获取三带单的牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByType06Three_One(Three_OneCardsValue, cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}

		case 7:// 三带双
			{
				int Three_TwoCardsValue = play->getThree_TwoValue(v).at(0);// 获取三带双的牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByType07Three_Two(Three_TwoCardsValue, cards);

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
				tmp_moves = (vector<CARDSMOVE>)getMovesByType08SingleJunko(cardBeginValue, cardEndValue, cards);

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
				tmp_moves = (vector<CARDSMOVE>)getMovesByType09DualJunko(cardBeginValue, cardEndValue, cards);

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
				tmp_moves = (vector<CARDSMOVE>)getMovesByType10Three_Shun(cardBeginValue, cardEndValue, cards);

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
				tmp_moves = (vector<CARDSMOVE>)getMovesByType11Three_Shun_One(cardBeginValue, cardEndValue, cards);

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
				tmp_moves = (vector<CARDSMOVE>)getMovesByType12Three_Shun_Couple(cardBeginValue, cardEndValue, cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}

		case 13:// 四带二
			{
				int Four_TwoCardsValue = play->getFour_TwoValue(v).at(0);	    // 获取四带二单的牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByType13Four_Two(Four_TwoCardsValue, cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}
		case 14:// 四带二对
			{
				int Four_TwoCoupleCardsValue = play->getFour_TwoCoupleValue(v).at(0);  // 获取四带二对的牌面值
				tmp_moves = (vector<CARDSMOVE>)getMovesByType14Four_TwoCouple(Four_TwoCoupleCardsValue, cards);

				for(size_t i=0;i<tmp_moves.size();i++)
				{
					moves.push_back(tmp_moves.at(i));
				}
				break;
			}
		case -1:
			break;
		}

		if(CardsType!=ZHADAN && CardsType!=ROCKET)// 炸弹和火箭管一般牌型
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

		// 被动出牌加入PASS走步
		CARDSMOVE m = CARDSMOVE();
		m.cardsType = PASS;
		moves.push_back(m);
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
		tmp_moves=(vector<CARDSMOVE>)getMovesBy01Card(cards);
		break;

	case 2:// 获取牌张数为2的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy02Cards(cards);
		break;

	case 3:// 获取牌张数为3的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy03Cards(cards);
		break;

	case 4:// 获取牌张数为4的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy04Cards(cards);
		break;

	case 5:// 获取牌张数为5的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy05Cards(cards);
		break;

	case 6:// 获取牌张数为6的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy06Cards(cards);
		break;

	case 7:// 获取牌张数为7的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy07Cards(cards);
		break;

	case 8:// 获取牌张数为8的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy08Cards(cards);
		break;

	case 9:// 获取牌张数为9的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy09Cards(cards);
		break;

	case 10:// 获取牌张数为10的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy10Cards(cards);
		break;

	case 11:// 获取牌张数为11的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy11Cards(cards);
		break;

	case 12:// 获取牌张数为12的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy12Cards(cards);
		break;

	case 13:// 不存在13张的牌型
		
	case 14:// 获取牌张数为14的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy14Cards(cards);
		break;
	case 15:// 获取牌张数为15的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy15Cards(cards);
		break;

	case 16:// 获取牌张数为16的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy16Cards(cards);
		break;

	case 17:// 不存在17张的牌型
	
	case 18:// 获取牌张数为18的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy18Cards(cards);
		break;

	case 19:// 不存在19张的牌型
		break;
	case 20:// 获取牌张数为20的所有走步
		tmp_moves=(vector<CARDSMOVE>)getMovesBy20Cards(cards);
		break;
	}

	return tmp_moves;
}


/**获取我方出牌为一张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesBy01Card(unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy02Cards(unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy03Cards(unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy04Cards(unsigned *cards)
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
					break;		
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy05Cards(unsigned *cards)
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
					break;//任意带一对牌			
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy06Cards(unsigned *cards)
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

		if (HaveThreeJunko(i, 2, tmp_cards))// 有i->i+2的三顺
		{
			CARDSMOVE tmp_vector;
			for (size_t tmp = i; tmp<i + 2; tmp++)
			{
				for (size_t k = 0; k<3; k++)
				{
					tmp_vector.cards.push_back(tmp);
				}
			}

			tmp_vector.cardsType = 10;// 在走步后面追加牌型，三顺牌型为10
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy07Cards(unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy08Cards(unsigned *cards)
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
			for(size_t m=0;m<2;m++)
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
					if (carryCards.size() == 2)
					{
						break;
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy09Cards(unsigned *cards)
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
					tmp_vector.cards.push_back(tmp);
				}
			}

			tmp_vector.cardsType=10;// 在走步后面追加牌型，三顺牌型为10
			NineCards_moves.push_back(tmp_vector);
		}
	}

	return NineCards_moves;
}


/**获取我方出牌为十张的走步 */
vector<CARDSMOVE> CMoveGenerator::getMovesBy10Cards(unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy11Cards(unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy12Cards(unsigned *cards)
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
					if (carryCards.size() == 3)
					{
						break;
					}
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy14Cards(unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy15Cards(unsigned *cards)
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
					if (carryCards.size() == 3)
					{
						break;
					}
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy16Cards(unsigned *cards)
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
					if (carryCards.size() == 4)
					{
						break;
					}
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy18Cards(unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesBy20Cards(unsigned *cards)
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
					if (carryCards.size() == 5)
					{
						break;
					}
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
					if (carryCards.size() == 4)
					{
						break;
					}
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

/**		判断是否一副牌有从指定位置的指定N连的单顺			*/
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


/**		判断是否一副牌有从指定位置的指定N连的双顺			*/
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

/**		判断是否一副牌有从指定位置的指定N连的三顺			*/
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
vector<CARDSMOVE> CMoveGenerator::getMovesByType02ZhaDan(unsigned ZhaDanValue,unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesByType03SingleCard(unsigned SingleCard, unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesByType04CoupleCards(unsigned CoupleCards, unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesByType05ThreeTiaoCards(unsigned ThreeCards, unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesByType06Three_One(unsigned Three_OneCards, unsigned *cards)
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
					tmp_vector.cardsType = 6;// 在走步后面追加牌型，三带单牌型为6
					Three_OneCards_moves.push_back(tmp_vector);
					break;
				}
			}
			
		}
	}

	return Three_OneCards_moves;
}

/** 根据对方三带二获得我方所有三带二走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByType07Three_Two(unsigned Three_TwoCards, unsigned *cards)
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
					tmp_vector.cardsType = 7;// 在走步后面追加牌型，三带双牌型为7
					Three_TwoCards_moves.push_back(tmp_vector);
					break;
				}
			}			
		}
	}

	return Three_TwoCards_moves;
}

/** 根据对方单顺获得我方所有单顺走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByType08SingleJunko(unsigned cardBeginValue, unsigned cardEndValue, unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesByType09DualJunko(unsigned cardBeginValue, unsigned cardEndValue, unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesByType10Three_Shun(unsigned cardBeginValue, unsigned cardEndValue, unsigned *cards)
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
vector<CARDSMOVE> CMoveGenerator::getMovesByType11Three_Shun_One(unsigned cardBeginValue, unsigned cardEndValue, unsigned *cards)
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
					if (carryCards.size() == JunkoNum)
					{
						break;
					}
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

/** 根据对方三顺带双获得我方所有三顺带双走步*/
vector<CARDSMOVE> CMoveGenerator::getMovesByType12Three_Shun_Couple(unsigned cardBeginValue, unsigned cardEndValue, unsigned *cards)
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
					if (carryCards.size() == JunkoNum)
					{
						break;
					}
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
vector<CARDSMOVE> CMoveGenerator::getMovesByType13Four_Two(unsigned Four_TwoCards, unsigned *cards)
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
						if (carryCards.size() == 2)
						{
							break;
						}
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
vector<CARDSMOVE> CMoveGenerator::getMovesByType14Four_TwoCouple(unsigned Four_TwoCoupleCards, unsigned *cards)
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
					carryCards.push_back(i1);
					if (carryCards.size() == 4)
					{
						break;
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
				tmp_vector.cards.push_back(carryCards[k]);
			}

			tmp_vector.cardsType=14;// 在走步后面追加牌型，四带二双牌型为14
			Four_TwoCouple_moves.push_back(tmp_vector);
		}
	}
	return Four_TwoCouple_moves;
}

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
		if (cards1.size() != cards2.size())
		{
			return false;
		}
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
		case THREE_ONE:
		{
						  vector<unsigned> cardsInfo1 = play->getThree_OneValue(cards1);
						  vector<unsigned> cardsInfo2 = play->getThree_OneValue(cards2);
					return cardsInfo1.at(0)<cardsInfo2.at(0);
					break;
		}
		case THREE_TWO:
		{
						  if (cards1.size() != cards2.size())
						  {
							  return false;
						  }
						  vector<unsigned> cardsInfo1 = play->getThree_TwoValue(cards1);
						  vector<unsigned> cardsInfo2 = play->getThree_TwoValue(cards2);
						  return cardsInfo1.at(0)<cardsInfo2.at(0);
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
	else if(startValue1>=startValue2)
	{
			return false;
	}
	else
	{
		return true;
	}
}