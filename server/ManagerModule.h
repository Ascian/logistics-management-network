#pragma once
#include "ConsoleModule.h"
#include "CourierModule.h"
#include "UserModule.h"

enum EVENT : uint8_t
{
    RETURN, CHECK, PASSWORD, NAME, ADDCOURIER, DELETECOURIER, ASSIGN, DSPLYUSER,
    DSPLYCOURIER, DSPLYEXP, SRCHCREATETM, FINDUSER, FINDCOURIER, FINDEXP
};

class ManagerModule :
    public ConsoleModule
{
public:
    ManagerModule() {
        subModules = { new UserModule(), new CourierModule()};
    }

    virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx);
};

