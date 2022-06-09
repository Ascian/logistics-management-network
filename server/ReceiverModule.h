#pragma once
#include "ConsoleModule.h"

enum EVENT : uint8_t
{
    RETURN, DSPLYNPEXP, DSPLYNREXP, DSPLYREXP, SRCHSENDER, SRCHRECEIVETM
};

class ReceiverModule :
    public ConsoleModule
{
public:
    ReceiverModule() {}

    virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx);
};

