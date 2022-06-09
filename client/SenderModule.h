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
    SenderModule()
        :ConsoleModule("sender") {
        commands = { "return", "dsplynsexp", "dsplysexp", "srchreceiver",
            "srchsendtm", "help" };
    }

    virtual bool execute(SOCKET& cliSock, const string& command, const set<int>& ban);
};

