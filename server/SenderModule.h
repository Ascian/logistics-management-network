#pragma once
#include "ConsoleModule.h"
class SenderModule :
    public ConsoleModule
{
public:
    SenderModule()
        :ConsoleModule("sender") {
        commands = { "return", "dsplynsexp", "dsplysexp", "srchreceiver", 
            "srchsendtm", "help" };
    }

    virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, const set<int> ban);
};

