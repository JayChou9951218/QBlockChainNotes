#define _CRT_SECURE_NO_WARNINGS  

/**
*Date: 2018/12/07 22:17
*Author:yqq
*Descriptions:	
	ÿ��һ��ˢ��һ����Ϸ��ҷ������а�

	ShowRank ��  UseRedis  �ֿ�����, ����������ִ���ļ�
*/


#include "use_redis.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

const char *IP = "192.168.10.119";
const int PORT = 6379;

//��ʾ������а�
void showRank()
{
	redisContext * pConn = redisConnect(IP, PORT);
	if (NULL == pConn)
	{
		printf("redisConnect err\n");
		return;
	}

	while (1)
	{
#ifdef _WIN32
		Sleep(1000); //����1��
#else //linux
		sleep(1); //����1��
#endif
		redisReply *pRet = (redisReply *)redisCommand(pConn, "zrevrange players 0 5 withscores"); //��ȡǰ��
		if (NULL == pRet)
		{
			printf("��ȡǰ��ʧ��\n");
			continue;
		}
		printf("======================\n");
		int i = 0;
		for (i = 0; i < pRet->elements; i++)
		{
			std::cout << pRet->element[i]->str << std::endl;
		}
		printf("======================\n");
	}
}



#if  0 //�ֿ�����, �����µĿ�ִ���ļ�, 
int main()
{
	showRank();
#if 0
	Redis *r = new Redis();
	if (!r->connect("192.168.10.119", 6379))
	{
		printf("connect error!\n");
		return 0;
	}
	r->set("name", "Andy");
	printf("Get the name is %s\n", r->get("name").c_str());
	delete r;
#endif
	return 0;
}
#endif
