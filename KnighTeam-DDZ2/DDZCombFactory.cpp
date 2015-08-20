#include "DDZCombFactory.h"
#include "VectorUtil.h"
#include "Player.h"
#include "DDZMoveManager.h"

DDZCombFactory::DDZCombFactory(void)
{
	comb = Comb();
	comb.moves = vector<CARDSMOVE>();
	combs = CombsLib();
}

DDZCombFactory::DDZCombFactory(unsigned cards[15], int cardsNum)
{
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			flag[i][j]=false;
		}
	}
	comb = Comb();
	comb.moves = vector<CARDSMOVE>();
	combs = CombsLib();
	memcpy(this->comb_cards, cards, sizeof(this->comb_cards));
	this->onHandCardsNum = cardsNum;
	int tmp_cardsNum = onHandCardsNum;
	getCombLeastSingle(this->comb_cards, &tmp_cardsNum);
}

DDZCombFactory::~DDZCombFactory(void)
{
}


void DDZCombFactory::getCombLeastSingle(unsigned *cards, int *cardsNum)
{
	if ((*cardsNum) == 0)
		return;

	filters(cards, cardsNum);

	int index = -1;
	int start = -1;
	int end = -1;
	for (int i = 0; i < 15; i++)
	{
		if (cards[i] != 0)
		{
			if (start == -1)
			{
				start = i;


				for (int j = i; j < 15; j++)
				{
					if (cards[j] != 0)
					{
						end = j;
					}
					else
					{
						i = j;
						index++;
						break;
					}
				}
				int junkoLength = end - start + 1;
				int len = comb.moves.size();
				int tmp_cardsNum = (*cardsNum);
				if (junkoLength >= 5)
				{
					
					getBasicTypeComb(cards, start, end, tmp_cardsNum);
					removeMove(&comb.moves, len, comb.moves.size());
					canMakeByJunko(cards, start, end,  cardsNum, 1);
					

				}
				else if (junkoLength >= 3)
				{
					
					getBasicTypeComb(cards, start, end, tmp_cardsNum);
					removeMove(&comb.moves, len, comb.moves.size());
					canMakeByJunko(cards, start, end, cardsNum, 2);
					

				}
				else if (junkoLength >= 2)
				{
					getBasicTypeComb(cards, start, end, tmp_cardsNum);
					removeMove(&comb.moves, len, comb.moves.size());
					canMakeByJunko(cards, start, end, cardsNum, 3);
					
				}
				else
				{
					
				}
				start = -1;
				end = -1;
			}
		}
	}
}

/**
*   �������ƽ��в����ϣ����ɸ������
*   @*cards ������
*   @start n˳���
*	@end n˳�յ�
*	@*cardsNum ����������
*   @nJunko ��˳/˫˳/��˳
*/
void DDZCombFactory::canMakeByJunko(unsigned *cards, int start, int end, int *cardsNum, int nJunko)
{
	int leastJunkoNum, tmp_cardsType;
	int junkoLength = end - start + 1;
	leastJunkoNum = getLeastJunkoNum(nJunko);
	tmp_cardsType = getCardsTypeByNJunko(nJunko);
	
	if (junkoLength < leastJunkoNum || *cardsNum == 0)
	{

		return;
	}
	else
	{
		flag[start][end]=true;
		CARDSMOVE move;
		

		for (int i = start; i <= end; i++)
		{
			for (int j = 0; j < nJunko; j++)
			{
				move.cards.push_back(i);
			}
		}
		move.cardsType =tmp_cardsType;
		

		int len = comb.moves.size();
		comb.moves.push_back(move);


		int tmp_cards[15];
		int tmp_cardsNum = *cardsNum;
		*cardsNum -= junkoLength * nJunko;
		memcpy(tmp_cards, cards, sizeof(tmp_cards));
		for (int i = start; i <= end; i++)
		{
			cards[i] -= nJunko;
		}

		while (*cardsNum > 0)
		{
			getCombLeastSingle(cards, cardsNum);
		}

		addComb();//��һ�������ӵ���Ͽ���
		removeMove(&comb.moves, len, comb.moves.size());//comb����

		if (junkoLength > leastJunkoNum)
		{
			if (!flag[start][end-1])
			{
				*cardsNum = tmp_cardsNum;
				memcpy(cards, tmp_cards, sizeof(tmp_cards));
				canMakeByJunko(cards, start, end - 1, cardsNum, nJunko);

				*cardsNum = tmp_cardsNum;
				memcpy(cards, tmp_cards, sizeof(tmp_cards));
				canMakeByJunko(cards, start + 1, end, cardsNum, nJunko);
			}
			
			if (!flag[start+1][end])
			{
				*cardsNum = tmp_cardsNum;
				memcpy(cards, tmp_cards, sizeof(tmp_cards));
				canMakeByJunko(cards, start + 1, end, cardsNum, nJunko);
			}
			
		}
		else
		{
			//�Ż��ݹ�
		}
	}
	return;
}

/**
*   �����ϵ���Ͽ���
*/
void DDZCombFactory::addComb()
{
	if (IsCorrect(comb, this->onHandCardsNum) && !IsContainsComb(combs, comb))
	{
		Comb newComb = comb;//c++ vetor��=��ֵ�����
		combs.push_back(newComb);
		//newComb.moves = vector<CARDSMOVE>();//��ʼ��ָ��
		//combs.push_back(newComb);
		//for (int i = 0; i < comb.moves.size(); i++)
		//{
		//	combs.at(combs.size()-1).moves.push_back(comb.moves.at(i));
		//}
	}
}

void DDZCombFactory::removeMove(vector<CARDSMOVE> *moves, int start, int end)
{
	vector<CARDSMOVE>::iterator it=moves->begin();

	moves->erase(it+start, it+end);
}

void DDZCombFactory::removeMove(vector<CARDSMOVE> *moves, int index)
{
	vector<CARDSMOVE>::iterator it = moves->begin();
	moves->erase(it+index);
}

/**
*    ɸѡ����������
*/
void DDZCombFactory::filters(unsigned *cards, int *cardsNum)
{
	for (size_t i = 0; i < 13; i++)
	{
		CARDSMOVE move= CARDSMOVE();
		
		switch (cards[i])
		{
			case 1:
			{
					  if (!IsRelateToJunko(cards, i, 1))
					  {
						  VectorUtil::addElement(&move.cards, i, cards[i]);
						  move.cardsType =SINGLE;
						  
						  comb.moves.push_back(move);
						  cards[i] -= 1;
						  *cardsNum -= 1;
					  }
					  break;
			}
			case 2:
			{
					  if (!IsRelateToJunko(cards, i, 1) && !IsRelateToJunko(cards, i, 2))
					  {
						  VectorUtil::addElement(&move.cards, i, cards[i]);
						  move.cardsType = COUPLE;
						  comb.moves.push_back(move);
						  cards[i] -= 2;
						  *cardsNum -= 2;
					  }
					  break;
			}
			case 3:
			{
					  if (!IsRelateToJunko(cards, i, 1) && !IsRelateToJunko(cards, i,  2)
						  && !IsRelateToJunko(cards, i, 3))
					  {
						  VectorUtil::addElement(&move.cards, i, cards[i]);
						  move.cardsType = SANTIAO;
						  comb.moves.push_back(move);
						  cards[i] -= 3;
						  *cardsNum -= 3;
					  }
					  break;
			}
			case 4:
			{
					  if (!IsRelateToJunko(cards, i, 1) && !IsRelateToJunko(cards, i, 2)
						  && !IsRelateToJunko(cards, i, 3))
					  {
						  VectorUtil::addElement(&move.cards, i, cards[i]);
						  move.cardsType = ZHADAN;
						  comb.moves.push_back(move);
						
						  cards[i] -= 4;
						  *cardsNum -= 4;
					  }
					  break;
			}
		}
	}

	if (cards[13] == 1 && cards[14] == 1)
	{
		CARDSMOVE move = CARDSMOVE();
		move.cards.push_back(13);
		move.cards.push_back(14);
		move.cardsType = ROCKET;

		comb.moves.push_back(move);
		cards[13] -= 1;
		cards[14] -= 1;
		*cardsNum -= 2;
	}
	else
	{
		CARDSMOVE move = CARDSMOVE();
		move.cardsType = SINGLE;
		if (cards[13] == 1 && cards[14] == 0)
		{
			move.cards.push_back(13);
			comb.moves.push_back(move);
			cards[13] -= 1;
			*cardsNum -= 1;
		}
		else if (cards[13] == 0 && cards[14] == 1)
		{
			move.cards.push_back(14);
			comb.moves.push_back(move);
			cards[14] -= 1;
			*cardsNum -= 1;
		}
	}
	if ((*cardsNum) == 0)
	{
		addComb();
		return;
	}
}
 

/*  ɸѡvalue�����Ƿ���ָ��˳�����
*	@*cards ������
*	@value ָ������ֵ
*	@nJunko ����˫����˳
*/
bool DDZCombFactory::IsRelateToJunko(unsigned *cards, int value, int nJunko)
{
	int LeastJunkoNum = getLeastJunkoNum(nJunko);
	int start = value - LeastJunkoNum + 1;

	for (int i = start; i <= value; i++)
	{
		if (i < 0) // ˳������Խ��
		{
			i = -1;
			continue;
		}

		int count = 0;
		for (int j = i; j < i + LeastJunkoNum && j <= A; j++)//�жϰ���value��junkoNum��˳
		{
			if (cards[j] >= nJunko)
			{
				count++;
			}
			else
			{
				break;
			}
		}

		if (count == LeastJunkoNum)// ����value�������junkoNum��˳
		{
			return true;
		}
	}

	return false;
}

/**
*	�ж����ɵ�����Ƿ���ȷ��
*   ���Ƶ������Ƿ��������������ͬ
*/
bool DDZCombFactory::IsCorrect(Comb comb, int OnHandCardsNum)
{
	int sum = 0;
	for (int i = 0; i < comb.moves.size(); i++)
	{
		sum += comb.moves.at(i).cards.size();
	}

	return sum == OnHandCardsNum ? true : false;
}
/**
*	����n˳�����������Ҫ������˳
*   �磺��˳������Ҫ5��˳
*/
int DDZCombFactory::getLeastJunkoNum(int nJunko)
{
	return (nJunko == 1 ? 5 : 
				(nJunko == 2 ? 3 : 
					(nJunko == 3 ? 2 : 1)));
}

/**
*	����n˳���������
*	�磺��˳����Ϊ SINGLEJUNKO -> 8
*/
int DDZCombFactory::getCardsTypeByNJunko(int nJunko)
{
	return (nJunko == 1 ? SINGLEJUNKO : 
				(nJunko == 2 ? DUALJUNKO : 
					(nJunko == 3 ? THREEJUNKO : INVALID)));
}

int DDZCombFactory::getCardsTypeByBasicCards(int cardsNum)
{
	return (cardsNum == 1 ? SINGLE : 
				(cardsNum == 2 ? COUPLE : 
					(cardsNum == 3 ? SANTIAO : 
						(cardsNum == 4 ? ZHADAN : INVALID))));
}

bool DDZCombFactory::IsContainsComb(CombsLib combs, Comb comb)
{
	if (combs.empty())
	{
		return false;
	}

	for (size_t i = 0; i < combs.size(); i++)
	{
		if (IsSameComb(combs[i], comb))
			return true;
	}

	return false;
}
bool DDZCombFactory::IsSameComb(Comb c1, Comb c2)
{
	int len1 = c1.moves.size();
	int len2 = c2.moves.size();
	if (len1 != len2)
		return false;
	for (size_t i = 0; i < len1; i++)
	{
		if (!IsContainsCardsMove(c1.moves, c2.moves.at(i)))
				return false;	
	}
	return true;
}

bool DDZCombFactory::IsSameCardsMove(CARDSMOVE cm1, CARDSMOVE cm2)
{
	if (cm1.cardsType!=cm2.cardsType)
		return false;
	if (cm1.cards.size() != cm2.cards.size())
		return false;
	if (VectorUtil::IsSameVector(cm1.cards, cm2.cards))
	{
		return true;
	}

	return false;
}

bool DDZCombFactory::IsContainsCardsMove(vector<CARDSMOVE> cms, CARDSMOVE cm)
{
	for (size_t i = 0; i < cms.size(); i++)
	{
		if (IsSameCardsMove(cms[i], cm))
			return true;
	}

	return false;
}

CombsLib DDZCombFactory::getCombs()
{
	int tmp_cardsNum = onHandCardsNum;
	unsigned tmp_cards[15];
	memcpy(tmp_cards, comb_cards, sizeof(tmp_cards));

	getCombLeastSingle(tmp_cards, &tmp_cardsNum);
	
	
	return combs;
}

/**
*	��ó�����ϣ�����N˳�����
*/
void DDZCombFactory::getBasicTypeComb(unsigned *cards, int start, int end, int cardsNum)
{
	for (size_t i = start; i <= end; i++)
	{
		CARDSMOVE move = CARDSMOVE();
		VectorUtil::addElement(&move.cards, i, cards[i]);
		move.cardsType = getCardsTypeByBasicCards(cards[i]);
		if (move.cardsType != INVALID)
		{		
			comb.moves.push_back(move);
		}
	}

	
	
	//����Ӧ�õ���canMakeJunko(...);��Ȼ������ǲ���ȫ?
	addComb();//�����������������������
	return;
}

/**
*   �ҵ����ٵ������
*/
Comb DDZCombFactory::getComb1LeastSingle()
{
	int index = -1;
	int currentSingleNum=20;
	CombsLib combsSingle = combs;
	for (size_t i = 0; i < combsSingle.size(); i++)
	{
		setSingleNum(&combsSingle.at(i));
		setGain(&combsSingle.at(i));
		int singleNum = combsSingle.at(i).singleNum - combsSingle.at(i).gain * 2;
		//ȡ���ٵ������
		if (compare(currentSingleNum, singleNum) > 0)
		{
			currentSingleNum = singleNum;
			index = i;
		}
		else if (currentSingleNum == singleNum)
		{
			//������������ȣ���ȡ������
			if (compare(combsSingle[index].gain, combsSingle[i].gain) < 0)
			{
				currentSingleNum = combsSingle.at(i).singleNum;
				index = i;
			}
			else if (compare(combsSingle[index].gain, combsSingle[i].gain) == 0)
			{
				int movesSize1 = combsSingle[index].moves.size() - combsSingle[index].gain * 2;
				int movesSize2 = combsSingle[i].moves.size() - combsSingle[i].gain * 2;
				// ��������ȣ���ȡ��ϲ����ٵ�
				if (compare(movesSize1, movesSize2) > 0)
				{
					currentSingleNum = combsSingle.at(i).singleNum;
					index = i;
				}
			}		
		}
	}
	if (index == -1)
		return Comb();

	return combsSingle[index];
}

/**
*	�ҵ����ٲ��������
*/
Comb DDZCombFactory::getComb2LeastMoves()
{
	int index = -1;
	int currentMovesNum = 21;
	CombsLib combsLeastMoves = combs;

	for (size_t i = 0; i < combsLeastMoves.size(); i++)
	{
		setSingleNum(&combsLeastMoves.at(i));
		setGain(&combsLeastMoves.at(i));
		// ȡ���ٲ������
		if (currentMovesNum>combsLeastMoves.at(i).moves.size())
		{
			currentMovesNum = combsLeastMoves.at(i).moves.size();
			index = i;
		}
		else if (currentMovesNum == combsLeastMoves.at(i).moves.size())
		{
			//����ϲ�����ȣ���ȡ������
			if (compareGain(combsLeastMoves[index], combsLeastMoves[i]) > 0)
			{
				currentMovesNum = combsLeastMoves.at(i).moves.size();
				index = i;
			}
			else if (compareGain(combsLeastMoves[index], combsLeastMoves[i]) == 0)
			{
				// ��������ȣ���ȡ��ϲ����ٵ�
				if (compareSingleNum(combsLeastMoves[index], combsLeastMoves[i]) > 0)
				{
					currentMovesNum = combsLeastMoves.at(i).moves.size();
					index = i;
				}
			}
				
		}
	}
	if (index == -1)
		return combsLeastMoves[0];

	return combsLeastMoves[index];
}

/**
*	�ҵ������������
*/
Comb DDZCombFactory::getComb3MaxGain()
{
	int currentGain = -1;
	int currentIndex = -1;
	CombsLib combsLib = combs;
	for (int i = 0; i < combsLib.size(); i++)
	{
		setSingleNum(&combsLib.at(i));//Ϊʲô���ﲻ����ϿⲻԤ�ȴ�����Ϊ���뱣����ԭ����Ͽ�...�����
		setGain(&combsLib[i]);
		if (currentGain < combsLib[i].gain)
		{
			currentGain = combsLib[i].gain;
			currentIndex = i;
		}
		else if (currentGain == combsLib[i].gain)//���������
		{
			if (compareSingleNum(combsLib[currentIndex], combsLib[i]) > 0)//ȡ���������ٵ�
			{
				currentGain = combsLib[i].gain;
				currentIndex = i;
			}
			else if (compareSingleNum(combsLib[currentIndex], combsLib[i]) == 0)//�����������
			{
				if (compareMovesNum(combsLib[currentIndex], combsLib[i]) > 0)//ȡ��ϲ����ٵ�
				{
					currentGain = combsLib[i].gain;
					currentIndex = i;
				}
			}
		}
	}

	if (currentIndex==-1)
		return combsLib[0];

	return combsLib[currentIndex];
}

/**
*	�Ƚ�������ϵ�������
*	@return ֵΪ1��cb1���������ֵࣻΪ-1��cb2���������ֵࣻΪ0�������������
*/
int DDZCombFactory::compareSingleNum(Comb cb1, Comb cb2)
{
	return cb1.singleNum > cb2.singleNum ? 1:
			(cb1.singleNum < cb2.singleNum ? -1 : 0);
}

/**
*	�Ƚ�������ϵ�������
*	@return ֵΪ1��cb1��ϲ����ֵࣻΪ-1��cb2��ϲ����ֵࣻΪ0����ϲ������
*/
int DDZCombFactory::compareMovesNum(Comb cb1, Comb cb2)
{
	int n1 = cb1.moves.size();
	int n2 = cb2.moves.size();

	return n1 > n2 ? 1 : 
			(n1 < n2 ? -1 : 0);
}
/**
*	�Ƚ������������
*	@return ֵΪ1��cb1����ֵࣻΪ-1��cb2����ֵࣻΪ0���������
*/
int DDZCombFactory::compareGain(Comb cb1, Comb cb2)
{
	int gain1 = cb1.gain;
	int gain2 = cb2.gain;

	return compare(gain1,gain2);
}

int DDZCombFactory::compare(int n1, int n2)
{
	return n1 > n2 ? 1 :
		(n1 < n2 ? -1 : 0);
}
void DDZCombFactory::setGain(Comb *cb)
{
	// ���水ը����������㣬������������Է�ֹ����ı�
	int currentGain = 0;
	for (size_t i = 0; i < cb->moves.size(); i++)
	{
		
		if (cb->moves.at(i).cardsType == ROCKET || 
				cb->moves.at(i).cardsType == ZHADAN)
		{
			currentGain++;
		}
	}

	cb->gain = currentGain;
}

/**
*	 ������е���������2�����ô˺���
*    ������Ͽ��еڶ�С��������������±�
*/
int DDZCombFactory::getIndexOfSecondMinSingle(CombsLib combsLib)
{
	int secondMinSingleValue = -1;//�ڶ��ŵ��Ƶ�ֵ
	int secondMinSingleIndex = -1;//�ڶ��ŵ��Ƶ�
	for (size_t i = 0; i < combsLib.size(); i++)
	{
		Comb currentComb = combsLib.at(i);
		int currentSingleNum = currentComb.singleNum;//��ǰ��ϵ�������
		if (currentSingleNum>1)//���뵥����������1������������ 
		{
			int secondSingleIndex = currentComb.moves.size() - currentSingleNum + 2 - 1;//�ڶ�����С�����±�
			int secondSingleValue = currentComb.moves.at(secondSingleIndex).cards[0];//��ǰ�ڶ���С����ֵ

			if (secondSingleValue > secondMinSingleValue)
			{
				secondMinSingleValue = secondSingleValue;
				secondMinSingleIndex = i;
			}
			else if (secondSingleValue == secondMinSingleValue)
			{
				if (currentComb.moves.size() < combsLib.at(secondMinSingleIndex).moves.size())//����ȣ�ȡ��ϲ����ٵ����
				{
					secondMinSingleIndex = i;
				}
			}
		}
		
	}

	return secondMinSingleIndex;
}
/**
*��������
*/
void DDZCombFactory::setSingleNum(Comb *s_comb)
{
	int singleMoves[20];
	int singleCount = 0;

	vector<int> coupleMoves = vector<int>();//�洢��������ֵ
	
	for (int i = s_comb->moves.size() - 1; i >= 0; i--)
	{
		if (s_comb->moves.at(i).cardsType == SINGLE)
		{		
			singleMoves[singleCount] = s_comb->moves.at(i).cards[0];
			singleCount++;
			//removeMove(&s_comb->moves, i);//�Ƚ������߲�ȫ���Ƴ��������ƺ��ٽ�ʣ������ӽ�ȥ
		}
		else if (s_comb->moves.at(i).cardsType == COUPLE)
		{
			coupleMoves.push_back(s_comb->moves.at(i).cards[0]);
			removeMove(&s_comb->moves, i);
		}
	}
	quickSort(singleMoves, 0, singleCount - 1, singleCount);
	if (coupleMoves.size() > 0)
	{
		quickSort(&coupleMoves[0], 0, coupleMoves.size() - 1, coupleMoves.size());
	}

	
	for (int i = coupleMoves.size()-1; i >=0 && !coupleMoves.empty(); i--)
	{
		int distance = 1;
		int j=i-1;
		while (j >=0 && (coupleMoves[i] - coupleMoves[j] == distance))
		{
			distance++;
			j--;
		}
		
		if (distance >= 3 && coupleMoves[j+1]<=7)//˫˳���С��J���˫˳
		{
			CARDSMOVE move = CARDSMOVE();
			for (int k = j+1; k <= i; k++)
			{
				move.cards.push_back(coupleMoves[k]);
				move.cards.push_back(coupleMoves[k]);
			}
			move.cardsType = DUALJUNKO;
			s_comb->moves.push_back(move);
			vector<int>::iterator it = coupleMoves.begin();
			coupleMoves.erase(it+(j+1), it+(i+1));
		}
		i = j+1;
	}
	int singleCarryCardsNum = 0;
	int coupleCarryCardsNum = 0;
	//for (size_t i = 0; i < s_comb->moves.size(); i++)//��ʱ������
	//{
	//	int cardsType = s_comb->moves.at(i).cardsType;
	//	if (cardsType == SANTIAO)
	//	{
	//		if (singleCarryCardsNum < singleCount)
	//		{
	//			s_comb->moves.at(i).cards.push_back(singleMoves[singleCarryCardsNum++]);
	//			s_comb->moves.at(i).cardsType = THREE_ONE;
	//		}
	//		else if (coupleCarryCardsNum < coupleMoves.size())
	//		{
	//			s_comb->moves.at(i).cards.push_back(coupleMoves[coupleCarryCardsNum]);
	//			s_comb->moves.at(i).cards.push_back(coupleMoves[coupleCarryCardsNum]);
	//			s_comb->moves.at(i).cardsType = THREE_TWO;
	//			coupleCarryCardsNum++;
	//		}
	//		
	//	}
	//	else if (cardsType == THREEJUNKO)
	//	{
	//		int shouldCarry = s_comb->moves.at(i).cards.size() / 3;
	//		if (singleCarryCardsNum + shouldCarry < singleCount)
	//		{
	//			for (int k = 0; k < shouldCarry; k++)
	//			{
	//				s_comb->moves.at(i).cards.push_back(singleMoves[singleCarryCardsNum++]);
	//			}
	//			s_comb->moves.at(i).cardsType = THREEJUNKO_ONE;
	//		}
	//		else if (coupleCarryCardsNum + shouldCarry < coupleMoves.size())
	//		{
	//			for (int k = 0; k < shouldCarry; k++)
	//			{
	//				s_comb->moves.at(i).cards.push_back(coupleMoves[coupleCarryCardsNum]);
	//				s_comb->moves.at(i).cards.push_back(coupleMoves[coupleCarryCardsNum]);
	//				coupleCarryCardsNum++;
	//			}
	//			s_comb->moves.at(i).cardsType = THREEJUNKO_TWO;
	//		}		
	//	}
	//	/*else if (cardsType == ZHADAN)//�ݲ�����ը������
	//	{
	//		if (carryCardsNum + 2 < singleCount)
	//		{
	//			for (int k = 0; k < 2; k++)
	//			{
	//				comb->moves.at(i).cards.push_back(singleMoves[carryCardsNum++]);
	//			}
	//		}

	//		comb->moves.at(i).cardsType = FOUR_TWO;
	//	}*/
	//}

	int restOfSingle = singleCount;//ʣ�൥����
	s_comb->singleNum = restOfSingle;
	s_comb->coupleNum = coupleMoves.size();
	//if (restOfSingle > 0)
	//{
	//	for (int i = singleCarryCardsNum; i < singleCount; i++)
	//	{
	//		CARDSMOVE move = CARDSMOVE();
	//		move.cards.push_back(singleMoves[i]);
	//		move.cardsType = SINGLE;
	//		
	//		s_comb->moves.push_back(move);
	//	}
	//}

	for (int i = 0; i < coupleMoves.size(); i++)
	{
		CARDSMOVE move = CARDSMOVE();
		move.cards.push_back(coupleMoves[i]);
		move.cards.push_back(coupleMoves[i]);
		move.cardsType = COUPLE;

		s_comb->moves.push_back(move);
	}


}

/*
*	�������Ϊ������
*/
void DDZCombFactory::setCarryCards1(vector<CARDSMOVE> *moves)
{
	CARDSMOVE m = DDZMoveManager().getMinimumMove(*moves, SINGLE);
	for (int i = 0; i < moves->size(); i++)
	{
		if (moves->at(i).cardsType == SANTIAO && m.cardsType != PASS 
				&& moves->at(i).cards[0]!=m.cards[0])
		{
			VectorUtil::addElement(&moves->at(i).cards, m.cards[0],1);
			moves->at(i).cardsType = THREE_ONE;
		}
	}
}
/*
*	�������Ϊ����˫
*/
void DDZCombFactory::setCarryCards2(vector<CARDSMOVE> *moves)
{
	CARDSMOVE m = DDZMoveManager().getMinimumMove(*moves, COUPLE);
	for (int i = 0; i < moves->size(); i++)
	{
		if (moves->at(i).cardsType == SANTIAO && m.cardsType != PASS 
				&& moves->at(i).cards[0] != m.cards[0])
		{
			VectorUtil::addElement(&moves->at(i).cards, m.cards[0], 2);
			moves->at(i).cardsType = THREE_TWO;
		}
	}
}
void DDZCombFactory::setCarryCards3(vector<CARDSMOVE> *moves)
{
	vector<CARDSMOVE> singles = DDZMoveManager().getType3Single(*moves);
	for (int i = 0; i < moves->size(); i++)
	{
		if (moves->at(i).cardsType == THREEJUNKO && singles.size()>(moves->at(i).cards.size()/3))
		{
			int junkoNum = moves->at(i).cards.size() / 3;
			for (int j = 0; j < junkoNum; j++)
			{
				VectorUtil::addElement(&moves->at(i).cards, singles[j].cards[0], 1);
			}
			
			moves->at(i).cardsType = THREEJUNKO_ONE;
		}
	}
}
void DDZCombFactory::setCarryCards4(vector<CARDSMOVE> *moves)
{
	vector<CARDSMOVE> couples = DDZMoveManager().getType4Couple(*moves);
	for (int i = 0; i < moves->size(); i++)
	{
		if (moves->at(i).cardsType == THREEJUNKO && couples.size()>(moves->at(i).cards.size() / 3))
		{
			int junkoNum = moves->at(i).cards.size() / 3;
			for (int j = 0; j < junkoNum; j++)
			{
				VectorUtil::addElement(&moves->at(i).cards, couples[j].cards[0], 2);
			}

			moves->at(i).cardsType = THREEJUNKO_TWO;
		}
	}
}
void DDZCombFactory::quickSort(int *arr, int first, int last, int len)
{	
	int left = first;
	int right= last;

	if (first>last ||  (left<0 || last<0 || left>len || right>len))
		return;
	
	int key = arr[left];
	
	while (left < right)
	{
		while (left < right && key< arr[right])
		{
			right--;
		}

		arr[left] = arr[right];
	

		while (left<right && key>arr[left])
		{
			left++;
		}

		arr[right] = arr[left];
	}
	
	arr[right] = key;
	quickSort(arr, left + 1, last, len);
	quickSort(arr, first, right - 1, len);
}

/**
*	�Ƚ����������߲�����ֵ��С
*/
int DDZCombFactory::compareSingle(CARDSMOVE single1, CARDSMOVE single2)
{
	if (single1.cards[0]>single2.cards[0])
		return 1;
	else if (single1.cards[0]<single2.cards[0])
		return -1;
	else 
		return 0;
}

Comb DDZCombFactory::getCombByCardsType(CARDSMOVE key)
{
	Comb leastSingleComb = getComb1LeastSingle();
	DDZMoveManager ddz_MM = DDZMoveManager();
	vector<CARDSMOVE> singles = ddz_MM.getType3Single(leastSingleComb.moves);
	vector<CARDSMOVE> couples = ddz_MM.getType4Couple(leastSingleComb.moves);

	switch ((key.cardsType))
	{
		case ROCKET:
			break;

		case ZHADAN:
			 return getComb3MaxGain();
			break;

		case SINGLE:
			return leastSingleComb;
		
			break;

		case COUPLE:
			return leastSingleComb;
			break;

		case SANTIAO:
			return leastSingleComb;
			break;

		case THREE_ONE:
			break;
		default:
			break;
	}
}