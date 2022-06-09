#pragma once
#include "ConsoleModule.h"

enum EVENT : uint8_t
{
    RETURN, DSPLYNSEXP, DSPLYSEXP, SRCHRECEIVER, SRCHSENDTM
};

class SenderModule :
    public ConsoleModule
{
public:
    SenderModule() {}

    virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx);
};

