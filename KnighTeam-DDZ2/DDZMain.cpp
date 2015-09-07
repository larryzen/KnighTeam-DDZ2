// KnighTeam-DDZ2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include "DDZPlayer.h"
#include "string.h"
#include "ThinkTable.h"
#include "Player.h"
#include "FileWriter.h"
#include "time.h"

using namespace std;


/**
*   @version   二打一斗地主博弈引擎.exe 1.0
*   创 建 人：  周超
*	创建日期：  2014年9月20日
*   版权所有：  重庆理工大学计算机博弈实验室
**/
int main()
{
	DDZPlayer player = DDZPlayer();
	CThinkTable think = CThinkTable();
	char cInMessage[200];   //输入通信内容
	char cOutMessage[200];  //输出通信内容
	char cShort[4];

	cin.getline(cInMessage,200);		//获取来自裁判系统的指令

	
	while(cInMessage[0]>='A')
	{
		/*FileWriter fw = FileWriter("order.txt");
		fw.writeOrderFromFatherExe(cInMessage);*/
		for(size_t i=0;i<3;i++)
		{
			cShort[i]=cInMessage[i];
		}
		cShort[3]='\0';

		if(strcmp(cShort,"DOU")==0)				//版本信息
		{
			player.CalName(cInMessage,cOutMessage);
			cout<< cOutMessage<<endl;
		}
		if(strcmp(cShort,"INF")==0)					//轮局信息
		{	
			if (!Player::cardsMoveRecords.empty())
			{
				string filePath = FileWriter::getSystemTime1();
				FileWriter fw = FileWriter(filePath);
				fw.WriteToFile(Player::cardsMoveRecords, CARDSMOVE());
			}
			

			player.initPlayer();
			player.CalInfo(cInMessage,cOutMessage);
			cout<< cOutMessage<<endl;
		}
		if(strcmp(cShort,"DEA")==0)					//牌套信息
		{
			player.CalDeal(cInMessage,cOutMessage);
			
			cout<< cOutMessage<<endl;
		}
		if(strcmp(cShort,"BID")==0)					//叫牌过程
		{
			player.CalBid(cInMessage,cOutMessage);
			
			cout<< cOutMessage<<endl;
		}
		if(strcmp(cShort,"LEF")==0)					//底牌信息
		{
			player.CalLeft(cInMessage,cOutMessage);
			cout << cOutMessage<< endl;
		}
		if(strcmp(cShort,"PLA")==0)					//出牌过程
		{
			player.CalPla(cInMessage,cOutMessage);
			cout<< cOutMessage<<endl;
		}
		if(strcmp(cShort,"ERR")==0)
		{
			player.CalErr(cInMessage,cOutMessage);
			cout<< cOutMessage<<endl;
		}
		if(strcmp(cShort,"GAM")==0)					//胜负信息
		{
			player.CalGam(cInMessage,cOutMessage);
			cout<< cOutMessage<<endl;
		}

		cin.getline(cInMessage,200);

	}
	
	return 0;
}