// KnighTeam-DDZ2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "iostream"
#include "DDZPlayer.h"
#include "string.h"
#include "ThinkTable.h"
#include "Player.h"
using namespace std;


/**
*   @version   ����һ��������������.exe 1.0
*   �� �� �ˣ�  �ܳ�
*	�������ڣ�  2014��9��20��
*   ��Ȩ���У�  ��������ѧ���������ʵ����
**/
int main()
{
	char D_Ver[200];    //Э��汾

	char cInMessage[200];   //����ͨ������
	char cOutMessage[200];  //���ͨ������
	char cShort[4];
	DDZPlayer player;
	CThinkTable think;
	Player p;
	
	cin.getline(cInMessage,200);		//��ȡ���Բ���ϵͳ��ָ��

	
	while(cInMessage[0]>='A')
	{
		for(size_t i=0;i<3;i++)
		{
			cShort[i]=cInMessage[i];
		}
		cShort[3]='\0';

		if(strcmp(cShort,"DOU")==0)				//�汾��Ϣ
		{
			player.CalName(cInMessage,D_Ver,cOutMessage);
			cout<<cOutMessage<<endl;
		}
		if(strcmp(cShort,"INF")==0)					//�־���Ϣ
		{
			player.CalInfo(cInMessage,cOutMessage);
			cout<<cOutMessage<<endl;
		}
		if(strcmp(cShort,"DEA")==0)					//������Ϣ
		{
			player.CalDeal(cInMessage,cOutMessage,p);
			player.initCardsInfo(p);
			cout<<cOutMessage<<endl;
		}
		if(strcmp(cShort,"BID")==0)					//���ƹ���
		{
			player.CalBid(cInMessage,cOutMessage,p);
			
			cout<<cOutMessage<<endl;
		}
		if(strcmp(cShort,"LEF")==0)					//������Ϣ
		{
			player.CalLeft(cInMessage,cOutMessage,p);
			cout<<cOutMessage<<endl;
		}
		if(strcmp(cShort,"PLA")==0)					//���ƹ���
		{
			player.CalPla(cInMessage,cOutMessage,p);
			cout<<cOutMessage<<endl;
		}
		if(strcmp(cShort,"ERR")==0)
		{
			player.CalErr(cInMessage,cOutMessage);
			cout<<cOutMessage<<endl;
		}
		if(strcmp(cShort,"GAM")==0)					//ʤ����Ϣ
		{
			player.initPlayer(p);
			player.CalGam(cInMessage,cOutMessage);
			cout<<cOutMessage<<endl;
		}

		cin.getline(cInMessage,200);

	}
	
	return 0;
}