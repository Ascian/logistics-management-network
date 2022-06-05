#pragma once
#include<string>
#include<vector>
#include<set>
#include<Winsock2.h>

#include"Logistics.h"

const int STRING_MAX_SIZE = 30;
const int MAX_BUFFER_SIZE = 2000;

using namespace std;

class Client {
public:
	SOCKET cliSock = INVALID_SOCKET;
	User* pUser = NULL;
	Courier* pCourier = NULL;
};

class ConsoleModule
{
protected:
	vector<ConsoleModule*> subModules;//��ģ��
	int currentModule;//��ǰ��ģ��
public:
	ConsoleModule() {
		currentModule = -1;
	}

	virtual ~ConsoleModule() {
		for (auto temp : subModules)
			delete(temp);
	}

	//��������
	virtual inline bool parse(Logistics* pLogistics, Client* pClient,const char* recvBuf) {
		if (currentModule != -1) {
			//������ģ��ִ��
			if (subModules.at(currentModule)->parse(pLogistics, pClient, recvBuf))
				currentModule = -1;
			return false;
		}
		return execute(pLogistics, pClient, recvBuf);
	}

	//ִ������
	virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf) = 0;

};

