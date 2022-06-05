#include "CourierModule.h"

bool CourierModule::execute(Main& main, const string& command, const set<int> ban)
{

    
    else if (command == commands.at(5) && !ban.contains(5)) {
        cout << "Not picked-up expresses:" << endl;
        try {
            main.pCourier->notPExpressToString();
        }
        catch (const char* msg) {
            cout << msg << endl;
            return false;
        }
        cout << endl;
        cout << "Please input the courier numbers of not expresses you want to pick up" << endl;
        cout << "Note: input '#' as the end" << endl;
        unsigned int i = 1;
        cout << i << ": ";
        string courierNum;
        cin >> courierNum;
        regex r("[0-9]{10}");
        while (!(regex_match(courierNum, r) || courierNum == "#")) {
            cout << "Format error, please input a 10-digits courier number or '#'" << endl;
            cout << i << ": ";
            cin >> courierNum;
        } 
        while (courierNum != "#") {
            try {
                Express* pExpress = main.pLogistics->findExpress(stoul(courierNum));
                if (pExpress->getCourier() == main.pCourier->getUsername())
                    main.pLogistics->pickUpExpress(pExpress);
                else
                    cout << "You can not pick up this express" << endl;
            }
            catch (const char* msg) {
                cout << msg << endl;
            }
            i++;
            cout << i << ": ";
            cin >> courierNum;
            while (!(regex_match(courierNum, r) || courierNum == "#")) {
                cout << "Format error, please input a 10-digits courier number or '#'" << endl;
                cout << i << ": ";
                cin >> courierNum;
            }
        }
    }
    else if (command == commands.at(6) && !ban.contains(6)) {
        try {
            main.pCourier->notPExpressToString();
            cout << endl;
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(7) && !ban.contains(7)) {
        try {
            main.pCourier->notRExpToString();
            cout << endl;
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(8) && !ban.contains(8)) {
        try {
            main.pCourier->rExpToString();
            cout << endl;
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(9) && !ban.contains(9)) {
        cout << "Please input the sender's username: ";
        string sender;
        cin >> sender;
        try {
            vector<const Express*> expresses = main.pCourier->searchSender(sender);
            for (auto temp : expresses)
                cout << *((Express*)temp) << endl;
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(10) && !ban.contains(10)) {
        cout << "Please input the receiver's username: ";
        string receiver;
        cin >> receiver;
        try {
            vector<const Express*> expresses = main.pCourier->searchReceiver(receiver);
            for (auto temp : expresses)
                cout << *((Express*)temp) << endl;
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(11) && !ban.contains(11)) {
        cout << "Please input the time information in format %Y-%m-%d %H:%M:%S" << endl;
        cout << "Lower Bound: ";
        struct tm lowerBound;
        cin >> get_time(&lowerBound, "%Y-%m-%d %H:%M:%S");
        cout << "Upper Bound: ";     
        struct tm upperBound;
        cin >> get_time(&upperBound, "%Y-%m-%d %H:%M:%S");
        cout << endl;
        try {
            vector<const Express*> expresses = main.pCourier->searchPickTime(mktime(&lowerBound), mktime(&upperBound));
            for (auto temp : expresses) {
                cout << *((Express*) temp) << endl;
            }
        }   
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(12) && !ban.contains(12)) {
        cout << "Please input the courier number: ";
        string courierNum;
        cin >> courierNum;
        regex r("[0-9]{10}");
        while (!regex_match(courierNum, r)) {
            cout << "Format error, courier number has 10 digits" << endl;
            cout << "Please input the courier number: ";
            cin >> courierNum;
        }
        cout << endl;
        try {
            cout << *main.pLogistics->findExpress(stoul(courierNum)) << endl;
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(13) && !ban.contains(13)) {
        cout << "Available commands:" << endl;
        if (!ban.contains(0))
            cout << "  " << commands.at(0) << ": return to the upper module" << endl;
        if (!ban.contains(1))
            cout << "  " << commands.at(1) << ": check your information" << endl;
        if (!ban.contains(2))
            cout << "  " << commands.at(2) << ": change your password" << endl;
        if (!ban.contains(3))
            cout << "  " << commands.at(3) << ": change your name" << endl;
        if (!ban.contains(4))
            cout << "  " << commands.at(4) << ": change your phone number" << endl;
        if (!ban.contains(5))
            cout << "  " << commands.at(5) << ": pick up expresses" << endl;
        if (!ban.contains(6))
            cout << "  " << commands.at(6) << ": display all not picked-up expresses' message" << endl;
        if (!ban.contains(7))
            cout << "  " << commands.at(7) << ": display all not received expresses' message" << endl;
        if (!ban.contains(8))
            cout << "  " << commands.at(8) << ": display all received expresses' message" << endl;
        if (!ban.contains(9))
            cout << "  " << commands.at(9) << ": search the related expresses by sender" << endl;
        if (!ban.contains(10))
            cout << "  " << commands.at(10) << ": search the related expresses by receiver" << endl;
        if (!ban.contains(11))
            cout << "  " << commands.at(11) << ": search the related expresses by pick-up time" << endl;
        if (!ban.contains(12))
            cout << "  " << commands.at(12) << ": find the specific expresses by courier number" << endl;
        if (!ban.contains(13))
            cout << "  " << commands.at(13) << ": print this help" << endl;
    }
    else
        cout << "Unknown command" << endl;
    return false;
}

bool CourierModule::execute(Logistics* pLogistics, Client* pClient, const char* recvBuf)
{
    ostringstream outBuf;
    istringstream recvInf(recvBuf);
    char event;
    recvInf >> event;
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

        cout << "Not picked-up expresses:" << endl;
        try {
            main.pCourier->notPExpressToString();
        }
        catch (const char* msg) {
            cout << msg << endl;
            return false;
        }
        cout << endl;
        cout << "Please input the courier numbers of not expresses you want to pick up" << endl;
        cout << "Note: input '#' as the end" << endl;
        unsigned int i = 1;
        cout << i << ": ";
        string courierNum;
        cin >> courierNum;
        regex r("[0-9]{10}");
        while (!(regex_match(courierNum, r) || courierNum == "#")) {
            cout << "Format error, please input a 10-digits courier number or '#'" << endl;
            cout << i << ": ";
            cin >> courierNum;
        }
        while (courierNum != "#") {
            try {
                Express* pExpress = main.pLogistics->findExpress(stoul(courierNum));
                if (pExpress->getCourier() == main.pCourier->getUsername())
                    main.pLogistics->pickUpExpress(pExpress);
                else
                    cout << "You can not pick up this express" << endl;
            }
            catch (const char* msg) {
                cout << msg << endl;
            }
            i++;
            cout << i << ": ";
            cin >> courierNum;
            while (!(regex_match(courierNum, r) || courierNum == "#")) {
                cout << "Format error, please input a 10-digits courier number or '#'" << endl;
                cout << i << ": ";
                cin >> courierNum;
            }
        }
    }
    default:
        return false;
    }
    return false;
}
