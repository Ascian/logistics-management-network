#include"ManagerModule.h"

bool ManagerModule::execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx)
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
    case CHECK: {
        outBuf << pLogistics->manager;
        send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        break;
    }
    case PASSWORD: {
        string password;
        recvInf >> password;
        pLogistics->manager.setPassword(password);
        break;
    }
    case NAME: {
        string name;
        recvInf >> name;
        pLogistics->manager.setName(name);
        break;
    }
    case ADDCOURIER: {
        string username;
        string password;
        string name;
        string phone;
        recvInf >> username >> password >> name >> phone;
        try {
            mutx.lock();
            pLogistics->createCourierAccount(username, password, name, stoull(phone));
            mutx.unlock();
            cout << "A new courier \"" << username << "\" added" << endl;
            char outBuf = SUCCESS;
            send(pClient->cliSock, &outBuf, 1, 0);
        }
        catch (const char msg) {
            mutx.unlock();
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case DELETECOURIER: {
        string username;
        recvInf >> username;
        try {
            mutx.lock();
            pLogistics->deleteCourierAccount(username);
            mutx.unlock();
            cout << "Courier \"" << username << "\" deleted" << endl;
            char outBuf = SUCCESS;
            send(pClient->cliSock, &outBuf, 1, 0);
        }
        catch (const char msg) {
            mutx.unlock();
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case ASSIGN: {
        string username;
        recvInf >> username;
        int i = 0;
        bool moreInf;
        try {
            moreInf = pLogistics->notAExpToString(outBuf, i * 10, i * 10 + 9);
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
                moreInf = pLogistics->notAExpToString(outBuf, i * 10, i * 10 + 9);
                i++;
                send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
                Sleep(1);
            }
            else
                break;
        }
        msg = 0;
        send(pClient->cliSock, &msg, 1, 0);

        unsigned int courierNum;
        recv(pClient->cliSock, (char*)&courierNum, 4, 0);

        Express* pExpress;
        try {
            pExpress = pLogistics->findExpress(courierNum);
        }
        catch (const char msg) {
            send(pClient->cliSock, &msg, 1, 0);
            break;
        }

        try {
            mutx.lock();
            pLogistics->assignExpress(pExpress, username);
            mutx.unlock();
            cout << "Express \"" << setfill('0') << setw(10) << courierNum << 
                "\" assigned to courier \"" << username << "\"" << endl;
            char outBuf = SUCCESS;
            send(pClient->cliSock, &outBuf, 1, 0);
        }
        catch (const char msg) {
            mutx.unlock();
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case DSPLYUSER: {
        int i = 0;
        bool moreInf;
        try {
            moreInf = pLogistics->usersToString(outBuf, i * 10, i * 10 + 9);
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
                moreInf = pLogistics->usersToString(outBuf, i * 10, i * 10 + 9);
                i++;
                send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
                Sleep(1);
            }
            else
                break;
        }
        msg = 0;
        send(pClient->cliSock, &msg, 1, 0);
        break;
    }
    case DSPLYCOURIER: {
        int i = 0;
        bool moreInf;
        try {
            moreInf = pLogistics->couriersToString(outBuf, i * 10, i * 10 + 9);
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
                moreInf = pLogistics->couriersToString(outBuf, i * 10, i * 10 + 9);
                i++;
                send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
                Sleep(1);
            }
            else
                break;
        }
        msg = 0;
        send(pClient->cliSock, &msg, 1, 0);
        break;
    }
    case DSPLYEXP: {
        int i = 0;
        bool moreInf;
        try {
            moreInf = pLogistics->expressesToString(outBuf, i * 10, i * 10 + 9);
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
                moreInf = pLogistics->expressesToString(outBuf, i * 10, i * 10 + 9);
                i++;
                send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
                Sleep(1);
            }
            else
                break;
        }
        msg = 0;
        send(pClient->cliSock, &msg, 1, 0);
        break;
    }
    case SRCHCREATETM: {
        struct tm lowerBound;
        recvInf >> get_time(&lowerBound, "%Y-%m-%d %H:%M:%S");
        struct tm upperBound;
        recvInf >> get_time(&upperBound, "%Y-%m-%d %H:%M:%S");

        try {
            vector<const Express*> expresses = pLogistics->searchCreateTime(mktime(&lowerBound), mktime(&upperBound));
            char msg = SUCCESS;
            send(pClient->cliSock, &msg, 1, 0);
            int i = 0, j = 0;
            for (j = i * 10; j < i * 10 + 10 && j < expresses.size(); j++) {
                outBuf << "[" << j << "]" << endl;
                outBuf << *(Express*)expresses.at(j) << endl;
            }
            send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
            Sleep(1);
            i++;
            msg = 1;
            while (j < expresses.size()) {
                send(pClient->cliSock, &msg, 1, 0);

                recv(pClient->cliSock, &msg, 1, 0);
                if (msg) {
                    outBuf.clear();
                    for (j = i * 10; j < i * 10 + 10 && j < expresses.size(); j++) {
                        outBuf << "[" << j << "]" << endl;
                        outBuf << *(Express*)expresses.at(j) << endl;
                    }
                    i++;
                    send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
                    Sleep(1);
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
    case FINDUSER: {
        string username;
        recvInf >> username;
        try {
            pClient->pUser = pLogistics->findUser(username);
            currentModule = 0;
            char outBuf = SUCCESS;
            send(pClient->cliSock, &outBuf, 1, 0);
        }
        catch (const char msg) {
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case FINDCOURIER: {
        string username;
        recvInf >> username;
        try {
            pClient->pCourier = pLogistics->findCourier(username);
            currentModule = 1;
            char outBuf = SUCCESS;
            send(pClient->cliSock, &outBuf, 1, 0);
        }
        catch (const char msg) {
            send(pClient->cliSock, &msg, 1, 0);
        }
        break;
    }
    case FINDEXP: {
        string courierNum;
        recvInf >> courierNum;
        try {
            outBuf << *pLogistics->findExpress(stoul(courierNum)) << endl;
            char msg = SUCCESS;
            send(pClient->cliSock, &msg, 1, 0);
            send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
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
