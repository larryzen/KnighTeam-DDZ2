#ifndef define_h_
#define define_h_
#include<vector>
#include<set>

using namespace std;

#define MAX_Depth 120     //���ؿ������
#define AlphaBeta_Depth 15 // ��-��ģ�����
#define UCT_Depth 50 // UCTģ�����
#define START_UCT 0
#define UCB_c 0.8

#define BestMoveLinkListLen 80//����߲�������

#define AlphaBetaTime 12

#define HalfGame 7 // �볡�ֽ���

#define RESTCARDS 2 // �з�ʣ������Ʊز�
#define PASS -1				// pass
#define INVALID 0 			//��Ч����
#define ROCKET 1			// ���
#define ZHADAN 2			// ը��
#define SINGLE 3			// ����
#define COUPLE 4			// ����
#define SANTIAO 5			// ����
#define THREE_ONE 6			// ������
#define THREE_TWO 7			// ����˫
#define SINGLEJUNKO 8		// ��˳
#define DUALJUNKO 9			// ˫˳
#define THREEJUNKO 10		// ��˳
#define THREEJUNKO_ONE 11   // ��˳����
#define THREEJUNKO_TWO 12   // ��˳��˫
#define FOUR_TWO 13         // �Ĵ���
#define FOUR_TWO_COUPLE 14  // �Ĵ�����

#define TOTAL_KINDS 15 //�ܹ�������

#define J 8  //�궨�� J
#define Q 9  //�궨�� Q
#define K 10 //�궨�� K
#define A 11 //�궨�� A
#define T 12 //�궨�� 2
#define X 13 //�궨�� С����XW)
#define D 14 //�궨�� ������DW)

#define GET_ARRAY_LEN(array,len) {len = (sizeof(array) / sizeof(array[0]));}//������鳤��

#define STATUS_MAX 1		//  ��ʤ��
#define STATUS_LARGE 2		//  ����
#define STATUS_MID 3		//  �е���
#define STATUS_SMALL 4		//  С��


#define FA_Single 4      //�׹������޶�����
#define FA_Couple 4      //�׹������޶�����
#define FA_Santiao 3     //�׹������޶�����

// ���ÿ��߲�
#define NULL_MOVE(move) \
{ \
	move.cards =vector<unsigned>();\
	move.cardsType= PASS;\
	move.score=0;\
}

#define max(x, y) {x = x > y ? x : y;}





typedef vector<unsigned> VECTORINT;

typedef struct _cardsmoves
{
	vector<unsigned> cards;
	int cardsType;
	int side;			//���Ʒ�
	int status;			// ״̬
	int outWay;			// ���Ʒ�ʽ
	int score;			//��ǰ����ֵ ��-��
	int win;			//ģ��Ӯ�ô��� ��-��
	int lose;			//ģ����Ĵ�����-��
	bool IsSplit;		//�Ƿ�Ϊ���Ʋ������߲�
	int current_times;  //��ǰ���ģ����� uct
	double UCBValue;    //��ǰ���UCBֵ uct
	int Count;			//��ģ���ܴ�����-��
}CARDSMOVE;//�߲�

typedef struct _comb
{
	vector<CARDSMOVE> moves;//һ���߲�
	int score;
	int singleNum;		//����е�������
	int coupleNum;		//����ж�������
	int santiaoNum;		//�������������
	int dualJunkoNum;	//�����˫˳������
	int gain;			//����
}Comb;

typedef vector<Comb> CombsLib;


#endif //define_h_