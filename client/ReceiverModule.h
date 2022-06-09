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
    ReceiverModule()
        :ConsoleModule("receiver") {
        commands = { "return", "dsplynpexp", "dsplynrexp", "dsplyrexp",
            "srchsender", "srchreceivetm", "help" };
    }

    virtual bool execute(SOCKET& cliSock, const string& command, const set<int>& ban);
};

