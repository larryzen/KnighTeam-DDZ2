#include "AlphaBeta.h"
#include "FileWriter.h"
#include "sstream"
#include "DDZMoveManager.h"

AlphaBeta::AlphaBeta(void):score(-99999),currentScore(0)
{
	junkoNum=0;
};

AlphaBeta::~AlphaBeta(void)
{
}

CARDSMOVE AlphaBeta::bestMove;
int AlphaBeta::SearchAGoodMove(Player p,int nDepth)
{
	DDZMoveManager ddz_MM = DDZMoveManager();
	int current=-99999;
	CMoveGenerator *m_pMG;
	CEveluation *m_pEve;
	m_pMG=(CMoveGenerator*)malloc(sizeof(CMoveGenerator));
	m_pEve=(CEveluation*)malloc(sizeof(CEveluation));

	
	vector<CARDSMOVE> moves = m_pMG->getMoves(p,nDepth);
	FileWriter fw = FileWriter("m.txt");

	if(moves.size()>0&&moves[0].outWay==1)
	{
		p.p3_allMoves=moves;
	}

	m_pEve->RandMenu(p);

	for(size_t i=0;i!=moves.size();i++)
	{	
		m_pEve->EveluateMove(p,&moves[i],nDepth);
	}
	for(size_t i=0;i!=p.p3_allMoves.size();i++)
	{	
		m_pEve->EveluateMove(p,&p.p3_allMoves[i],nDepth);
	}
	

	for(size_t i=0;i<moves.size();i++)
	{
		m_pEve->cutCarryCards(p,&moves[i]);// 估值剪掉带牌
	}
	for(size_t i=0;i<p.p3_allMoves.size();i++)
	{
		m_pEve->cutCarryCards(p,&p.p3_allMoves[i]);
	}

	if(moves.size()==0)
	{
		NULL_MOVE(bestMove);	
		return 0;
	}
	if(m_pEve->IsAiPass(p))	//让同伴权
	{
		NULL_MOVE(bestMove);	
		return 0;
	}

	vector<CARDSMOVE> comb = ddz_MM.getCombBySocre(p);
	ddz_MM.setMovesStatus(p,&comb);
	for(size_t i=0;i<moves.size();i++)
	{
		if(i>=0&&i<moves.size())
		{ 
			if(nDepth==AlphaBeta_Depth)
			{
				if(current<moves[i].score)
				{
					bestMove=moves[i];
					current=moves[i].score;
				}
			}	
		}
	}
	int way = moves[0].outWay;
	if(way)//主动出牌
	{
		int k =ddz_MM.getMaxScoreMove(p.p3_allMoves);

		if(k>=0)
			bestMove=p.p3_allMoves[k];
		else
			NULL_MOVE(bestMove);
		return 0;
	}
	else        //被动出牌
	{
		return PassivePlay(moves,p);
	}

	
	if(current<0)
	{
		NULL_MOVE(bestMove);	
	}
	//fw.WriteToFile(moves,bestMove);写入文件
	//if(moves.size()==1)
	//{
	//	bestMove.cards.clear();
	//	bestMove.cardsType=-1;
	//}
	delete(m_pEve);
	delete(m_pMG);
	m_pEve=NULL;
	m_pMG=NULL;
	return 0;
}

/** 被动出牌查找走步 */
int AlphaBeta::PassivePlay(vector<CARDSMOVE> moves,Player p)
{
	DDZMoveManager ddz_MM = DDZMoveManager();
	//被动出牌
	int cardsType = moves[0].cardsType;
	switch(cardsType)
	{
	case SINGLE:
		{
			vector<CARDSMOVE> single= ddz_MM.getGoodMove3Single(p.p3_allMoves,moves,p.p3_EachCardNum,0);
			if(single.size()==0)
			{
				NULL_MOVE(bestMove);
				return 0;
			}
			if(p.p1_IsLandlord)
			{
				bestMove=single[single.size()-1];
			}
			else
			{
				bestMove=single[0];
			}
			return 0;
			break;
		}
	case COUPLE:
		{
			vector<CARDSMOVE> couple= ddz_MM.getGoodMove4Couple(p.p3_allMoves,
					moves,p.p3_EachCardNum,0);
			if(couple.size()==0)
			{
				NULL_MOVE(bestMove);
				return 0;
			}
			if(p.p1_IsLandlord)
			{
				bestMove=couple[couple.size()-1];
			}
			else
			{
				bestMove=couple[0];
			}
			return 0;
			break;
		}
	case THREE_ONE:	
		{
			vector<CARDSMOVE> tmp_single;
			vector<CARDSMOVE> single= ddz_MM.getGoodMove3Single(p.p3_allMoves,tmp_single,p.p3_EachCardNum,1);
			if(single.size()>0)
			{
				for(int i=moves[0].cards.size()-1;i>2;i--)
				{
					vector<unsigned>::iterator it = moves[0].cards.begin();
					moves[0].cards.erase(it+i);
				}
				moves[0].cards.push_back(single[0].cards[0]);	
				bestMove=moves[0];
			}
			else
				NULL_MOVE(bestMove);
			return 0;
			break;
		}
	case THREE_TWO:
		{
			vector<CARDSMOVE> tmp_couple;
			vector<CARDSMOVE> couple= ddz_MM.getGoodMove4Couple(p.p3_allMoves,
				tmp_couple,p.p3_EachCardNum,1);
			if(couple.size()>0)
			{
				for(int i=moves[0].cards.size()-1;i>2;i--)
				{
					vector<unsigned>::iterator it = moves[0].cards.begin();
					moves[0].cards.erase(it+i);
				}
				moves[0].cards.push_back(couple[0].cards[0]);	
				moves[0].cards.push_back(couple[0].cards[0]);
				bestMove=moves[0];
			}
			else
				NULL_MOVE(bestMove);
			return 0;
			break;
		}
	}		
}