#include "MCSearch.h"


//MCSearch::MCSearch(void)
//{
//}
//
//
//MCSearch::~MCSearch(void)
//{
//}
//int MCSearch::SearchAGoodMove(int nDepth)
//{
//	int i;
//	i=IsGameOver(nDepth);
//	srand(time(NULL));
//	if(i!=0)
//	{
//		if(i>0)
//		{
//			VODList[MoveIndex]++;
//		}
//		else
//		{
//			VODList[MoveIndex]--;
//		}
//		return i;
//	}
//
//	if(nDepth<=0)
//	{
//		return 0;
//	}
//	CMoveGenerator *m_pMG;
//	m_pMG=(CMoveGenerator*)malloc(sizeof(CMoveGenerator));
//	CEveluation *m_pEve;
//	vector<CARDSMOVE> allMove = m_pMG->getMoves(nDepth);
//	m_pEve=(CEveluation*)malloc(sizeof(CEveluation));
//
//
//		if(nDepth==MAX_Depth)
//		{
//			m_pEve->NegativeSimulatedMenu();
//			MoveIndex=rand()%(allMove.size());//记录当前搜索的下标
//
//			int tmp_p1_Each[15],tmp_p2_Each[15],tmp_p3_Each[15];
//			vector<CARDSMOVE> tmp_p1_general,tmp_p2_general,tmp_p3_general;
//			int tmp_p1_cardsNum,tmp_p2_cardsNum,tmp_p3_cardsNum;
//
//			memcpy(tmp_p1_Each,p.p1_EachCardNum,sizeof(tmp_p1_Each));
//			memcpy(tmp_p2_Each,p.p2_EachCardNum,sizeof(tmp_p2_Each));
//			memcpy(tmp_p3_Each,p.p3_EachCardNum,sizeof(tmp_p3_Each));
//
//			tmp_p1_general=p.p1_general;
//			tmp_p2_general=p.p2_general;
//			tmp_p3_general=p.p3_general;
//
//			tmp_p1_cardsNum=p.p1_cardsNum;
//			tmp_p2_cardsNum=p.p2_cardsNum;
//			tmp_p3_cardsNum=p.p3_cardsNum;
//			if(MoveIndex==allMove.size()-1)
//			{
//				CARDSMOVE null_move;
//				vector<unsigned> tmp;
//				tmp.push_back(-1);
//				null_move.cards=tmp;
//				null_move.cardsType=-1;
//				MakeMove(null_move,nDepth);
//			}
//			else
//			{
//				MakeMove(allMove.at(MoveIndex),nDepth);// 第一层出牌
//			}
//			SearchAGoodMove(nDepth-1);
//			//UnMakeMove(allMove.at(i),p,nDepth);
//
//			memcpy(p.p1_EachCardNum,tmp_p1_Each,sizeof(p.p1_EachCardNum));
//			memcpy(p.p2_EachCardNum,tmp_p2_Each,sizeof(p.p2_EachCardNum));
//			memcpy(p.p3_EachCardNum,tmp_p3_Each,sizeof(p.p3_EachCardNum));
//
//			p.p1_general=tmp_p1_general;
//			p.p2_general=tmp_p2_general;
//			p.p3_general=tmp_p3_general;
//
//			p.p1_cardsNum=tmp_p1_cardsNum;
//			p.p2_cardsNum=tmp_p2_cardsNum;
//			p.p3_cardsNum=tmp_p3_cardsNum;
//		
//	}
//
//	else
//	{
//		
//		int randNum = rand()%allMove.size();
//
//		if(randNum>=allMove.size()-1)
//		{
//			CARDSMOVE null_move;
//			vector<unsigned> tmp;
//			tmp.push_back(-1);
//			null_move.cards=tmp;
//			null_move.cardsType=-1;
//
//			MakeMove(null_move,p,nDepth);
//			SearchAGoodMove(p,nDepth-1);
//		}
//		else
//		{
//			MakeMove(allMove.at(randNum),p,nDepth);
//			SearchAGoodMove(p,nDepth-1);
//		}
//	}
//	delete(m_pMG);
//}


//void MCSearch::SearchAGoodMove(Player p,int nDepth,int randTimes)
//{
//	CEveluation *m_pEval;
//	m_pEval=(CEveluation*)malloc(sizeof(CEveluation));
//	initBestMoveList();
//	for(size_t i=0;i<randTimes;i++)
//	{
//		m_pEval->NegativeSimulatedMenu();// 随机菜单启动：随机模拟对手手中牌
//		SearchAGoodMove(p,nDepth);
//		BestMoveList[getMaxElementIndex(VODList)]++;
//	}
//	delete(m_pEval);
//}

//vector<unsigned> MCSearch::SearchAGoodMove(Player p)
//{
//	CMoveGenerator *m_pMG;
//	m_pMG=(CMoveGenerator*)malloc(sizeof(CMoveGenerator));
//	vector<CARDSMOVE> myMoves;
//
//	SearchAGoodMove(p,MAX_Depth,RANDTIME);
//	int bestIndex = getMaxElementIndex(BestMoveList);
//	myMoves=m_pMG->getMoves(p,MAX_Depth);
//	if(myMoves.size()<=1)
//	{
//		vector<unsigned> tmp;
//		tmp.push_back(-1);
//		return tmp;
//	}
//
//	return myMoves[bestIndex].cards;
//}

//int MCSearch::getMaxElementIndex(int v[300])
//{
//	int maxIndex=0;
//	for(size_t i=1;i<300;i++)
//	{
//		if(v[i]>v[maxIndex])
//		{
//			maxIndex=i;
//		}
//	}
//
//	return maxIndex;
//}
//
//void MCSearch::initBestMoveList()
//{
//	for(size_t i=0;i<300;i++)
//	{
//		VODList[i]=0;
//		BestMoveList[i]=0;
//	}
//}