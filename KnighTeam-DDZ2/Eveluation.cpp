#include "Eveluation.h"
#include "time.h"
#include "DDZMoveManager.h"

CEveluation::CEveluation() 
{

}

CEveluation::~CEveluation() 
{

}

/**
*   初始化玩家隶属度表
*/
void CEveluation::initMembership()
{
	for(size_t i=0;i<54;i++)  //初始化玩家p1隶属度表
	{
		Player::p1_Membership[i]=0.5;
	}

	for(size_t i=0;i<54;i++)  //初始化玩家p2隶属度表
	{
		Player::p2_Membership[i]=0.5;
	}
}


/**
* 根据我方手中牌将玩家p1、p2隶属度表更新
*/
void CEveluation::ClearedByDealCards()
{
	for(size_t i=0;i<Player::p3_cardsList.size();i++)
	{
		int value = Player::p3_cardsList[i];
		Player::p1_Membership[value]=0;
		Player::p2_Membership[value]=0;
	}
}
/**
* 根据3张底牌将玩家p1、p2隶属度表更新
* @leftCards 3张底牌
*/
void CEveluation::ClearedByLeftCards(vector<unsigned> leftCards)
{
	if(Player::p3_IsLandlord)    // 我方抓牌
	{
		for(size_t i=0;i<leftCards.size();i++)
		{
			Player::p1_Membership[leftCards.at(i)]=0;//抓牌隶属度清零
			Player::p2_Membership[leftCards.at(i)]=0;//抓牌隶属度清零
		}
	}
	else if(Player::p2_IsLandlord)// 玩家p2抓牌
	{
		for(size_t i=0;i<leftCards.size();i++)
		{
			Player::p1_Membership[leftCards.at(i)]=0;//抓牌隶属度清零
			Player::p2_Membership[leftCards.at(i)]=1;//抓牌隶属度为1.0
		}
	}
	else					// 玩家p1抓牌
	{
		for(size_t i=0;i<leftCards.size();i++)
		{
			Player::p1_Membership[leftCards.at(i)]=1;//抓牌隶属度为1.0
			Player::p2_Membership[leftCards.at(i)]=0;//抓牌隶属度清零
		}
	}
}

/**
*  玩家出牌，将隶属度表更新
* @playerCards 出牌
*/
void CEveluation::ClearedByPlayCards(vector<unsigned> playCards)
{

	for(size_t i=0;i<playCards.size();i++)
	{
		Player::p1_Membership[playCards.at(i)]=0;
		Player::p2_Membership[playCards.at(i)]=0;
	}
}

/**
*  更新剩余牌型表
*  @list 已出的牌
*/
void CEveluation::RefreshRemaining(vector<unsigned> list)
{
	for(size_t i=0;i<list.size();i++)
	{
		if(list[i]==53)
		{
			if(Player::remaining[14]!=0)
			{
				Player::remaining[14]--;// 大王特殊处理
			}
		}
		else
		{
			if(Player::remaining[list.at(i)/4]!=0)
			{
				Player::remaining[list.at(i)/4]--;
			}
		}
	}
}
/**
*  根据走步猜测其他牌隶属度，还需更改
*  @whoseGo 轮到谁走步  @cardsType 出牌类型  @playCards 出牌
*/
void CEveluation::UpdateByPlayCards(int whoseGo,int cardsType,vector<unsigned> playCards)
{
	DDZPlayer *play = new DDZPlayer();
	
	switch(cardsType)
	{
	case ROCKET:
		break;
	case ZHADAN:
		break;
	case SINGLE:
		{
			unsigned singleValue = playCards.at(0);
			for (size_t i = 4 * singleValue; i<4 * singleValue + 4; i++)
			{
				if(whoseGo==1)
				{
					if(Player::p1_Membership[i]!=0)
					{
						Player::p1_Membership[i] -= 0.1;
					}

					if(Player::p2_Membership[i]!=0)
					{
						Player::p2_Membership[i] += 0.1;
					}

				}
				else if(whoseGo==2)
				{
					if(Player::p1_Membership[i]!=0)
					{
						Player::p1_Membership[i] += 0.1;
					}

					if(Player::p2_Membership[i]!=0)
					{
						Player::p2_Membership[i] -= 0.1;
					}
				}
			}
			break;
		}
	case COUPLE:
		{
			unsigned coupleValue = playCards.at(0);
			for (size_t i = 4 * coupleValue; i<4 * coupleValue + 4; i++)
			{
				if(whoseGo==1)
				{
					if(Player::p1_Membership[i]!=0)
					{
						Player::p1_Membership[i] -= 0.1;
					}

					if(Player::p2_Membership[i]!=0)
					{
						Player::p2_Membership[i] += 0.1;
					}

				}
				else if(whoseGo==2)
				{
					if(Player::p1_Membership[i]!=0)
					{
						Player::p1_Membership[i] += 0.1;
					}

					if(Player::p2_Membership[i]!=0)
					{
						Player::p2_Membership[i] -= 0.1;
					}
				}
			}
			break;
		}
	case SANTIAO:
		{	
			unsigned SanTiaoValue = playCards.at(0);
			for (size_t i = 4 * SanTiaoValue; i<4 * SanTiaoValue + 4; i++)
			{
				if(whoseGo==1)
				{
					if(Player::p1_Membership[i]!=0)
					{
						Player::p1_Membership[i]=0;
					}

					if(Player::p2_Membership[i]!=0)
					{
						Player::p2_Membership[i]=1;
					}

				}
				else if(whoseGo==2)
				{
					if(Player::p1_Membership[i]!=0)
					{
						Player::p1_Membership[i]=1;
					}

					if(Player::p2_Membership[i]!=0)
					{
						Player::p2_Membership[i]=0;
					}
				}
			}
		break;
		}
	case THREE_ONE:
		{

			vector<unsigned> Three_OneInfo = play->getThree_OneValue(playCards);

			unsigned SanTiaoValue = Three_OneInfo.at(0);//三带一 三条牌面值
			unsigned SanTiao_One = Three_OneInfo.at(2);//三带一 带牌牌面值

			for (size_t i = 4 * SanTiaoValue; i<4 * SanTiaoValue + 4; i++)
			{
				if(whoseGo==1)// 玩家一方打出三带一，若我方手中无三条剩余的一张，则另一张几乎在另一玩家手中
				{
					if(Player::p1_Membership[i]!=0)
					{
						Player::p1_Membership[i]=0;
					}

					if(Player::p2_Membership[i]!=0)
					{
						Player::p2_Membership[i]=1;
					}

				}
				else if(whoseGo==2)
				{
					if(Player::p1_Membership[i]!=0)
					{
						Player::p1_Membership[i]=1;
					}

					if(Player::p2_Membership[i]!=0)
					{
						Player::p2_Membership[i]=0;
					}
				}
			}

			for (size_t i = 4 * SanTiao_One; i<4 * SanTiao_One + 4; i++)
			{
				if(whoseGo==1)// 更改带牌隶属度
				{
					if(Player::p1_Membership[i]!=0)
					{
						Player::p1_Membership[i] -= 0.1;
					}

					if(Player::p2_Membership[i]!=0)
					{
						Player::p2_Membership[i] += 0.1;
					}

				}
				else if(whoseGo==2)
				{
					if(Player::p1_Membership[i]!=0)
					{
						Player::p1_Membership[i] += 0.1;
					}

					if(Player::p2_Membership[i]!=0)
					{
						Player::p2_Membership[i] -= 0.1;
					}
				}
			}
		break;
		}
	case THREE_TWO:
		{
			vector<unsigned> Three_TwoInfo = play->getThree_TwoValue(playCards);

			unsigned SanTiaoValue = Three_TwoInfo.at(0);//三带一 三条牌面值
			unsigned SanTiao_Two = Three_TwoInfo.at(2);//三带一 对牌牌面值

			for (size_t i = 4 * SanTiaoValue; i<4 * SanTiaoValue + 4; i++)
			{
				if(whoseGo==1)// 玩家一方打出三带一，若我方手中无三条剩余的一张，则另一张几乎在另一玩家手中
				{
					if(Player::p1_Membership[i]!=0)
					{
						Player::p1_Membership[i]=0;
					}

					if(Player::p2_Membership[i]!=0)
					{
						Player::p2_Membership[i]=1;
					}

				}
				else if(whoseGo==2)
				{
					if(Player::p1_Membership[i]!=0)
					{
						Player::p1_Membership[i]=1;
					}

					if(Player::p2_Membership[i]!=0)
					{
						Player::p2_Membership[i]=0;
					}
				}
			}

			for (size_t i = 4 * SanTiao_Two; i<4 * SanTiao_Two + 4; i++)
			{
				if(whoseGo==1)// 更改带牌隶属度
				{
					if(Player::p1_Membership[i]!=0)
					{
						Player::p1_Membership[i] -= 0.1;
					}

					if(Player::p2_Membership[i]!=0)
					{
						Player::p2_Membership[i] += 0.1;
					}

				}
				else if(whoseGo==2)
				{
					if(Player::p1_Membership[i]!=0)
					{
						Player::p1_Membership[i] += 0.1;
					}

					if(Player::p2_Membership[i]!=0)
					{
						Player::p2_Membership[i] -= 0.1;
					}
				}
			}
		break;
		}
	case SINGLEJUNKO:
		{
			vector<unsigned> singleJunkoInfo = play->getSingleJunkoValue(playCards);

			unsigned startJunkoValue = singleJunkoInfo.at(0);
			unsigned endJunkoValue = singleJunkoInfo.at(1);

			for(size_t k=startJunkoValue;k<=endJunkoValue;k++)
			{
				for (size_t i = 4 * k; i<4 * k + 4; i++)
				{
					if(whoseGo==1)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i] -= 0.1;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i] += 0.1;
						}

					}
					else if(whoseGo==2)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i] += 0.1;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i] -= 0.1;
						}
					}
				}
			}
		
		break;
		}
	case DUALJUNKO:
		{
			vector<unsigned> DualJunkoInfo = play->getDualJunkoValue(playCards);

			unsigned startJunkoValue = DualJunkoInfo.at(0);
			unsigned endJunkoValue = DualJunkoInfo.at(1);

			for(size_t k=startJunkoValue;k<=endJunkoValue;k++)
			{
				for (size_t i = 4 * k; i<4 * k + 4; i++)
				{
					if(whoseGo==1)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i] -= 0.1;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i] += 0.1;
						}

					}
					else if(whoseGo==2)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i] += 0.1;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i] -= 0.1;
						}
					}
				}
			}
		
		break;
		}
	case THREEJUNKO:
		{
			vector<unsigned> ThreeJunkoInfo = play->getThree_ShunValue(playCards);

			unsigned startJunkoValue = ThreeJunkoInfo.at(0);
			unsigned endJunkoValue = ThreeJunkoInfo.at(1);

			for(size_t k=startJunkoValue;k<=endJunkoValue;k++)
			{
				for (size_t i = 4 * k; i<4 * k + 4; i++)
				{
					if(whoseGo==1)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i]=0;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i]=1;
						}

					}
					else if(whoseGo==2)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i]=1;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i]=0;
						}
					}
				}
			}
		
		break;
		}
	case THREEJUNKO_ONE:
		{
			vector<unsigned> ThreeJunko_OneInfo = play->getThree_OneValue(playCards);

			unsigned startJunkoValue = ThreeJunko_OneInfo.at(0);
			unsigned endJunkoValue = ThreeJunko_OneInfo.at(1);

			for(size_t k=startJunkoValue;k<=endJunkoValue;k++)
			{
				for (size_t i = 4 * k; i<4 * k + 4; i++)
				{
					if(whoseGo==1)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i]=0;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i]=1;
						}

					}
					else if(whoseGo==2)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i]=1;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i]=0;
						}
					}
				}
			}

			for(size_t k=2;k<ThreeJunko_OneInfo.size();k++)
			{
				for (size_t i = 4 * (ThreeJunko_OneInfo.at(k)); 
					i<4 * (ThreeJunko_OneInfo.at(k)) + 4; i++)
				{
					if(whoseGo==1)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i] -= 0.1;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i] += 0.1;
						}

					}
					else if(whoseGo==2)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i] += 0.1;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i] -= 0.1;
						}
					}
				}
			}
		
		break;
		}
	case THREEJUNKO_TWO:
		{
			vector<unsigned> ThreeJunko_TwoInfo = play->getThree_TwoValue(playCards);

			unsigned startJunkoValue = ThreeJunko_TwoInfo.at(0);
			unsigned endJunkoValue = ThreeJunko_TwoInfo.at(1);

			for(size_t k=startJunkoValue;k<=endJunkoValue;k++)
			{
				for (size_t i = 4 * k; i<4 * k + 4; i++)
				{
					if(whoseGo==1)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i]=0;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i]=1;
						}

					}
					else if(whoseGo==2)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i]=1;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i]=0;
						}
					}
				}
			}

			for(size_t k=2;k<ThreeJunko_TwoInfo.size();k++)// 带牌信息更新
			{
				for (size_t i = 4 * (ThreeJunko_TwoInfo.at(k)); 
					i<4 * (ThreeJunko_TwoInfo.at(k)) + 4; i++)
				{
					if(whoseGo==1)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i]=0;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i]=1;
						}

					}
					else if(whoseGo==2)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i]=1;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i]=0;
						}
					}
				}
			}
		
		break;
		}
	case FOUR_TWO:
		{
			vector<unsigned> four_TwoInfo = play->getFour_TwoValue(playCards);

			for(size_t k=1;k<four_TwoInfo.size();k++)// 带牌信息更新
			{
				for (size_t i = 4 * (four_TwoInfo.at(k)); i<4 * (four_TwoInfo.at(k)) + 4; i++)
				{
					if(whoseGo==1)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i] -= 0.1;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i] += 0.1;
						}

					}
					else if(whoseGo==2)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i] += 0.1;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i] -= 0.1;
						}
					}
				}
			}


			break;
		}
	case FOUR_TWO_COUPLE:
		{
		vector<unsigned> four_TwoCoupleInfo = play->getFour_TwoCoupleValue(playCards);

			for(size_t k=1;k<four_TwoCoupleInfo.size();k++)// 带牌信息更新
			{
				for (size_t i = 4 * (four_TwoCoupleInfo.at(k)); 
					i<4 * (four_TwoCoupleInfo.at(k)) + 4; i++)
				{
					if(whoseGo==1)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i] -= 0.1;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i] += 0.1;
						}

					}
					else if(whoseGo==2)
					{
						if(Player::p1_Membership[i]!=0)
						{
							Player::p1_Membership[i] += 0.1;
						}

						if(Player::p2_Membership[i]!=0)
						{
							Player::p2_Membership[i] -= 0.1;
						}
					}
				}
			}
		break;
		}
	}


	delete(play);
	play = NULL;
}

void CEveluation::UpdateFewCards(int whoseGo)
{
	int cardsType=Player::lastMove.cardsType;
	if(whoseGo!=1||whoseGo!=2||whoseGo!=3)
	{
		return ;
	}
	else if(whoseGo==1)
	{
		Player::p1_fewCards[cardsType]=1;
	}
	else if(whoseGo==2)
	{
		Player::p2_fewCards[cardsType]=1;
	}
}

void CEveluation::UpdateManyCards(int whoseGo)
{
	int cardsType=Player::firstMove.cardsType;
	if(whoseGo!=1||whoseGo!=2||whoseGo!=3)
	{
		return ;
	}
	else if(whoseGo==1&&Player::firstPlayer==1)
	{
		Player::p1_manyCards[cardsType]=1;
	}
	else if(whoseGo==2&&Player::firstPlayer==2)
	{
		Player::p2_manyCards[cardsType]=1;
	}
}

/**
*   蒙特卡罗随机
*   斗地主悲观模拟菜单
*   根据剩余棋子表和斗地主隶属度表产生一个玩家的模拟持有牌，则剩余棋子即为另一玩家
*   备注：这里玩家模拟方选择也可随机。。
*/
void CEveluation::NegativeSimulatedMenu()
{
	srand(time(NULL));

	//初始化玩家1、2牌面，全清零进行模拟
	init_p1_p2_EachCardNum();


	int whoseRand=rand()%10;

	double tmp_Membership[54];
	int cardsNum=0;   // 随机菜单需要产生的随机牌张数???

	if(whoseRand<5) // 随机数为0-4，随机生成玩家1模拟牌面
	{
		memcpy(tmp_Membership,Player::p1_Membership,sizeof(tmp_Membership));
		cardsNum = Player::p1_cardsNum;

		while(cardsNum)// 直到随机完玩家手中牌，循环结束
		{
			for(size_t i=0;i<54;i++) // 牌面随机模拟正式开始
			{
				double randNum = (double)(rand()%100)/100.00;// 产生一个0.00->1.00的随机数
				int value = getValue(i);
				
				if (randNum < tmp_Membership[value] && cardsNum != 0
					&& Player::p1_EachCardNum[value] < Player::remaining[value])
				{
					Player::p1_EachCardNum[value]++;
					cardsNum--;
				}
				
			}
		}

		for(size_t i=0;i<15;i++)
		{
			Player::p2_EachCardNum[i] = Player::remaining[i] - Player::p1_EachCardNum[i];//根据玩家1的随机牌及牌型剩余表得到玩家2的牌
		}
	}
	else			// 随机数为5-9，随机生成玩家2模拟牌面
	{
		memcpy(tmp_Membership, Player::p2_Membership, sizeof(tmp_Membership));
		cardsNum = Player::p2_cardsNum;

		while(cardsNum)
		{
			for(size_t i=0;i<54;i++) // 牌面随机模拟正式开始
			{
				double randNum = (double)(rand()%100)/100.00;// 产生一个0.00->1.00的随机数
		
				int value = getValue(i);
				
				if (randNum < tmp_Membership[i] && cardsNum != 0
					&& Player::p2_EachCardNum[value] < Player::remaining[value])
				{
					Player::p2_EachCardNum[value]++;
					cardsNum--;
				}
			}
		}

		for(size_t i=0;i<15;i++)
		{
			Player::p1_EachCardNum[i] = Player::remaining[i] - Player::p2_EachCardNum[i];//根据玩家2的随机牌及牌型剩余表得到玩家1的牌
		}
	}	
}

/**
*	斗地主乐观模拟菜单
*	当玩家某张牌持有的概率大于0.5，
*   则认为一定存在于该玩家手中。
*	若小于或等于0.5，则需要模拟
*/
void CEveluation::PositiveSimulatedMenu()
{
	srand(time(NULL));
	
	//初始化玩家1、2牌面，全清零进行模拟
	init_p1_p2_EachCardNum(); 

	int p1_cardsNum = Player::p1_cardsNum;
	int p2_cardsNum = Player::p2_cardsNum;
	
	unsigned remaining[15];
	memcpy(remaining, Player::remaining, sizeof(remaining));
	double gailv = 0.5;
	for (int i = 0; i < 54; i++)
	{
		if (Player::p1_Membership[i] > gailv)// 依次取概率大于0.5的作为玩家1手中牌
		{
			handleMoreThanHalf(i, &p1_cardsNum, remaining, 1);
		}
	}
	for (int i = 0; i < 54; i++)
	{
		if (Player::p2_Membership[i] > gailv)// 依次取概率大于0.5的作为玩家2手中牌
		{
			handleMoreThanHalf(i, &p2_cardsNum, remaining, 2);
		}
	}
	while (p1_cardsNum && gailv > 0)
	{
		gailv -= 0.1;
		for (int i = 0; i < 54; i++)
		{
			if (Player::p1_Membership[i] > gailv)// 依次取概率的大的作为玩家1手中牌
			{
				handleMoreThanHalf(i, &p1_cardsNum, remaining, 1);
			}
		}
		
	}
	gailv = 0.5;
	while (p2_cardsNum && gailv > 0)
	{
		gailv -= 0.1;
		for (int i = 0; i < 54; i++)
		{
			if (Player::p2_Membership[i] > gailv)// 依次取概率的大的作为玩家1手中牌
			{
				handleMoreThanHalf(i, &p2_cardsNum, remaining, 2);
			}
		}
		
	}
	
	if (p1_cardsNum)
	{
		for (size_t j = 0; j<15; j++)
		{
			if (remaining[j] > 0)
			{
				if (p1_cardsNum > remaining[j])
				{
					Player::p1_EachCardNum[j] += remaining[j];
					p1_cardsNum-= remaining[j];
					remaining[j]=0;
				}	
				else
				{
					Player::p1_EachCardNum[j] += p1_cardsNum;
					remaining[j]-=p1_cardsNum;
					p1_cardsNum = 0;
				}
			}
		}
	}

	if (p2_cardsNum)
	{
		for (size_t j = 0; j<15; j++)
		{
			Player::p2_EachCardNum[j] += remaining[j];//玩家2差的牌等于剩余牌
		}
	}
	
	
	
	//for (int i = 0; i < 54; i++)
	//{
	//	if (Player::p1_Membership[i] > 0.5)//当概率大于0.5，则确定持有该牌
	//	{
	//		handleMoreThanHalf(i, &p1_cardsNum, remaining, 1);
	//	}
	//}

	//for (int i = 0; i<54; i++)
	//{
	//	if (Player::p2_Membership[i]>0.5)
	//	{
	//		handleMoreThanHalf(i, &p2_cardsNum, remaining, 2);
	//	}
	//}

	//for (int i = 0; i<15; i++)
	//{
	//	Player::p1_EachCardAndRemaining[i] = remaining[i] + Player::p1_EachCardNum[i];
	//	Player::p2_EachCardAndRemaining[i] = remaining[i] + Player::p2_EachCardNum[i];
	//}
	//int index = 0;
	//while (p1_cardsNum)
	//{
	//	if (index > 53)
	//	{
	//		index = 0;
	//	}
	//	if (Player::p1_Membership[index] > 0.5 || Player::p1_Membership[index]<=0)
	//	{
	//		index++;//大于0.5的已经被模拟，直接跳过,等于零不模拟
	//	}
	//	else
	//	{
	//		double randNum = (double)(rand() % 100) / 100.00;
	//		handleNotMoreThanHalf(index, randNum, &p1_cardsNum, remaining, 1);
	//		index++;
	//	}
	//}

	//for (size_t j = 0; j<15; j++)
	//{
	//	Player::p2_EachCardNum[j] += remaining[j];//p2手中牌=模拟后剩余牌+本身已经确定的牌
	//}

}

/**
*	处理p1概率大于0.5的牌
*	@i 牌的下标
*   @cardsNum 玩家手中牌数量
*   @who  哪个玩家
*/
void CEveluation::handleMoreThanHalf(int i, int *cardsNum, unsigned *remaining, int who)
{
	int value = getValue(i);

	if (who == 1 && (*cardsNum) > 0 &&
		Player::p1_EachCardNum[value]<remaining[value])
	{
		Player::p1_EachCardNum[value]++;
		remaining[value]--;
		(*cardsNum)--;
	}
	else if (who == 2 && (*cardsNum)> 0 &&
			 Player::p2_EachCardNum[value] < remaining[value])
	{
		Player::p2_EachCardNum[value]++;
		remaining[value]--;
		(*cardsNum)--;
	}
}

/**
*	处理p1概率不超过0.5的牌
*   随机数不好控制，舍弃此方法模拟
*/
void CEveluation::handleNotMoreThanHalf(int i, double randNum, int *cardsNum, unsigned *remaining, int who)
{
	int value = getValue(i);
	if (who == 1 && Player::p1_EachCardNum[value] < remaining[value]
		&& randNum < Player::p1_Membership[i] && (*cardsNum)>0)
	{
		Player::p1_EachCardNum[value]++;
		remaining[value]--;
		(*cardsNum)--;
	}
	else if (who == 2 && Player::p2_EachCardNum[value] < remaining[value]
			 && randNum < Player::p2_Membership[i] && (*cardsNum) > 0)
	{
		Player::p2_EachCardNum[value]++;
		remaining[value]--;
		(*cardsNum)--;
	}
}

void CEveluation::handle1(int value, int *cardsNum, int who)
{
	if (who == 1 && (*cardsNum) > 0 &&
			Player::p1_EachCardNum[value]<Player::remaining[value])
	{
		Player::p1_EachCardNum[value]++;
		(*cardsNum)--;
	}
	else if (who == 2 && (*cardsNum )> 0 && 
				Player::p2_EachCardNum[value] < Player::remaining[value])
	{
		Player::p2_EachCardNum[value]++;
		(*cardsNum)--;
	}
}

void CEveluation::handle2(int value, int i, double randNum, int *cardsNum, unsigned *remaining, int who)
{
	if (Player::p1_EachCardNum[value] < Player::remaining[value]
		&& randNum < Player::p1_Membership[i])
	{
		Player::p1_EachCardNum[value]++;
		(*cardsNum)--;
	}
}



int CEveluation::getValue(int i)
{
	return i == 53 ? 14 : (i / 4);
}




void CEveluation::init_p1_p2_EachCardNum()
{
	for (int i = 0; i < 15; i++)
	{
		Player::p1_EachCardNum[i] = 0;
		Player::p2_EachCardNum[i] = 0;
	}
}

vector<int> CEveluation::ComputeNumOfBasic(int *cards)
{
	vector<int> tmp_vector;

	int single=0;
	int couple=0;
	int threeTiao=0;

	int tmp_cards[15];
	memcpy(tmp_cards,cards,sizeof(tmp_cards));

	for(size_t i=0;i<13;i++)//3->A、2
	{
		if(tmp_cards[i]==1)
		{
			single++;
		}
		else if(tmp_cards[i]==2)
		{
			couple++;
		}
		else if(tmp_cards[i]==3)
		{
			threeTiao++;
		}
	}

	if(tmp_cards[13]==1&&tmp_cards[14]==0)
	{
		single++;
	}
	if(tmp_cards[13]==0&&tmp_cards[14]==1)
	{
		single++;
	}

	tmp_vector.push_back(single);
	tmp_vector.push_back(couple);
	tmp_vector.push_back(threeTiao);

	return tmp_vector;
}
//
//int getMaxIndexOfBasic(vector<int> vector)
//{
//	int maxIndex=0;
//
//	for(size_t i=1;i<vector.size();i++)
//	{
//		if(vector.at(i)>vector.at(i-1))
//		{
//			maxIndex=i;
//		}
//	}
//
//	return maxIndex+1;
//}

/*    AI Pass策略   */
int CEveluation::IsAiPass()
{
	if(Player::p2_IsLandlord)		 //  上家为地主
	{
		if(Player::lastPlayer==1)	 //  出牌者为下家
		{
			return 1;
		}
	}

	if(Player::p1_IsLandlord)
	{
		if(Player::p1_general.size()>0 && Player::p1_general[Player::p1_general.size()-1].cardsType==-1 
			&& Player::lastPlayer == 2 && Player::firstPlayer ==3)
		{
			return 1;
		}
	}

	return 0;
}

int CEveluation::GetCardsRightIfPass()
{
	int size = Player::cardsMoveRecords.size();

	if (Player::p1_IsLandlord)
	{
		if (Player::lastPlayer == 1)
			return -1;
		else
			return 0;
	
	}
	else if (Player::p2_IsLandlord)
	{
		if (Player::lastPlayer == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

int CEveluation::IsCurrentTeam(bool current_IsLandlord, int turn)
{
	if (current_IsLandlord)
	{
		return false;
	}
	else
	{
		if (turn == 1)
		{
			if (Player::p1_IsLandlord)
				return false;
			else
				return true;
		}
		else if (turn == 2)
		{
			if (Player::p2_IsLandlord)
				return false;
			else
				return true;
		}
		else
		{
			if (Player::p3_IsLandlord)
				return false;
			else
				return true;
		}
	}
}

/*
*	是否为此轮出牌发起者
*/
bool CEveluation::IsCardOriginator(int turn)
{
	return Player::firstPlayer == turn ? true : false;
}

int CEveluation::EveluateMove(CARDSMOVE* move, int whoseGo)
{
	DDZPlayer *play = new DDZPlayer();
	int score=0;//估值
	int outWay = move->outWay;
	int cardsType = move->cardsType;
	vector<unsigned> current_cards = move->cards;
	int currentValue; // 当前走步牌面值
	int currentCardsNum; //当前玩家手中牌数量
	
	
	
	unsigned tmp_cards[15];
	unsigned next_cards[15];

	bool current_IsLandlord; // 当前是否为地主
	bool next_IsLandlord;     // 下家是否为地主
	bool front_IsLandLord;
	int turn = whoseGo % 3;


	if (cardsType != INVALID && cardsType != PASS)
	{
		currentValue = current_cards[0];
	}
	if(turn == 0)
	{
		memcpy(tmp_cards,Player::p3_EachCardNum,sizeof(tmp_cards));
		memcpy(next_cards,Player::p1_EachCardNum,sizeof(next_cards));

		currentCardsNum = Player::p3_cardsNum;
		current_IsLandlord=Player::p3_IsLandlord;
		next_IsLandlord=Player::p1_IsLandlord;
		front_IsLandLord=Player::p2_IsLandlord;
	}
	else if(turn == 2)
	{
		memcpy(tmp_cards,Player::p1_EachCardNum,sizeof(tmp_cards));
		memcpy(next_cards,Player::p2_EachCardNum,sizeof(next_cards));

		currentCardsNum = Player::p2_cardsNum;
		current_IsLandlord=Player::p1_IsLandlord;
		next_IsLandlord=Player::p2_IsLandlord;
		front_IsLandLord=Player::p3_IsLandlord;
	}
	else
	{
		memcpy(tmp_cards,Player::p2_EachCardNum,sizeof(tmp_cards));
		memcpy(next_cards,Player::p3_EachCardNum,sizeof(next_cards));

		currentCardsNum = Player::p1_cardsNum;
		current_IsLandlord=Player::p2_IsLandlord;
		next_IsLandlord=Player::p3_IsLandlord;
		front_IsLandLord=Player::p1_IsLandlord;
	}

	
	
	if ((IsCardOriginator(turn) || (!current_IsLandlord && !next_IsLandlord)) 
			&& cardsType != PASS)
	{
		score += (D - current_cards[0]) * 10;//自己主动出牌发起者，尽量走最小
	}
	else if (!IsCardOriginator(turn) && next_IsLandlord && cardsType != PASS)
	{
		switch (currentValue)//顶牌
		{
			case K: score += 50; break;
			case A: score += 40; break;
			case T: score += 30; break;
			case Q: score += 20; break;
			case X: score += 10; break;
			case D: score += 5;  break;
		}
	}

	if (cardsType == SINGLEJUNKO || cardsType == DUALJUNKO || cardsType == THREEJUNKO
		|| cardsType == THREEJUNKO_ONE || cardsType == THREEJUNKO_TWO || cardsType == ZHADAN
		|| cardsType == FOUR_TWO || cardsType == FOUR_TWO_COUPLE)
	{
		if (IsCurrentTeam(current_IsLandlord, Player::lastPlayer))
			score-=1000;
	}
	else if (cardsType == SINGLE || cardsType == COUPLE || cardsType == SANTIAO
			 || cardsType == THREE_ONE || cardsType == THREE_TWO)
	{
		if (IsCurrentTeam(current_IsLandlord, Player::lastPlayer))
		{
			if (Player::lastMove.cardsType != INVALID)
			{
				int value = Player::lastMove.cards[0];
				if (value >= A)
				{
					score-=1000;//伙伴方出大牌我方不管
				}
			}
		}
	}


	if (currentCardsNum == current_cards.size())//非四带牌的能一手走完就走完
	{
		if (cardsType != FOUR_TWO || cardsType != FOUR_TWO_COUPLE)
		{
			score+=1000;
		}
	}


	switch (cardsType)
	{
		case PASS:
		{
				 int canGetRight = GetCardsRightIfPass();
				 if (canGetRight == 1)//pass获得牌权
				 {
					score+=10000;
				 }
				 else if (canGetRight == -1)
				 {
					score-=100;
				 }

				 if (Player::p1_IsLandlord)
				 {
					 if (Player::p1_cardsNum == 1 || Player::p1_cardsNum == 2)
					 {
						 score -= 200;
					 }
					 else if (Player::p2_cardsNum == 1 || Player::p1_cardsNum == 2)
					 {
						 score += 200;
					 }
				 }
				 else  if (Player::p2_IsLandlord)
				 {
					 if (Player::p2_cardsNum == 1 || Player::p2_cardsNum == 2)
					 {
						 score -= 200;
					 }
					 else if (Player::p1_cardsNum == 1 || Player::p1_cardsNum == 2)
					 {
						 score += 200;
					 }
				 }
				 break;
			}
		case ROCKET:
		{
				   if (Player::p3_cardsNum == 2)
				   {
						score+=9999;
				   }
					score-=1000;
			break;
		}
		

		case ZHADAN:
		{
				   if (Player::p3_cardsNum == 2)
				   {
					   score += 9999;
				   }
					score-=1000;
					int hisZhaDanValue = play->getZhaDanValue(current_cards);// 获取炸弹的牌面值
					int myZhaDanValue = play->getZhaDanValue(current_cards);// 获取炸弹的牌面值
					if (move->status == STATUS_MAX && Player::p3_canPlayOver)
					{
						score += 9999;
					}
					break;
		}

		case SINGLE:
			{
					   if (Player::p1_IsLandlord)
					   {
						   if (Player::p1_cardsNum == 1)
						   {
								score-=200;
						   }
						   else if (Player::p2_cardsNum == 1)
						   {
								score+=200;
						   }
					   }
					   else  if (Player::p2_IsLandlord)
					   {
						   if (Player::p2_cardsNum == 1)
						   {
							   score -= 200;
						   }
						   else if (Player::p1_cardsNum == 1)
						   {
							   score += 200;
						   }
					   }
				int SingleValue = play->getSingleValue(current_cards);
				score+=20;
			break;
		}

	case COUPLE:
		{
				   if (Player::p1_IsLandlord)
				   {
					   if (Player::p1_cardsNum == 2)
					   {
						   score -= 200;
					   }
					   else if (Player::p2_cardsNum == 2)
					   {
						   score += 200;
					   }
				   }
				   else  if (Player::p2_IsLandlord)
				   {
					   if (Player::p2_cardsNum == 2)
					   {
						   score -= 200;
					   }
					   else if (Player::p1_cardsNum == 2)
					   {
						   score += 200;
					   }
				   }
			int coupleValue =play->getCoupleValue(current_cards);
			score += 10;
			break;
		}

	case SANTIAO:
		{
			int threeTiaoValue=play->getSanTiaoValue(current_cards);
			score+=5;

			break;
		}

	case THREE_ONE:
		{
			vector<unsigned> cardsInfo = play->getThree_OneValue(current_cards);
			int threeTiaoValue=cardsInfo.at(0);

			if(tmp_cards[threeTiaoValue]==3)
			{
				score+=30;
			}
			else if(tmp_cards[threeTiaoValue]==4)
			{
				score-=60;
			}
		}
		break;

	case THREE_TWO:
		{
			vector<unsigned> cardsInfo = play->getThree_TwoValue(current_cards);
		
			int threeTiaoValue=cardsInfo.at(0);

			if(tmp_cards[threeTiaoValue]==3)
			{
				score+=30;
			}
			else if(tmp_cards[threeTiaoValue]==4)
			{
				score-=60;
			}
		}
		break;

	case SINGLEJUNKO:
		{
			vector<unsigned> cardsInfo = play->getSingleJunkoValue(current_cards);
			int startValue = cardsInfo.at(0);
			int endValue = cardsInfo.at(1);

			for(unsigned i =startValue;i<=endValue;i++)
			{
				if(tmp_cards[i]==1)
				{
					score+=20;
				}
				else if(tmp_cards[i]==2)
				{
					score-=10;
				}
				else if(tmp_cards[i]==3)
				{
					score-=20;
				}
				else if(tmp_cards[i]==4)
				{
					score-=60;
				}
			}
		}
		break;

	case DUALJUNKO:
		{
			vector<unsigned> cardsInfo = play->getDualJunkoValue(current_cards);
			int startValue = cardsInfo.at(0);
			int endValue = cardsInfo.at(1);
			//int junkoNum = endValue-startValue;

			for(unsigned i =startValue;i<=endValue;i++)
			{
				if(tmp_cards[i]==2)
				{
					score+=15;
				}
				else if(tmp_cards[i]==3)
				{
					score-=20;
				}
				else if(tmp_cards[i]==4)
				{
					score-=60;
				}
			}
		}
		break;

	case THREEJUNKO:
		{
			vector<unsigned> cardsInfo = play->getThree_ShunValue(current_cards);
			int startValue = cardsInfo.at(0);
			int endValue = cardsInfo.at(1);
			//int junkoNum = endValue-startValue;

			for(unsigned i =startValue;i<=endValue;i++)
			{
				if(tmp_cards[i]==3)
				{
					score+=30;
				}
				else if(tmp_cards[i]==4)
				{
					score-=60;
				}
			}
		}
		break;

	case THREEJUNKO_ONE:
		{
			vector<unsigned> cardsInfo = play->getThree_OneValue(current_cards);
			int startValue = cardsInfo.at(0);
			int endValue = cardsInfo.at(1);
			int junkoNum = endValue-startValue+1;
			
			for(unsigned i =startValue;i<=endValue;i++)
			{
				if(tmp_cards[i]==3)
				{
					score+=40;
				}
				else if(tmp_cards[i]==4)
				{
					score-=200;
				}
			}
		}
		break;

	case THREEJUNKO_TWO:
		{
			vector<unsigned> cardsInfo = play->getThree_TwoValue(current_cards);
			int startValue = cardsInfo.at(0);
			int endValue = cardsInfo.at(1);
			int junkoNum = endValue-startValue+1;
			 
			for(unsigned i =startValue;i<=endValue;i++)
			{
				if(tmp_cards[i]==3)
				{
					score+=40;
				}
				else if(tmp_cards[i]==4)
				{
					score-=200;
				}
			}
		}
		break;

	case FOUR_TWO:
		{
			vector<unsigned> cardsInfo = play->getFour_TwoValue(current_cards);
			int FourValue = cardsInfo.at(0);

			score-=100;
		}
		break;

	case FOUR_TWO_COUPLE:
		{
			vector<unsigned> cardsInfo = play->getFour_TwoCoupleValue(current_cards);
			int FourValue = cardsInfo.at(0);
			
			score-=100;
		}
		break;
	}

	move->score=score;
	return score;
}


int CEveluation::Evelute(CARDSMOVE move, int whoseGo)
{	
	int cardsValue = move.cards[0]; //牌面值
	int outWay = move.outWay;//出牌方式
	bool current_IsLandlord, next_Islandlord;
	int key = 1;
	int score = 0;
	int turn = whoseGo % 3;
	if (turn == 0)
	{
		current_IsLandlord = Player::p3_IsLandlord;
		next_Islandlord = Player::p1_IsLandlord;
	}
	else if (turn == 1)
	{
		current_IsLandlord = Player::p1_IsLandlord;
		next_Islandlord = Player::p2_IsLandlord;
	}
	else
	{
		current_IsLandlord = Player::p2_IsLandlord;
		next_Islandlord = Player::p3_IsLandlord;
	}

	if (!current_IsLandlord)
	{
		key = -1;
	}
	score += (key * (D - cardsValue) * 5);//优先出小牌


	return score;
}
void CEveluation::EveluateMoves(vector<CARDSMOVE> *moves, int whoseGo)
{
	for (int i = moves->size() - 1; i >= 0; i--)
	{
		EveluateMove(&moves->at(i), whoseGo);
	}
}

void CEveluation::cutCarryCards(CARDSMOVE *move)
{
	int cardsType = move->cardsType;
	DDZPlayer play = DDZPlayer();
	vector<unsigned> cardsInfo;
	vector<unsigned> canCarryCards;
	if(cardsType==THREE_ONE || cardsType == THREEJUNKO_ONE)
	{
		cardsInfo = play.getThree_OneValue(move->cards);
		canCarryCards = VectorUtil::subVector(2,cardsInfo);
		if(cardsType == THREE_ONE)
		{	
			cutCarryCards1(move, canCarryCards, 1, 3, 1);
		}
		else
		{
 			int start = cardsInfo.at(0);
			int end = cardsInfo.at(1);
			int junkoNum = end-start+1;
			canCarryCards = VectorUtil::subVector(2,cardsInfo);

			cutCarryCards1(move, canCarryCards, junkoNum , 3 * junkoNum,1);
		}
		
	}
	else if(cardsType == THREE_TWO || cardsType == THREEJUNKO_TWO)
	{
		cardsInfo = play.getThree_TwoValue(move->cards);
		canCarryCards = VectorUtil::subVector(2,cardsInfo);

		if(cardsType == THREE_TWO)
		{	
			cutCarryCards1(move, canCarryCards, 2, 3, 0);
		}
		else
		{
 			int start = cardsInfo.at(0);
			int end = cardsInfo.at(1);
			int junkoNum = end-start+1;
			canCarryCards = VectorUtil::subVector(2,cardsInfo);

			cutCarryCards1(move, canCarryCards, 2 * junkoNum, 3 * junkoNum, 0);
		}
	}
	else if(cardsType==FOUR_TWO)
	{
		cardsInfo = play.getFour_TwoValue(move->cards);
		canCarryCards = VectorUtil::subVector(1,cardsInfo);

		cutCarryCards1(move, canCarryCards, 2, 4, 1);
	}
	else if(cardsType == FOUR_TWO_COUPLE)
	{
		cardsInfo = play.getFour_TwoCoupleValue(move->cards);
		canCarryCards = VectorUtil::subVector(1,cardsInfo);

		cutCarryCards1(move, canCarryCards, 4, 4, 0);
	}
	else
	{
		return ;
	}
}

void CEveluation::cutCarryCardsMoves(vector<CARDSMOVE> *moves)
{
	for (int i = moves->size() - 1; i >= 0;i--)
	{
		cutCarryCards(&moves->at(i));// 估值剪掉带牌
	}
}

void CEveluation::FilterForMoves(vector<CARDSMOVE> *moves,unsigned *cards)
{
	DDZMoveManager ddz_MM = DDZMoveManager();
	vector<CARDSMOVE> singleJunko = ddz_MM.getType8SingleJunko(*moves);


	for(size_t i=0;i < singleJunko.size();i++)
	{
		vector<CARDSMOVE>::iterator it ;
		int len =  singleJunko[i].cards.size();
		int single = 0, couple = 0, santiao = 0, bomb = 0;
		for(size_t j=0;j<len;j++)
		{
			int value = singleJunko[i].cards[j];
			
			if(cards[value]==1)
			{
				single++;
			}
			else if(cards[value]==2)
			{
				couple++;
			}
			else if(cards[value]==3)
			{
				santiao++;
			}
			else 
			{
				bomb++;
			}
		}
		
		if(single < (couple + santiao + 2 * bomb))
		{
			it = singleJunko.begin()+i;
			moves->erase(it);
		}
		
	}
}


/** 
*		对三带单/双、三顺带单/双进行剪带牌操作
*
*		@*move 被剪带牌的走步
*		@canCarryCards 带牌
*		@shouldCarryCards_num 应该带的牌数
*		@startIndex 带牌之前的坐标（不进行剪牌操作）
*		@flag 带的是单牌还是对牌？
*/
void CEveluation::cutCarryCards1(CARDSMOVE *move,
								 vector<unsigned> canCarryCards,int shouldCarryCards_num,
								 int startIndex, int  flag)
{
	int carryCards_Score[17]={0};
	int carryNum=canCarryCards.size();
	DDZMoveManager ddz_MM= DDZMoveManager();
	vector<CARDSMOVE> tmp_moves;
	vector<CARDSMOVE> single= ddz_MM.getGoodMove3Single(Player::p3_allMoves,tmp_moves,Player::p3_EachCardNum,1);
	vector<CARDSMOVE> couple= ddz_MM.getGoodMove4Couple(Player::p3_allMoves,tmp_moves,Player::p3_EachCardNum,1);

	if(flag)
	{
		if(single.size() > 0 && couple.size() > 0)
		{
			if(single[0].cards[0] > couple[0].cards[0])
			{
				move->score-=20;
			}
		}
	}
	else
	{
		if(single.size() > 0 && couple.size() > 0)
		{
			if(single[0].cards[0] < couple[0].cards[0])
			{
				move->score-=20;
			}
		}
	}

	for(size_t i=0;i<canCarryCards.size();i++)
	{
		if(flag)
		{
			if(ddz_MM.IsRelateMoves(single,canCarryCards[i]))
			{
				carryCards_Score[i]+=60;
			}
			else
			{
				carryCards_Score[i]-=30;
			}
		}
		else
		{
			if(ddz_MM.IsRelateMoves(couple,canCarryCards[i]))
			{
				carryCards_Score[i]+=60;
			}
			else
			{
				carryCards_Score[i]-=30;
			}
		}
	}
	vector<unsigned> carryCardsIndex=getMaxElementIndex(carryCards_Score,canCarryCards.size(),shouldCarryCards_num);
	
	for(size_t i=move->cards.size()-1;i>=startIndex;i--)
	{
		bool IsDelete = true;
		for(size_t j=0;j<carryCardsIndex.size();j++)
		{
			if(canCarryCards[carryCardsIndex[j]]==move->cards[i])
			{
				IsDelete = false;
				if (carryCards_Score[carryCardsIndex[j]] < 0)
				{
					move->score -=30;
				}
			}
		}

		if (IsDelete)
		{
			vector<unsigned>::iterator it = move->cards.begin();
			move->cards.erase(it+i);
		}
	}
}

//// 三带双、三顺带双剪掉带牌
//void CEveluation::cutCarryCards2(Player p,CARDSMOVE *move,vector<unsigned> canCarryCards,int shouldCarryCards_num)
//{
//	int carryCards_Score[17]={0};
//	int carryNum=canCarryCards.size();
//	DDZMoveManager ddz_MM= DDZMoveManager();
//	vector<CARDSMOVE> tmp_moves;
//	vector<CARDSMOVE> couple= ddz_MM.getGoodMove4Couple(Player::p3_allMoves,tmp_moves,p.p3_EachCardNum,1);
//	
//	for(size_t i=0;i<canCarryCards.size();i++)
//	{
//		if(ddz_MM.IsRelateMoves(couple,canCarryCards[i]))
//		{
//			carryCards_Score[i]+=60;
//		}
//		else
//		{
//			carryCards_Score[i]-=30;
//		}
//	}
//
//	vector<unsigned> carryCardsIndex=getMaxElementIndex(carryCards_Score,canCarryCards.size(),shouldCarryCards_num);
//	
//	for(size_t i=move->cards.size()-1;i>=3*shouldCarryCards_num/2;i--)
//	{
//		bool flag = true;
//		for(size_t j=0;j<carryCardsIndex.size();j++)
//		{
//			if(canCarryCards[carryCardsIndex[j]]==move->cards[i])
//			{
//				flag=false;	
//			}
//		}
//
//		if(flag)
//		{
//			vector<unsigned>::iterator it = move->cards.begin();
//			move->cards.erase(it+i);
//		}
//	}
//}
//
//// 四带二单剪掉带牌
//void CEveluation::cutCarryCards3(Player p,CARDSMOVE *move,vector<unsigned> canCarryCards,int shouldCarryCards_num)
//{
//	int carryCards_Score[17]={0};
//	int carryNum=canCarryCards.size();
//	DDZMoveManager ddz_MM= DDZMoveManager();
//	vector<CARDSMOVE> tmp_moves;
//	vector<CARDSMOVE> couple= ddz_MM.getGoodMove4Couple(p.p3_allMoves,tmp_moves,p.p3_EachCardNum,1);
//	
//	for(size_t i=0;i<canCarryCards.size();i++)
//	{
//		if(ddz_MM.IsRelateMoves(couple,canCarryCards[i]))
//		{
//			carryCards_Score[i]+=60;
//		}
//		else
//		{
//			carryCards_Score[i]-=30;
//		}
//	}
//	vector<unsigned> carryCardsIndex=getMaxElementIndex(carryCards_Score,canCarryCards.size(),2);
//	
//	for(size_t i=move->cards.size()-1;i>=4;i--)
//	{
//		bool flag = true;
//		for(size_t j=0;j<carryCardsIndex.size();j++)
//		{
//			if(canCarryCards[carryCardsIndex[j]]==move->cards[i])
//			{
//				flag=false;	
//			}
//		}
//
//		if(flag)
//		{
//			vector<unsigned>::iterator it = move->cards.begin();
//			move->cards.erase(it+i);
//		}
//	}
//}
//
//// 四带二对剪掉带牌
//void CEveluation::cutCarryCards4(Player p,CARDSMOVE *move,vector<unsigned> canCarryCards,int shouldCarryCards_num)
//{
//	int carryCards_Score[17]={0};
//	int carryNum=canCarryCards.size();
//	for(size_t i=0;i<canCarryCards.size();i++)
//	{
//		
//	}
//	vector<unsigned> carryCardsIndex=getMaxElementIndex(carryCards_Score,canCarryCards.size(),4);
//	
//	for(size_t i=move->cards.size()-1;i>=4;i--)
//	{
//		bool flag = true;
//		for(size_t j=0;j<carryCardsIndex.size();j++)
//		{
//			if(canCarryCards[carryCardsIndex[j]]==move->cards[i])
//			{
//				flag=false;	
//			}
//		}
//
//		if(flag)
//		{
//			vector<unsigned>::iterator it = move->cards.begin();
//			move->cards.erase(it+i);
//		}
//	}
//}
//

/** 获得数组元素中最大的几个元素 
*  @arr					数组
*  @len					数组长度
*  @top_num		多少个元素
*
*/
vector<unsigned> CEveluation::getMaxElementIndex(int arr[15],unsigned len,int top_num)
{
	vector<unsigned> top_vec;
	int tmp_arr[15];
	memcpy(tmp_arr, arr, sizeof(tmp_arr));

	int maxIndex=0;

	while(top_num--)
	{
		for(size_t i=1;i<len;i++)
		{
			if (tmp_arr[i]>tmp_arr[maxIndex] && !VectorUtil::isContains(top_vec, i))
			{
				maxIndex=i;
			}
		}

		top_vec.push_back(maxIndex);
		tmp_arr[maxIndex] = -999;
		maxIndex=0;

	}

	return top_vec;
}


/**
*  此函数只针对我方进行判断
*  
*/
//bool IsPassMove(Player p)
//{
//	if(p.p2_IsLandlord)
//	{
//		if(p.lastPlayer==1)
//		{
//			return true;
//		}
//
//		if(p.firstPlayer==2&&IsBadMove(p)&&IsNotSickMove(p))
//		{
//			return true;
//		}
//
//		if(p.firstPlayer==1&&IsBadMove(p))
//		{
//			return true;
//		}
//	}
//	else if(p.p1_IsLandlord)
//	{
//		if(p.lastPlayer==2&&IsTopMove(p))
//		{
//			return true;
//		}
//
//		if(p.lastPlayer==2&&IsBadMove(p))
//		{
//			return true;
//		}
//	}
//	else 
//	{
//		if(IsBadMove(p))
//		{
//			return true;
//		}
//	}
//}
//
//bool IsBadMove(Player p)
//{
//	return false;
//}
//
//bool IsNotSickMove(Player p)
//{
//	return false;
//}
//
//bool IsTopMove(Player p)
//{
//	return false;
//}

/**
*  在指定的范围中，牌的数量为对牌（单牌或三条、四个）的个数
*  @array 牌的数组形式
*  @num   指定的数量
*  @start 开始范围
*  @end   结束范围
*/
unsigned ComputeTotalOfNum(int *array,int num,unsigned start,unsigned end)
{
	unsigned count=0;
	unsigned len;

	GET_ARRAY_LEN(array,len);

	for(unsigned i=start;i<len&&i<=end;i++)
	{
		if(array[i]>=num)
		{
			count++;
		}
	}

	return count;
}


int CEveluation::getMinIndexOfCombs(CombsLib combs)
{
	int min=0;
	for(size_t i=1;i<combs.size();i++)
	{
		if(combs[i].moves.size()<combs[min].moves.size())
		{
			min=i;
		}
	}

	return min;
}

vector<unsigned> CEveluation::haveZhaDanOutside()
{
	vector<unsigned> zhaDans;
	for(size_t i=0;i<13;i++)
	{
		if(Player::remaining[i]==4)
		{
			zhaDans.push_back(i); 
		}
	}

	return zhaDans;
}

vector<unsigned> CEveluation::haveZhaDanP2()
{
	vector<unsigned> zhaDans;

	for(size_t i=0;i<13;i++)
	{
		if(Player::p2_EachCardNum[i]==4)
		{
			zhaDans.push_back(i);
		}
	}

	return zhaDans;
}

vector<unsigned> CEveluation::haveZhaDanP1()
{
	vector<unsigned> zhaDans;

	for(size_t i=0;i<13;i++)
	{
		if(Player::p1_EachCardNum[i]==4)
		{
			zhaDans.push_back(i);
		}
	}


	return zhaDans;
}


int CEveluation::IsPlay3Single(vector<CARDSMOVE> moves,unsigned *cards)
{
	DDZMoveManager ddz_MM = DDZMoveManager();
	vector<CARDSMOVE> tmp_single;
	vector<CARDSMOVE> single = ddz_MM.getGoodMove3Single(moves,tmp_single,cards,1);
	unsigned santiao_Num = ddz_MM.getGoodMove5Santiao(moves,tmp_single,cards,1).size();
	int k = single.size() - santiao_Num;
	if(single.size()>0 && k >= 4 && single[0].cards[0]<7)
	{
		return 1;
	}

	return 0;
}

int CEveluation::IsPlay4Couple(vector<CARDSMOVE> moves, unsigned *cards)
{
	DDZMoveManager ddz_MM = DDZMoveManager();
	vector<CARDSMOVE> tmp_couple;
	vector<CARDSMOVE> couple = ddz_MM.getGoodMove4Couple(moves,tmp_couple,cards,1);
	
	if(couple.size() >=4)
	{
		return 1;
	}

	return 0;
}

int CEveluation::IsPlay5Santiao(vector<CARDSMOVE> moves,unsigned *cards)
{
	DDZMoveManager ddz_MM = DDZMoveManager();
	vector<CARDSMOVE> tmp_couple;
	vector<CARDSMOVE> santiao = ddz_MM.getGoodMove5Santiao(moves,tmp_couple,cards,1);
	
	if(santiao.size() >=3)
	{
		return 1;
	}

	if(santiao.size() >=2 && santiao[santiao.size()-1].cards[0]>7)
	{ 
		return 1;
	}

	return 0;

}
/**
*	首攻
*/
CARDSMOVE CEveluation::firstAttack(vector<CARDSMOVE> moves)
{
	DDZMoveManager ddz_MM = DDZMoveManager();
	vector<CARDSMOVE> singles = ddz_MM.getType3Single(moves);
	vector<CARDSMOVE> couples = ddz_MM.getType4Couple(moves);
	vector<CARDSMOVE> santiaos = ddz_MM.getType5Santiao(moves);
	vector<CARDSMOVE> singleJunkos = ddz_MM.getType8SingleJunko(moves);
	CARDSMOVE m =CARDSMOVE();
	if (moves.size() > 0)
	{
		 m= moves[0];
	}
	else
	{
		NULL_MOVE(m);
	}

	if (singles.size() >= FA_Single)
	{
		return ddz_MM.getMinimumMove(singles, SINGLE);
	}

	if (couples.size() > FA_Couple)
	{
		return ddz_MM.getMinimumMove(couples, COUPLE);
	}

	if (santiaos.size() > FA_Santiao)
	{
		return ddz_MM.getMinimumMove(santiaos, SANTIAO);
	}

	if (singleJunkos.size() > 0)
	{
		return singleJunkos[0];
	}

	return m;
}