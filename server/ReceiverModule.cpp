#include "ReceiverModule.h"

bool ReceiverModule::execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx)
{
    ostringstream outBuf;
    istringstream recvInf(recvBuf);
    char event;
    recvInf >> event;
    switch (event) {
    case RETURN: {
        return true;
        break;
    }
    case DSPLYNPEXP: {
        int i = 0;
        bool moreInf;
        try {
            moreInf = pClient->pUser->notPExpToString(outBuf, i * 10, i * 10 + 9);
        }
        catch (const char* msg) {
            send(pClient->cliSock, msg, 1, 0);
            break;
        }

        char msg = SUCCESS;
        send(pClient->cliSock, &msg, 1, 0);

        i++;
        send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        char msg = 1;
        while (moreInf) {
            send(pClient->cliSock, &msg, 1, 0);

            recv(pClient->cliSock, &msg, 1, 0);
            if (msg) {
                outBuf.clear();
                moreInf = pClient->pUser->notPExpToString(outBuf, i * 10, i * 10 + 9);
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

    case DSPLYNREXP: {
        int i = 0;
        bool moreInf;
        try {
            moreInf = pClient->pUser->notPExpToString(outBuf, i * 10, i * 10 + 9);
        }
        catch (const char* msg) {
            send(pClient->cliSock, msg, 1, 0);
            break;
        }

        char msg = SUCCESS;
        send(pClient->cliSock, &msg, 1, 0);

        i++;
        send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        char msg = 1;
        while (moreInf) {
            send(pClient->cliSock, &msg, 1, 0);

            recv(pClient->cliSock, &msg, 1, 0);
            if (msg) {
                outBuf.clear();
                moreInf = pClient->pUser->notRExpToString(outBuf, i * 10, i * 10 + 9);
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
    case DSPLYREXP: {
        int i = 0;
        bool moreInf;
        try {
            moreInf = pClient->pUser->notPExpToString(outBuf, i * 10, i * 10 + 9);
        }
        catch (const char* msg) {
            send(pClient->cliSock, msg, 1, 0);
            break;
        }

        char msg = SUCCESS;
        send(pClient->cliSock, &msg, 1, 0);

        i++;
        send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        char msg = 1;
        while (moreInf) {
            send(pClient->cliSock, &msg, 1, 0);

            recv(pClient->cliSock, &msg, 1, 0);
            if (msg) {
                outBuf.clear();
                moreInf = pClient->pUser->rExpToString(outBuf, i * 10, i * 10 + 9);
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
    case SRCHSENDER: {
        string sender;
        recvInf >> sender;
        try {
            vector<const Express*> expresses = pClient->pUser->searchSender(sender);

            char msg = SUCCESS;
            send(pClient->cliSock, &msg, 1, 0);

            int i = 0, j = 0;
            for (j = i * 10; j < i * 10 + 10 && j < expresses.size(); j++) {
                outBuf << "[" << j << "]" << endl;
                outBuf << expresses.at(j) << endl;
            }
            send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
            i++;
            char msg = 1;
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
        catch (const char* msg) {
            send(pClient->cliSock, msg, 1, 0);
        }
        break;
    }
    case SRCHRECEIVETM: {
        struct tm lowerBound;
        recvInf >> get_time(&lowerBound, "%Y-%m-%d %H:%M:%S");
        struct tm upperBound;
        recvInf >> get_time(&upperBound, "%Y-%m-%d %H:%M:%S");

        try {
            vector<const Express*> expresses = pClient->pUser->searchReceiveTime(mktime(&lowerBound), mktime(&upperBound));
            char msg = SUCCESS;
            send(pClient->cliSock, &msg, 1, 0);
            int i = 0, j = 0;
            for (j = i * 10; j < i * 10 + 10 && j < expresses.size(); j++) {
                outBuf << "[" << j << "]" << endl;
                outBuf << expresses.at(j) << endl;
            }
            send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
            i++;
            char msg = 1;
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
        catch (const char* msg) {
            send(pClient->cliSock, msg, 1, 0);
        }
        break;
    }
    default:
        return false;
    }
    return false;
}
