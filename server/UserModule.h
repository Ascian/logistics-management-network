#pragma once

#include "ConsoleModule.h"
#include"SenderModule.h"
#include"ReceiverModule.h"

enum EVENT : uint8_t
{
    RETURN, CHECK, PASSWORD, NAME, PHONE, ADDRESS, RECHARGE, SEND, SIGNFOR, 
    SENDINFRM, RECEIVEINFRM, FINDEXP
};

class UserModule :
    public ConsoleModule
{
public:
    UserModule() {}

    virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx);
};

