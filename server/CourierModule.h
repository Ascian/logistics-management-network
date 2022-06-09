#pragma once
#include "ConsoleModule.h"

enum EVENT : uint8_t
{
    RETURN, CHECK, PASSWORD, NAME, PHONE, PICKUP, DSPLYNPEXP, DSPLYNREXP,
    DESPLREXP, SRCHSENDER, SRCHRECEIVER, SRCHPICKTM, FINDEXP
};

class CourierModule :
    public ConsoleModule
{
public:
    CourierModule() {}

    virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx);
};

