#pragma once
#include "ConsoleModule.h"
#include "CourierModule.h"
#include "UserModule.h"

class ManagerModule :
    public ConsoleModule
{
public:
    ManagerModule()
        :ConsoleModule("manager") {
        subModules = { new UserModule, new CourierModule() };
    }

    virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf);
};

