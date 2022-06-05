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
    CourierModule()
        :ConsoleModule("courier") {
        commands = { "return", "check", "password", "name", "phone", "pickup",
            "dsplynpexp", "dsplynrexp", "dsplyrexp", "srchsender", "srchreceiver",
            "srchpicktm", "findexp", "help" };
    }

    virtual bool execute(SOCKET& cliSock, const string& command, const set<int>& ban);
};

