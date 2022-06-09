#include "SenderModule.h"

bool SenderModule::execute(Main& main, const string& command, const set<int> ban)
{
    if (command == commands.at(0) && !ban.contains(0))
        return true;
    else if (command == commands.at(1) && !ban.contains(1)) {
        try {
            main.pUser->notSExpToString();
            cout << endl;
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(2) && !ban.contains(2)) {
        try {
            main.pUser->sExpToString();
            cout << endl;
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(3) && !ban.contains(3)) {
        cout << "Please input the receiver's username: ";
        string receiver;
        cin >> receiver;
        cout << endl;
        try {
            vector<const Express*> expresses = main.pUser->searchReceiver(receiver);
            for (auto temp : expresses)
                cout << *((Express*)temp) << endl;
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(4) && !ban.contains(4)) {
        cout << "Please input the time information in format %Y-%m-%d %H:%M:%S" << endl;
        cout << "Lower Bound: ";
        struct tm lowerBound;
        cin >> get_time(&lowerBound, "%Y-%m-%d %H:%M:%S");
        cout << "Upper Bound: ";
        struct tm upperBound;
        cin >> get_time(&upperBound, "%Y-%m-%d %H:%M:%S");
        cout << endl;
        try {
            vector<const Express*> expresses = main.pUser->searchSendTime(mktime(&lowerBound), mktime(&upperBound));
            for (auto temp : expresses) {
                cout << *((Express*)temp) << endl;
            }
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(5) && !ban.contains(5)) {
        cout << "Available commands:" << endl;
        if (!ban.contains(0))
            cout << "  " << commands.at(0) << ": return to the upper module" << endl;
        if (!ban.contains(1))
            cout << "  " << commands.at(1) << ": display all not sended expresses' message" << endl;
        if (!ban.contains(2))
            cout << "  " << commands.at(2) << ": display all sended expresses' message" << endl;
        if (!ban.contains(3))
            cout << "  " << commands.at(4) << ": search the related expresses by receiver" << endl;
        if (!ban.contains(4))
            cout << "  " << commands.at(4) << ": search the related expresses by sending time" << endl;
        if (!ban.contains(5))
            cout << "  " << commands.at(5) << ": print this help" << endl;
    }
    else
        cout << "Unknown command" << endl;
    return false;
}

bool SenderModule::execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx)
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
    case DSPLYNSEXP: {
        int i = 0;
        bool moreInf;
        try {
            moreInf = pClient->pUser->notSExpToString(outBuf, i * 10, i * 10 + 9);
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
