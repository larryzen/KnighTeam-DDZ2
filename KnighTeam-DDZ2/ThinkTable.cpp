#include "ThinkTable.h"
#include "define.h"
#include "vector"



CThinkTable::CThinkTable()
{
}


CThinkTable::~CThinkTable()
{
}

/**���ƣ����ط��� */
int CThinkTable::getBid(Player p)
{
	int bid=0;
	int cardsPoints=0;// �Ƶ�
	getMyCardsInfo(p);
	if(p.p3_cardsInfo.n.big==1)
	{
		cardsPoints+=6;  // �����Ƶ�+6
	}
	if(p.p3_cardsInfo.n.small==1)
	{
		cardsPoints+=4;  // С���Ƶ�+4
	}
	if(p.p3_cardsInfo.m.num!=0)
	{
		cardsPoints+=(2*p.p3_cardsInfo.m.num); // �� 2 �Ƶ�+2*����
	}

	if(p.p3_cardsInfo.a.num!=0) // ��3
	{
		if(p.p3_cardsInfo.a.num==1)
			cardsPoints-=2;
		if(p.p3_cardsInfo.a.num==2)
			cardsPoints-=1;
		if(p.p3_cardsInfo.a.num==4)
			cardsPoints+=6;
	}
	if(p.p3_cardsInfo.b.num!=0)  // ��4
	{
		if(p.p3_cardsInfo.b.num==1)
			cardsPoints-=2;
		if(p.p3_cardsInfo.b.num==2)
			cardsPoints-=1;
		if(p.p3_cardsInfo.b.num==4)
			cardsPoints+=6;
	}
	if(p.p3_cardsInfo.c.num!=0)  // ��5
	{
		if(p.p3_cardsInfo.c.num==1)
			cardsPoints-=2;
		if(p.p3_cardsInfo.c.num==2)
			cardsPoints-=1;
		if(p.p3_cardsInfo.c.num==4)
			cardsPoints+=6;
	}

	if(p.p3_cardsInfo.d.num!=0)  // ��6
	{
		if(p.p3_cardsInfo.d.num==1)
			cardsPoints-=1;
		if(p.p3_cardsInfo.d.num==4)
			cardsPoints+=6;
	}
	if(p.p3_cardsInfo.e.num!=0)  // ��7
	{
		if(p.p3_cardsInfo.e.num==1)
			cardsPoints-=1;
		if(p.p3_cardsInfo.e.num==4)
			cardsPoints+=8;// ������7��ը�������������С˳���ƣ����ҷ��Ʋ�㶼���Խ���
	}
	if(p.p3_cardsInfo.f.num!=0)  // ��8
	{
		if(p.p3_cardsInfo.f.num==1)
			cardsPoints-=1;
		if(p.p3_cardsInfo.f.num==4)
			cardsPoints+=6;
	}
	if(p.p3_cardsInfo.g.num!=0)  // ��9
	{
		if(p.p3_cardsInfo.g.num==1)
			cardsPoints-=1;
		if(p.p3_cardsInfo.g.num==4)
			cardsPoints+=6;
	}
	if(p.p3_cardsInfo.h.num!=0)  // ��10
	{
		if(p.p3_cardsInfo.h.num==4)
			cardsPoints+=6;
	}
	if(p.p3_cardsInfo.i.num!=0)
	{
		if(p.p3_cardsInfo.i.num==3)
			cardsPoints+=1;
		if(p.p3_cardsInfo.i.num==4)  // ��J
			cardsPoints+=6;
	}
	if(p.p3_cardsInfo.j.num!=0)
	{
		if(p.p3_cardsInfo.j.num==3)
				cardsPoints+=1;
		if(p.p3_cardsInfo.j.num==4)  // ��Q
				cardsPoints+=6;
	}
	if(p.p3_cardsInfo.k.num!=0)
	{
		if(p.p3_cardsInfo.k.num==3)
				cardsPoints+=1;
		if(p.p3_cardsInfo.k.num==4)  // ��K
				cardsPoints+=6;
	}
	if(p.p3_cardsInfo.l.num!=0)
	{
		if(p.p3_cardsInfo.l.num==3)
			cardsPoints+=1;
		if(p.p3_cardsInfo.l.num==4)  // ��A
			cardsPoints+=6;
	}
		

	if(cardsPoints>=8)
		bid+=3;                  // ��3��
	else if(cardsPoints>=4)
		bid+=2;                  // ��2��
	else if(cardsPoints>=2)
		bid+=1;                  // ��1��
	return bid;
}

/**��ʼ�����ͣ�ת��Ϊ���ױ�ʶ������ */
void CThinkTable::initCardsInfo(Player p)
{
	// ��ʼ��3-->����������Ϊ��
	p.p3_cardsInfo.a.num=0;
	p.p3_cardsInfo.b.num=0;
	p.p3_cardsInfo.c.num=0;
	p.p3_cardsInfo.d.num=0;
	p.p3_cardsInfo.e.num=0;
	p.p3_cardsInfo.f.num=0;
	p.p3_cardsInfo.g.num=0;
	p.p3_cardsInfo.h.num=0;
	p.p3_cardsInfo.i.num=0;
	p.p3_cardsInfo.j.num=0;
	p.p3_cardsInfo.k.num=0;
	p.p3_cardsInfo.l.num=0;
	p.p3_cardsInfo.m.num=0;
	p.p3_cardsInfo.n.num=0;

	// ��ʼ��3-->����������ֵ3->16
	p.p3_cardsInfo.a.value=3;
	p.p3_cardsInfo.b.value=4;
	p.p3_cardsInfo.c.value=5;
	p.p3_cardsInfo.d.value=6;
	p.p3_cardsInfo.e.value=7;
	p.p3_cardsInfo.f.value=8;
	p.p3_cardsInfo.g.value=9;
	p.p3_cardsInfo.h.value=10;
	p.p3_cardsInfo.i.value=11;
	p.p3_cardsInfo.j.value=12;
	p.p3_cardsInfo.k.value=13;
	p.p3_cardsInfo.l.value=14;
	p.p3_cardsInfo.m.value=15;
	p.p3_cardsInfo.n.value=16;

	// ��ʼ��3->A��2÷��Ϊ��
	p.p3_cardsInfo.a.meihua=0;
	p.p3_cardsInfo.b.meihua=0;
	p.p3_cardsInfo.c.meihua=0;
	p.p3_cardsInfo.d.meihua=0;
	p.p3_cardsInfo.e.meihua=0;
	p.p3_cardsInfo.f.meihua=0;
	p.p3_cardsInfo.g.meihua=0;
	p.p3_cardsInfo.h.meihua=0;
	p.p3_cardsInfo.i.meihua=0;
	p.p3_cardsInfo.j.meihua=0;
	p.p3_cardsInfo.k.meihua=0;
	p.p3_cardsInfo.l.meihua=0;
	p.p3_cardsInfo.m.meihua=0;

	// ��ʼ��3->A��2����Ϊ��
	p.p3_cardsInfo.a.fangkuai=0;
	p.p3_cardsInfo.b.fangkuai=0;
	p.p3_cardsInfo.c.fangkuai=0;
	p.p3_cardsInfo.d.fangkuai=0;
	p.p3_cardsInfo.e.fangkuai=0;
	p.p3_cardsInfo.f.fangkuai=0;
	p.p3_cardsInfo.g.fangkuai=0;
	p.p3_cardsInfo.h.fangkuai=0;
	p.p3_cardsInfo.i.fangkuai=0;
	p.p3_cardsInfo.j.fangkuai=0;
	p.p3_cardsInfo.k.fangkuai=0;
	p.p3_cardsInfo.l.fangkuai=0;
	p.p3_cardsInfo.m.fangkuai=0;

	// ��ʼ��3->A��2����Ϊ��
	p.p3_cardsInfo.a.hongtao=0;
	p.p3_cardsInfo.b.hongtao=0;
	p.p3_cardsInfo.c.hongtao=0;
	p.p3_cardsInfo.d.hongtao=0;
	p.p3_cardsInfo.e.hongtao=0;
	p.p3_cardsInfo.f.hongtao=0;
	p.p3_cardsInfo.g.hongtao=0;
	p.p3_cardsInfo.h.hongtao=0;
	p.p3_cardsInfo.i.hongtao=0;
	p.p3_cardsInfo.j.hongtao=0;
	p.p3_cardsInfo.k.hongtao=0;
	p.p3_cardsInfo.l.hongtao=0;
	p.p3_cardsInfo.m.hongtao=0;

	// ��ʼ��3->A��2����Ϊ��
	p.p3_cardsInfo.a.heitao=0;
	p.p3_cardsInfo.b.heitao=0;
	p.p3_cardsInfo.c.heitao=0;
	p.p3_cardsInfo.d.heitao=0;
	p.p3_cardsInfo.e.heitao=0;
	p.p3_cardsInfo.f.heitao=0;
	p.p3_cardsInfo.g.heitao=0;
	p.p3_cardsInfo.h.heitao=0;
	p.p3_cardsInfo.i.heitao=0;
	p.p3_cardsInfo.j.heitao=0;
	p.p3_cardsInfo.k.heitao=0;
	p.p3_cardsInfo.l.heitao=0;
	p.p3_cardsInfo.m.heitao=0;

	// ��ʼ����С��
	p.p3_cardsInfo.n.small=0;
	p.p3_cardsInfo.n.big=0;

}

/** ͳ���ҷ�����3~K��A��2��С��������15����������*/
void CThinkTable::initEachCardNum(Player p)
{
	for(size_t i=0;i<15;i++)
	{
		p.p3_EachCardNum[i]=0;// ��ʼ������������
	}

	VECTORINT cardsList = p.p3_cardsList;

	for(size_t i=0;i<cardsList.size();i++)
	{
		if(cardsList.at(i)==52)
		{
			p.p3_EachCardNum[13]++;
		}
		else if(cardsList.at(i)==53)
		{
			p.p3_EachCardNum[14]++;
		}
		else
		{
			p.p3_EachCardNum[cardsList.at(i)/4]++; // ͳ���ҷ������Ƹ�����
		}
	}
}


/** ����ҷ����� �������� �� ��ɫ���� */
void CThinkTable::getMyCardsInfo(Player p)
{
	initCardsInfo(p);
	for(size_t i=0;i<p.p3_cardsList.size();i++)
	{
		int tmp=p.p3_cardsList.at(i);
		
		if(tmp>=0&&tmp<=3)  // �����3 �����͸���ɫ����
		{
			if(tmp==0)
			{
				p.p3_cardsInfo.a.meihua=1;
			}
			else if(tmp==1)
			{
				p.p3_cardsInfo.a.fangkuai=1;
			}
			else if(tmp==2)
			{
				p.p3_cardsInfo.a.hongtao=1;
			}
			else
			{
				p.p3_cardsInfo.a.heitao=1;
			}
			p.p3_cardsInfo.a.num++;
		}
		else if(tmp>=4&&tmp<=7)  // �����4 �����͸���ɫ����
		{
			if(tmp==4)
			{
				p.p3_cardsInfo.b.meihua=1;
			}
			else if(tmp==5)
			{
				p.p3_cardsInfo.b.fangkuai=1;
			}
			else if(tmp==6)
			{
				p.p3_cardsInfo.b.hongtao=1;
			}
			else
			{
				p.p3_cardsInfo.b.heitao=1;
			}
			p.p3_cardsInfo.b.num++;
		}
		else if(tmp>=8&&tmp<=11)  // �����5 �����͸���ɫ����
		{
			if(tmp==8)
			{
				p.p3_cardsInfo.c.meihua=1;
			}
			else if(tmp==9)
			{
				p.p3_cardsInfo.c.fangkuai=1;
			}
			else if(tmp==10)
			{
				p.p3_cardsInfo.c.hongtao=1;
			}
			else
			{
				p.p3_cardsInfo.c.heitao=1;
			}
			p.p3_cardsInfo.c.num++;
		}
		else if(tmp>=12&&tmp<=15)  // �����6 �����͸���ɫ����
		{
			if(tmp==12)
			{
				p.p3_cardsInfo.d.meihua=1;
			}
			else if(tmp==13)
			{
				p.p3_cardsInfo.d.fangkuai=1;
			}
			else if(tmp==14)
			{
				p.p3_cardsInfo.d.hongtao=1;
			}
			else
			{
				p.p3_cardsInfo.d.heitao=1;
			}
			p.p3_cardsInfo.d.num++;
		}
		else if(tmp>=16&&tmp<=19)  // �����7 �����͸���ɫ����
		{
			if(tmp==16)
			{
				p.p3_cardsInfo.e.meihua=1;
			}
			else if(tmp==17)
			{
				p.p3_cardsInfo.e.fangkuai=1;
			}
			else if(tmp==18)
			{
				p.p3_cardsInfo.e.hongtao=1;
			}
			else
			{
				p.p3_cardsInfo.e.heitao=1;
			}
			p.p3_cardsInfo.e.num++;
		}
		else if(tmp>=20&&tmp<=23)  // �����8 �����͸���ɫ����
		{
			if(tmp==20)
			{
				p.p3_cardsInfo.f.meihua=1;
			}
			else if(tmp==21)
			{
				p.p3_cardsInfo.f.fangkuai=1;
			}
			else if(tmp==22)
			{
				p.p3_cardsInfo.f.hongtao=1;
			}
			else
			{
				p.p3_cardsInfo.f.heitao=1;
			}
			p.p3_cardsInfo.f.num++;
		}
		else if(tmp>=24&&tmp<=27)  // �����9 �����͸���ɫ����
		{
			if(tmp==24)
			{
				p.p3_cardsInfo.g.meihua=1;
			}
			else if(tmp==25)
			{
				p.p3_cardsInfo.g.fangkuai=1;
			}
			else if(tmp==26)
			{
				p.p3_cardsInfo.g.hongtao=1;
			}
			else
			{
				p.p3_cardsInfo.g.heitao=1;
			}
			p.p3_cardsInfo.g.num++;
		}
		else if(tmp>=28&&tmp<=31)  // �����10 �����͸���ɫ����
		{
			if(tmp==28)
			{
				p.p3_cardsInfo.h.meihua=1;
			}
			else if(tmp==29)
			{
				p.p3_cardsInfo.h.fangkuai=1;
			}
			else if(tmp==30)
			{
				p.p3_cardsInfo.h.hongtao=1;
			}
			else
			{
				p.p3_cardsInfo.h.heitao=1;
			}
			p.p3_cardsInfo.h.num++;
		}
		else if(tmp>=32&&tmp<=35)  // �����J �����͸���ɫ����
		{
			if(tmp==32)
			{
				p.p3_cardsInfo.i.meihua=1;
			}
			else if(tmp==33)
			{
				p.p3_cardsInfo.i.fangkuai=1;
			}
			else if(tmp==34)
			{
				p.p3_cardsInfo.i.hongtao=1;
			}
			else
			{
				p.p3_cardsInfo.i.heitao=1;
			}
			p.p3_cardsInfo.i.num++;
		}
		else if(tmp>=36&&tmp<=39)  // �����Q �����͸���ɫ����
		{
			if(tmp==36)
			{
				p.p3_cardsInfo.j.meihua=1;
			}
			else if(tmp==37)
			{
				p.p3_cardsInfo.j.fangkuai=1;
			}
			else if(tmp==38)
			{
				p.p3_cardsInfo.j.hongtao=1;
			}
			else
			{
				p.p3_cardsInfo.j.heitao=1;
			}
			p.p3_cardsInfo.j.num++;
		}
		else if(tmp>=40&&tmp<=43)  // �����K �����͸���ɫ����
		{
			if(tmp==40)
			{
				p.p3_cardsInfo.k.meihua=1;
			}
			else if(tmp==41)
			{
				p.p3_cardsInfo.k.fangkuai=1;
			}
			else if(tmp==42)
			{
				p.p3_cardsInfo.k.hongtao=1;
			}
			else
			{
				p.p3_cardsInfo.k.heitao=1;
			}
			p.p3_cardsInfo.k.num++;
		}
		else if(tmp>=44&&tmp<=47)  // �����A �����͸���ɫ����
		{
			if(tmp==44)
			{
				p.p3_cardsInfo.l.meihua=1;
			}
			else if(tmp==45)
			{
				p.p3_cardsInfo.l.fangkuai=1;
			}
			else if(tmp==46)
			{
				p.p3_cardsInfo.l.hongtao=1;
			}
			else
			{
				p.p3_cardsInfo.l.heitao=1;
			}
			p.p3_cardsInfo.l.num++;
		}
		else if(tmp>=48&&tmp<=51)  // �����2 �����͸���ɫ����
		{
			if(tmp==48)
			{
				p.p3_cardsInfo.m.meihua=1;
			}
			else if(tmp==49)
			{
				p.p3_cardsInfo.m.fangkuai=1;
			}
			else if(tmp==50)
			{
				p.p3_cardsInfo.m.hongtao=1;
			}
			else
			{
				p.p3_cardsInfo.m.heitao=1;
			}
			p.p3_cardsInfo.m.num++;
		}
		else  // ����ƴ�С������
		{
			if(tmp==52)
				p.p3_cardsInfo.n.small=1;
			else
				p.p3_cardsInfo.n.big=1;
			p.p3_cardsInfo.n.num++;
		}
	}
}

