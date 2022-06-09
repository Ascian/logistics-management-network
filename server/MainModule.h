#pragma once

#include"ConsoleModule.h"
#include"UserModule.h"
#include"ManagerModule.h"
#include"CourierModule.h"

enum COMMAND : uint8_t { EXIT, USER, COURIER, MANAGER, REGISTER };

class MainModule :
    public ConsoleModule
{
public:
    MainModule()  {
        subModules = { new UserModule(), new CourierModule(), new ManagerModule()};
    }

    virtual bool execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx);
};

