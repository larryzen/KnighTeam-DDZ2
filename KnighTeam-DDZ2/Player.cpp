#include "Player.h"


Player::Player(void)
{
}


Player::~Player(void)
{
}

int Player::p1_pos=-1;

bool Player::p1_IsLandlord=false;

int Player::p1_cardsNum=17;

int Player::p1_bid=0;

int Player::p1_front=-1;

int Player::p1_next=-1;

vector<unsigned> Player::p1_cardsList;

vector<CARDSMOVE> Player::p1_yiChu;

vector<CARDSMOVE> Player::p1_general;

double Player::p1_Membership[54]={0};

unsigned Player::p1_EachCardNum[TOTAL_KINDS] = { 0 };

int Player::p1_manyCards[14]={0};

int Player::p1_fewCards[14]={0};




int Player::p2_pos=-1;

bool Player::p2_IsLandlord=false;

int Player::p2_cardsNum=17;

int Player::p2_bid=0;

int Player::p2_front=-1;

int Player::p2_next=-1;

vector<unsigned> Player::p2_cardsList;

vector<CARDSMOVE> Player::p2_yiChu;

vector<CARDSMOVE> Player::p2_general;

double Player::p2_Membership[54]={0};

unsigned Player::p2_EachCardNum[TOTAL_KINDS] = { 0 };

int Player::p2_manyCards[14]={0};

int Player::p2_fewCards[14]={0};


int Player::p3_pos=-1;

bool Player::p3_IsLandlord=false;

int Player::p3_cardsNum=17;

int Player::p3_bid=0;

int Player::p3_front=-1;

int Player::p3_next=-1;

vector<unsigned> Player::p3_cardsList;

vector<CARDSMOVE> Player::p3_yiChu;

unsigned Player::p3_EachCardNum[TOTAL_KINDS] = { 0 };

bool Player::p3_canPlayOver = false;

vector<CARDSMOVE> Player::p3_comb;



vector<CARDSMOVE> Player::p3_general;

unsigned Player::remaining[TOTAL_KINDS] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 1 };// ������

vector<CARDSMOVE> Player::p3_allMoves;


int Player::p3_IsExistCardsType[14]={0};

int Player::firstPlayer=-1;

CARDSMOVE Player::firstMove;

int Player::lastPlayer=-1;

CARDSMOVE Player::lastMove;

vector<CARDSMOVE> Player::cardsMoveRecords;

unsigned Player::p1_EachCardAndRemaining[TOTAL_KINDS];	//�洢��ȥp2����һ�����ڵ��ƻ�ʣ�����

unsigned Player::p2_EachCardAndRemaining[TOTAL_KINDS];	//�洢��ȥp1����һ�����ڵ��ƻ�ʣ�����

unsigned Player::OnHandCardsTypeNum[15] ={ 0 };