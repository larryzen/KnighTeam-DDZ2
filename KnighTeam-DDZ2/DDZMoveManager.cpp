#include "DDZMoveManager.h"
#include "MoveGenerator.h"


DDZMoveManager::DDZMoveManager(void)
{
}


DDZMoveManager::~DDZMoveManager(void)
{
}

vector<CARDSMOVE> DDZMoveManager::getType1Rocket(vector<CARDSMOVE> moves)
{
	return getMovesByType(moves,ROCKET);
}

vector<CARDSMOVE> DDZMoveManager::getType2Zhadan(vector<CARDSMOVE> moves)
{
	return getMovesByType(moves,ZHADAN);
}

vector<CARDSMOVE> DDZMoveManager::getType3Single(vector<CARDSMOVE> moves)
{
	return getMovesByType(moves,SINGLE);
}

vector<CARDSMOVE> DDZMoveManager::getType4Couple(vector<CARDSMOVE> moves)
{
	return getMovesByType(moves,COUPLE);
}

vector<CARDSMOVE> DDZMoveManager::getType5Santiao(vector<CARDSMOVE> moves)
{
	return getMovesByType(moves,SANTIAO);
}

vector<CARDSMOVE> DDZMoveManager::getType6Three_One(vector<CARDSMOVE> moves)
{
	return getMovesByType(moves,THREE_ONE);
}

vector<CARDSMOVE> DDZMoveManager::getType7Three_Two(vector<CARDSMOVE> moves)
{
	return getMovesByType(moves,THREE_TWO);
}

vector<CARDSMOVE> DDZMoveManager::getType8SingleJunko(vector<CARDSMOVE> moves)
{
	return getMovesByType(moves,SINGLEJUNKO);
}

vector<CARDSMOVE> DDZMoveManager::getType9DualJunko(vector<CARDSMOVE> moves)
{
	return getMovesByType(moves,DUALJUNKO);
}

vector<CARDSMOVE> DDZMoveManager::getType10ThreeJunko(vector<CARDSMOVE> moves)
{
	return getMovesByType(moves,THREEJUNKO);
}

vector<CARDSMOVE> DDZMoveManager::getType11ThreeJunko_One(vector<CARDSMOVE> moves)
{
	return getMovesByType(moves,THREEJUNKO_ONE);
}

vector<CARDSMOVE> DDZMoveManager::getType12ThreeJunko_Two(vector<CARDSMOVE> moves)
{
	return getMovesByType(moves,THREEJUNKO_TWO);
}

vector<CARDSMOVE> DDZMoveManager::getType13Four_Two(vector<CARDSMOVE> moves)
{
	return getMovesByType(moves,FOUR_TWO);
}

vector<CARDSMOVE> DDZMoveManager::getType14Four_Two_Couple(vector<CARDSMOVE> moves)
{
	return getMovesByType(moves,FOUR_TWO_COUPLE);
}

vector<CARDSMOVE> DDZMoveManager::getMovesByType(vector<CARDSMOVE> moves,int type)
{
	vector<CARDSMOVE> thisTypeMoves;
	for(int i=0;i<moves.size();i++)
	{
		if(moves[i].cardsType==type)
		{
			thisTypeMoves.push_back(moves[i]);
		}
	}

	return thisTypeMoves;
}

int DDZMoveManager::IsRelateMove(CARDSMOVE move,int n)
{
	if(move.cardsType==-1)
		return 0;
	
	for(size_t i=0;i<move.cards.size();i++)
	{
		if(n==move.cards[i])
			return 1;
	}

	return 0;
}

int DDZMoveManager::IsRelateMoves(vector<CARDSMOVE> moves,int n)
{
	
	for(size_t i=0;i<moves.size();i++)
	{
		for(size_t j=0;j<moves[i].cards.size();j++)
		{
			if(moves[i].cards[j]==n)
				return 1;
		}
	}

	return 0;
}

vector<CARDSMOVE> DDZMoveManager::dealMoves(vector<CARDSMOVE> moves,unsigned *cards)
{
	vector<CARDSMOVE> zhadan =  getType2Zhadan(moves);
	vector<CARDSMOVE> single   =  getType3Single(moves);
	vector<CARDSMOVE> couple  =	 getType4Couple(moves);
	vector<CARDSMOVE> santiao =  getType5Santiao(moves);

	vector<CARDSMOVE> singleJunko =  getType8SingleJunko(moves);
	vector<CARDSMOVE> dualJunko    =  getType9DualJunko(moves);

	CARDSMOVE singleJunkoMove;
	CARDSMOVE dualJunkoMove;
	int k = getMaxScoreMove(singleJunko);
	int m= getMaxScoreMove(dualJunko);
	
	if(k>=0)
	{
		singleJunkoMove = singleJunko[k];
	}
	else
	{
		NULL_MOVE(singleJunkoMove);
	}
	if(m>=0)
	{
		dualJunkoMove = dualJunko[m];
	}
	else
	{
		NULL_MOVE(dualJunkoMove);
	}
	for(int i=single.size()-1;i>=0;i--)
	{
		int n = single[i].cards[0];
		if(IsRelateMove(singleJunkoMove,n) && cards[n]==1)
		{
			vector<CARDSMOVE>::iterator it = single.begin();
			single.erase(it+i);
		}
		if(!IsRelateMove(singleJunkoMove,n)&&!IsRelateMove(dualJunkoMove,n) && (cards[n]>=2))
		{
			vector<CARDSMOVE>::iterator it = single.begin();
			single.erase(it+i);
		}
	}

	/*for(int t=0;t<santiao.size();t++)
	{
		if(single.size()>0)
		{
			vector<CARDSMOVE>::iterator it = single.begin();
			single.erase(it);
		}
	}*/

	return single;

}


int DDZMoveManager::getMaxScoreMove(vector<CARDSMOVE> moves)
{
	int max = 0;
	int maxScore=0;
	if(moves.size()>0)
	{
		maxScore=moves[0].score;
	}
	else
	{
		return -1;
	}
	for(size_t i=0;i<moves.size();i++)
	{
		if(moves[i].score>maxScore)
		{
			max = i;
			maxScore=moves[i].score;
		}
	}


	return max;
}

vector<CARDSMOVE> DDZMoveManager::getGoodMove3Single(vector<CARDSMOVE> moves,
																vector<CARDSMOVE> single,unsigned *cards,int flag)
{
	if(flag)
	{
		single = getType3Single(moves);
	}
	vector<CARDSMOVE> singleJunko =  getType8SingleJunko(moves);
	vector<CARDSMOVE> dualJunko    =  getType9DualJunko(moves);

	CARDSMOVE singleJunkoMove;
	CARDSMOVE dualJunkoMove;
	int k = getMaxScoreMove(singleJunko);
	int m= getMaxScoreMove(dualJunko);
	
	if(k!=-1 && singleJunko[k].score>=0)
	{
		singleJunkoMove = singleJunko[k];
	}
	else
	{
		NULL_MOVE(singleJunkoMove);
	}
	if(m!=-1 && dualJunko[m].score>=0)
	{
		dualJunkoMove = dualJunko[m];
	}
	else
	{
		NULL_MOVE(dualJunkoMove);
	}
	for(int i=single.size()-1;i>=0;i--)
	{
		int value = single[i].cards[0];
		if(IsRelateMove(singleJunkoMove,value) && cards[value]==1)
		{
			vector<CARDSMOVE>::iterator it = single.begin();
			single.erase(it+i);
		}
		if(!IsRelateMove(singleJunkoMove,value)
			&& !IsRelateMove(dualJunkoMove,value) 
			&& (cards[value]>=2))
		{
			vector<CARDSMOVE>::iterator it = single.begin();
			single.erase(it+i);
		}
	}

	return single;
}

vector<CARDSMOVE> DDZMoveManager::getGoodMove4Couple(vector<CARDSMOVE> moves,
														vector<CARDSMOVE> couple,unsigned *cards,int flag)
{
	if(flag)
	{
		couple = getType4Couple(moves);
	}
	vector<CARDSMOVE> singleJunko =  getType8SingleJunko(moves);
	vector<CARDSMOVE> dualJunko    =  getType9DualJunko(moves);

	CARDSMOVE singleJunkoMove;
	CARDSMOVE dualJunkoMove;
	int k = getMaxScoreMove(singleJunko);
	int m= getMaxScoreMove(dualJunko);

	if(k!=-1 && singleJunko[k].score>=0)
	{
		singleJunkoMove = singleJunko[k];
	}
	else
	{
		NULL_MOVE(singleJunkoMove);
	}
	if(m!=-1 && dualJunko[m].score>=0)
	{
		dualJunkoMove = dualJunko[m];
	}
	else
	{
		NULL_MOVE(dualJunkoMove);
	}

	for(int i = couple.size()-1;i>=0;i--)
	{
		int value = couple[i].cards[0];   
		if(IsRelateMove(singleJunkoMove, value)&& cards[value]==2)
		{
			vector<CARDSMOVE>::iterator it = couple.begin();
			couple.erase(it+i);
		}

		if(!IsRelateMove(singleJunkoMove, value) 
			&& !IsRelateMove(dualJunkoMove, value)
			&& cards[value]>2)
		{
			vector<CARDSMOVE>::iterator it = couple.begin();
			couple.erase(it+i);
		}
	}

	return couple;
}

/** 设置一系列走步的status */
void DDZMoveManager::setMovesStatus(Player p,vector<CARDSMOVE> *moves)
{
	for(size_t i=0;i<moves->size();i++)
	{
		setStatus(p,&moves->at(i));
	}
}


/** 设置某一走步的status */
void DDZMoveManager::setStatus(Player p,CARDSMOVE *move)
{
	CMoveGenerator ddz_MG = CMoveGenerator();
	vector<CARDSMOVE> moves = ddz_MG.getMovesByOthers(*move,p.remaining);

	if(moves.size()==0)
	{
		move->status=STATUS_MAX;
		return ;
	}

	int value= getMoveValue(*move);

	if(value>=0 && value<=4)
	{
		move->status = STATUS_SMALL;
		return ;
	}
	else if(value >= 5 && value<= 9)
	{
		move->status = STATUS_MID;
		return ;
	}
	else if(value >= 10 && value <=14)
	{
		move->status = STATUS_LARGE;
		return ;
	}
}

int DDZMoveManager::IsStatus1Max(Player p,CARDSMOVE move)
{
	CMoveGenerator ddz_MG = CMoveGenerator();
	vector<CARDSMOVE> moves = ddz_MG.getMovesByOthers(move,p.remaining);
	if(moves.size()==0)
	{
		return 1;
	}

	return 0;
}

int DDZMoveManager::IsStatus2Large(Player p,CARDSMOVE move)
{
	return 0;
}

int DDZMoveManager::IsStatus3Mid(Player p,CARDSMOVE move)
{
	return 0;
}
int DDZMoveManager::IsStatus4Small(Player p,CARDSMOVE move)
{
	return 0;
}

int DDZMoveManager::getMoveValue(CARDSMOVE move)
{
	int cardsType = move.cardsType;
	switch(cardsType)
	{
	case ROCKET:
		{
			return 14;
		}
	case ZHADAN:
		{
			return 14;
		}
	case SINGLE:
		{
			return move.cards[0];
			break;
		}
	case COUPLE:
		{
			return move.cards[0];
			break;
		}
	case SANTIAO:
		{
			return move.cards[0];
			break;
		}
	case THREE_ONE:
		{
			return move.cards[0];
			break;
		}
	case THREE_TWO:
		{
			return move.cards[0];
			break;
		}
	case SINGLEJUNKO:
		{
			return move.cards.size();
			break;
		}
	case DUALJUNKO:
		{
			return 9;
			break;
		}
	case THREEJUNKO:
		{
			return 14;
			break;
		}
	case THREEJUNKO_ONE:
		{
			return 14;
			break;
		}
	case THREEJUNKO_TWO:
		{
			return 14;
			break;
		}
	case FOUR_TWO:
		{
			return 14;
			break;
		}
	case FOUR_TWO_COUPLE:
		{
			return 14;
			break;
		}
	default:
		return -1;
	}
}

vector<CARDSMOVE> DDZMoveManager::getCombBySocre(Player p)
{
	CEveluation ddz_Eve = CEveluation();
	CMoveGenerator ddz_MG = CMoveGenerator();
	DDZMoveManager ddz_MM = DDZMoveManager();
	vector<CARDSMOVE> comb;

	unsigned tmp_cards[15];
	memcpy(tmp_cards,p.p3_EachCardNum,sizeof(tmp_cards));
	int tmp_cardsNum = p.p3_cardsNum;

	while(tmp_cardsNum)
	{
		
		vector<CARDSMOVE> moves = ddz_MG.getMovesByMyself(tmp_cardsNum,tmp_cards);
	
		for(size_t i=0;i<moves.size();i++)
		{
			ddz_Eve.EveluateMove(p,&moves[i],3);
		}
		for(size_t k=0;k<moves.size();k++)
		{
			ddz_Eve.cutCarryCards(p,&moves[k]);
		}
		int selectedMove = getMaxScoreMove(moves);
		CARDSMOVE selected = moves[selectedMove];
		comb.push_back(selected);
		for(size_t j=0;j<selected.cards.size();j++)
		{
			tmp_cards[selected.cards[j]]--;
		}
	
		tmp_cardsNum-=selected.cards.size();
	}

	return comb;
}