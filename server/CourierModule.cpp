#include "CourierModule.h"

bool CourierModule::execute(Logistics* pLogistics, Client* pClient, const char* recvBuf, mutex& mutx)
{
    ostringstream outBuf;
    istringstream recvInf(recvBuf);
    char event;
    recvInf.get(event);
    switch (event) {
    case RETURN:
        return true;
        break;
    case CHECK: {
        outBuf << *(pClient->pCourier);
        send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
        break;
    }
    case PASSWORD: {
        string password;
        recvInf >> password;
        pClient->pCourier->setPassword(password);
        break;
    }
    case NAME: {
        string name;
        recvInf >> name;
        pClient->pCourier->setName(name);
        break;
    }
    case PHONE: {
        string phone;
        recvInf >> phone;
        pClient->pCourier->setPhone(stoll(phone));
        break;
    }
    case PICKUP: {
        int i = 0;
        bool moreInf;
        try {
            moreInf = pClient->pCourier->notPExpToString(outBuf, i * 10, i * 10 + 9);
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
                moreInf = pClient->pCourier->notPExpToString(outBuf, i * 10, i * 10 + 9);
                i++;
                send(pClient->cliSock, outBuf.str().c_str(), outBuf.str().size(), 0);
                Sleep(1);
            }
            else
                break;
        }
        msg = 0;
        send(pClient->cliSock, &msg, 1, 0);

        while (true) {
            unsigned int courierNum;
            int len = recv(pClient->cliSock, (char*)&courierNum, 4, 0);
            if (len < 4)
                break;
            try {
                Express* pExpress = pLogistics->findExpress(courierNum);
                if (pExpress->getCourier() == pClient->pCourier->getUsername()) {
                    if (pExpress->getSendTime() != NOT_SENDED) {
                        char msg = EXPRESS_ALREADY_PICKUP;
                        send(pClient->cliSock, &msg, 1, 0);
                    }
                    else {
                        mutx.lock();
                        pLogistics->pickUpExpress(pExpress);
                        mutx.unlock();
                        cout << "Express \"" << setfill('0') << setw(10) << courierNum << "\" picked up" << endl;
                        char msg = SUCCESS;
                        send(pClient->cliSock, &msg, 1, 0);
                    }
                }                    
                else {
                    char msg = ITEM_NOT_BELONG_TO_YOU;
                    send(pClient->cliSock, &msg, 1, 0);
                }    
            }
            catch (const char msg) {
                send(pClient->cliSock, &msg, 1, 0);
            }
        }
        break;
    }
    case DSPLYNPEXP: {
        int i = 0;
        bool moreInf;
        try {
            moreInf = pClient->pCourier->notPExpToString(outBuf, i * 10, i * 10 + 9);
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
                moreInf = pClient->pCourier->notPExpToString(outBuf, i * 10, i * 10 + 9);
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

    case DSPLYNREXP: {
        int i = 0;
        bool moreInf;
        try {
            moreInf = pClient->pCourier->notPExpToString(outBuf, i * 10, i * 10 + 9);
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
                moreInf = pClient->pCourier->notRExpToString(outBuf, i * 10, i * 10 + 9);
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
    case DSPLYREXP: {
        int i = 0;
        bool moreInf;
        try {
            moreInf = pClient->pCourier->notPExpToString(outBuf, i * 10, i * 10 + 9);
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
                moreInf = pClient->pCourier->rExpToString(outBuf, i * 10, i * 10 + 9);
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
    case SRCHSENDER: {
        string sender;
        recvInf >> sender;
        try {
            vector<const Express*> expresses = pClient->pCourier->searchSender(sender); 

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
    case SRCHRECEIVER: {
        string receiver;
        recvInf >> receiver;
        try {
            vector<const Express*> expresses = pClient->pCourier->searchReceiver(receiver);
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
    case SRCHPICKTM: {
        struct tm lowerBound;
        recvInf >> get_time(&lowerBound, "%Y-%m-%d %H:%M:%S");
        struct tm upperBound;
        recvInf >> get_time(&upperBound, "%Y-%m-%d %H:%M:%S");

        try {
            vector<const Express*> expresses = pClient->pCourier->searchPickTime(mktime(&lowerBound), mktime(&upperBound));
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
