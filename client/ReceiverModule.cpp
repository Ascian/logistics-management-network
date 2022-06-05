#include "ReceiverModule.h"

bool ReceiverModule::execute(Main& main, const string& command, const set<int> ban)
{
    if (command == commands.at(0) && !ban.contains(0))  
        return true;
    else if (command == commands.at(1) && !ban.contains(1)) {
        try {
            main.user->displayNPExp();
            cout << endl;
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(2) && !ban.contains(2)) {
        try {
            main.user->displayNRExp();
            cout << endl;
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(3) && !ban.contains(3)) {
        try {
            main.user->displayRExp();
            cout << endl;
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(4) && !ban.contains(4)) {
        cout << "Please input the sender's username: ";
        string sender;
        cin >> sender;
        cout << endl;
        try {
            vector<const Express*> expresses = main.user->searchSender(sender);
            for (auto temp : expresses)
                cout << *((Express*)temp) << endl;
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(5) && !ban.contains(5)) {
        cout << "Please input the time information in format %Y-%m-%d %H:%M:%S" << endl;
        cout << "Lower Bound: ";
        struct tm lowerBound;
        cin >> get_time(&lowerBound, "%Y-%m-%d %H:%M:%S");
        cout << "Upper Bound: ";
        struct tm upperBound;
        cin >> get_time(&upperBound, "%Y-%m-%d %H:%M:%S");
        cout << endl;
        try {
            vector<const Express*> expresses = main.user->searchReceiveTime(mktime(&lowerBound), mktime(&upperBound));
            for (auto temp : expresses) {
                cout << *((Express*)temp) << endl;
            }
        }
        catch (const char* msg) {
            cout << msg << endl;
        }
    }
    else if (command == commands.at(6) && !ban.contains(6)) {
        cout << "Available commands:" << endl;
        if (!ban.contains(0))
            cout << "  " << commands.at(0) << ": return to the upper module" << endl;
        if (!ban.contains(1))
            cout << "  " << commands.at(1) << ": display all not picked-up expresses' message" << endl;
        if (!ban.contains(2))
            cout << "  " << commands.at(2) << ": display all not received expresses' message" << endl;
        if (!ban.contains(3))
            cout << "  " << commands.at(3) << ": display all received expresses' message" << endl;
        if (!ban.contains(4))
            cout << "  " << commands.at(4) << ": search the related expresses by sender" << endl;
        if (!ban.contains(5))
            cout << "  " << commands.at(5) << ": search the related expresses by receiving time" << endl;
        if (!ban.contains(6))
            cout << "  " << commands.at(6) << ": print this help" << endl;
    }
    else
        cout << "Unknown command" << endl;
    return false;
}
