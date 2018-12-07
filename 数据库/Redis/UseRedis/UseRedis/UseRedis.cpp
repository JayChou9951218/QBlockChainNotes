// UseRedis.cpp : �������̨Ӧ�ó������ڵ㡣
//
/**
*Date: 2018/12/07 22:26
*Author:yqq
*Descriptions:
	ʹ��redis  ģ��ʵ��  ��Ϸʵʱ���а�

ע��:
	LINK error 2038 ����
		1.ֱ�ӱ���redis-server����, ��������Ӧ�ľ�̬��,  ע�Ȿ����ʹ�õ��� release�澲̬�� ����  debug�澲̬��, 
		2.�����ʱ��ע������  ����>>C++>>��������>>���п� (Release��ΪMT,  Debug����Ϊ MdT)


	linux������:
		1.make  Ȼ�� make install
		2.�� ��̬��·������   ~/.profile��  Ȼ��  source ~/.profile  ʹ����Ч
		
		��/usr/local/lib ��ӵ�����������  , �༭  ~/.profile ������������
			export APTH=$PATH:/usr/local/lib
			export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
*/

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <memory>
#include <string>
#include <vector>
#include <windows.h>
#include "use_redis.h"

using namespace std;

Redis *gRh = NULL;
int testRedis();
int testOther();

DWORD WINAPI ShowRank(LPVOID lpThreadParameter);
BOOL bOver = FALSE;
const int PLAYER_COUNTS = 1000;
const int GAME_ROUNDS = 100;
void MyGame()
{
	/*
	1.����100,000����Ϸ���
	2.���ÿ��(ÿ�����������)  ==> ÿ�ֵķ���
	3.����һ����ִ�г���  ÿ�� 1s, ˢ��һ�η������а�

	˼·:  ʹ�� redis��   sorted set ����
	zrevrange 
	*/

	redisContext * pConn = redisConnect("127.0.0.1", 6379);
	if (NULL == pConn)
	{
		printf("redisConnect err\n");
		return ;
	}


	//1.�������
	redisReply *pRet = NULL;
	for (int i = 0; i < PLAYER_COUNTS; i++)
	{
		pRet = (redisReply *)redisCommand(pConn, "zadd players %d player%d", 0, i);
		if (NULL == pRet)
		{
			printf("�������ʧ��\n");
			return;
		}
	}

#ifdef MT
	HANDLE hThread = ::CreateThread(NULL, 0, ShowRank, NULL, 0, NULL);
	if (NULL == hThread)
	{
		printf("�������߳�ʧ��\n");
		return;
	}
#endif

	//2.��10000��
	srand((int)time(NULL));
	for (int i = 0; i < GAME_ROUNDS; i++)
	{
		for (int iPlayer = 0; iPlayer < PLAYER_COUNTS; iPlayer++)
		{
			int iScore = rand() % 100;
			pRet = (redisReply *)redisCommand(pConn, "zscore players player%d", iPlayer);
			if (NULL == pRet)
			{
				printf("��ȡ��ʷ����ʧ��\n");
				return;
			}
			int iOldScore = atoi(pRet->str);
			pRet = (redisReply *)redisCommand(pConn, "zadd players %d player%d", iOldScore + iScore, iPlayer);
			if (NULL == pRet)
			{
				printf("������ҷ���ʧ��\n");
				return;
			}
			
		}
	}

	bOver = TRUE;

#ifdef MT
	//�ȴ����߳̽���
	WaitForSingleObject(hThread, INFINITE);
#endif
}



//ʹ���̷߳�ʽ,  ���ǻ�ȡ����ǰ��, ��֪��ʲôԭ��, �������о�
#ifdef MT
DWORD WINAPI ShowRank(LPVOID lpvoid)
{

	redisContext * pConn = redisConnect("127.0.0.1", 6379);
	if (NULL == pConn)
	{
		printf("ShowRank() redisConnect err\n");
		return 0;
	}

	while (!bOver)
	{
		Sleep(1000);
		redisReply *pRet = (redisReply *)redisCommand(pConn, "zrevrange players 0 5 withscores");  //ǰ5
		if (NULL != pRet)
		{
			printf("��ȡǰ��ʧ��\n");
			continue;
			//return 0;
		}

		printf("=======================\n");
		for (int i = 0; i < pRet->elements && !bOver; i++)
		{
			std::cout << pRet->element[i]->str << std::endl;
		}
		printf("=======================\n");

	}
	printf("���߳������˳�");

	return 0;
}
#endif



int main()
{
	//testRedis();
	//testOther();

	MyGame();

	system("pause");
    return 0;
}






int testRedis()
{
	Redis *r = new Redis();
	if (!r->connect("127.0.0.1", 6379))
	{
		printf("connect error!\n");
		return 0;
	}
	r->set("name", "Andy");
	printf("Get the name is %s\n", r->get("name").c_str());

	gRh = r;
	delete r;
	return 0;
}


int testOther()
{
	redisContext * pConn = redisConnect("127.0.0.1", 6379);
	if (NULL == pConn)
	{
		printf("redisConnect err\n");
		return 0;
	}

	redisReply * pRet = (redisReply *)redisCommand(pConn, "sadd qvs 1 2 3 4 5");  //ִ��redis����
	if (NULL == pRet)
	{
		printf("redisCommand err\n");
		return 0;
	}

	pRet = (redisReply *)redisCommand(pConn, "smembers qvs");
	if (NULL == pRet)
	{
		printf("redisCommand err\n");
		return 0;
	}

	for (int i = 0; i < pRet->elements; i++)
	{
		//str>str << std::endl;
		std::cout << pRet->element[i]->str << std::endl;
	}
}
