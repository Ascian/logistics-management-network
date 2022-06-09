#include "SenderModule.h"

bool SenderModule::execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx)
{
    ostringstream outBuf;
    istringstream recvInf(recvBuf);
    char event;
    recvInf.get(event);
    switch (event) {
    case RETURN: {
        return true;
        break;
    }
    case DSPLYNSEXP: {
        int i = 0;
        bool moreInf;
        try {
            moreInf = pClient->pUser->notSExpToString(outBuf, i * 10, i * 10 + 9);
        }
        catch (const char msg) {
            send(pClient->cliSock, &msg, 1, 0);
            break;
        }

        char msg = SUCCESS;
        send(pClient->cliSock, &msg, 1, 0);

        i++;
        send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        msg = 1;
        while (moreInf) {
            send(pClient->cliSock, &msg, 1, 0);

            recv(pClient->cliSock, &msg, 1, 0);
            if (msg) {
                outBuf.clear();
                moreInf = pClient->pUser->notSExpToString(outBuf, i * 10, i * 10 + 9);
                i++;
                send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
            }
            else
                break;
        }
        msg = 0;
        send(pClient->cliSock, &msg, 1, 0);
        break;
    }

    case DSPLYSEXP: {
        int i = 0;
        bool moreInf;
        try {
            moreInf = pClient->pUser->sExpToString(outBuf, i * 10, i * 10 + 9);
        }
        catch (const char msg) {
            send(pClient->cliSock, &msg, 1, 0);
            break;
        }

        char msg = SUCCESS;
        send(pClient->cliSock, &msg, 1, 0);

        i++;
        send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        Sleep(1);
        msg = 1;
        while (moreInf) {
            send(pClient->cliSock, &msg, 1, 0);

            recv(pClient->cliSock, &msg, 1, 0);
            if (msg) {
                outBuf.clear();
                moreInf = pClient->pUser->sExpToString(outBuf, i * 10, i * 10 + 9);
                i++;
                send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
            }
            else
                break;
        }
        msg = 0;
        send(pClient->cliSock, &msg, 1, 0);
        break;
    }
    case SRCHRECEIVER: {
        string sender;
        recvInf >> sender;
        try {
            vector<const Express*> expresses = pClient->pUser->searchReceiver(sender);

            char msg = SUCCESS;
            send(pClient->cliSock, &msg, 1, 0);

            int i = 0, j = 0;
            for (j = i * 10; j < i * 10 + 10 && j < expresses.size(); j++) {
                outBuf << "[" << j << "]" << endl;
                outBuf << expresses.at(j) << endl;
            }
            send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
            i++;
             msg = 1;
            while (j < expresses.size()) {
                send(pClient->cliSock, &msg, 1, 0);

                recv(pClient->cliSock, &msg, 1, 0);
                if (msg) {
                    outBuf.clear();
                    for (j = i * 10; j < i * 10 + 10 && j < expresses.size(); j++) {
                        outBuf << "[" << j << "]" << endl;
                        outBuf << expresses.at(j) << endl;
                    }
                    i++;
                    send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
                }
                else
                    break;
            }
            msg = 0;
            send(pClient->cliSock, &msg, 1, 0);
        }
        catch (const char msg) {
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case SRCHSENDTM: {
        struct tm lowerBound;
        recvInf >> get_time(&lowerBound, "%Y-%m-%d %H:%M:%S");
        struct tm upperBound;
        recvInf >> get_time(&upperBound, "%Y-%m-%d %H:%M:%S");

        try {
            vector<const Express*> expresses = pClient->pUser->searchSendTime(mktime(&lowerBound), mktime(&upperBound));
            char msg = SUCCESS;
            send(pClient->cliSock, &msg, 1, 0);
            int i = 0, j = 0;
            for (j = i * 10; j < i * 10 + 10 && j < expresses.size(); j++) {
                outBuf << "[" << j << "]" << endl;
                outBuf << expresses.at(j) << endl;
            }
            send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
            i++;
            msg = 1;
            while (j < expresses.size()) {
                send(pClient->cliSock, &msg, 1, 0);

                recv(pClient->cliSock, &msg, 1, 0);
                if (msg) {
                    outBuf.clear();
                    for (j = i * 10; j < i * 10 + 10 && j < expresses.size(); j++) {
                        outBuf << "[" << j << "]" << endl;
                        outBuf << expresses.at(j) << endl;
                    }
                    i++;
                    send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
                }
                else
                    break;
            }
            msg = 0;
            send(pClient->cliSock, &msg, 1, 0);
        }
        catch (const char msg) {
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    default:
        return false;
    }
    return false;
}
