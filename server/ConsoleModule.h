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
	vector<ConsoleModule*> subModules;//子模块
	int currentModule;//当前子模块
public:
	ConsoleModule() {
		currentModule = -1;
	}

	virtual ~ConsoleModule() {
		for (auto temp : subModules)
			delete(temp);
	}

	//分析命令
	virtual inline bool parse(Logistics* pLogistics, Client* pClient,const char* recvBuf) {
		if (currentModule != -1) {
			//交给子模块执行
			if (subModules.at(currentModule)->parse(pLogistics, pClient, recvBuf))
				currentModule = -1;
			return false;
		}
		return execute(pLogistics, pClient, recvBuf);
	}

	//执行命令
	virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf) = 0;

};

