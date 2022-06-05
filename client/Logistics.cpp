#include "Logistics.h"


bool Logistics::createCourierAccount(const string& username, const string& password, const string& name, const unsigned _int64 phone)
{
	if (couriers.size() >= couriers.max_size()) {
		throw "Username amount reached the Maximum";
		return false;
	}
	if (couriers.contains(username)) {
		throw "Username already exists";
		return false;
	}
	Courier* newAccount = new Courier(username, password, name, phone);
	couriers.insert({ username , newAccount });
	return true;
}


bool Logistics::deleteCourierAccount(const string& username)
{
	if (!couriers.contains(username)) {
		throw "Username not found";
		return false;
	}
	couriers.erase(couriers.find(username));
	return true;
}

void createExpress(Express*& pExpress, const string& senderUsername, const string& receiverUsername,
	const time_t creatTime, const string& condition, const string& description, const unsigned int courierNumber,
	const unsigned int amount, const string& kind) {
	delete(pExpress);

	if (kind == Express::subKind().at(0))
		pExpress = new NormalExpress(senderUsername, receiverUsername, creatTime, condition, description, courierNumber, amount);
	else if (kind == Express::subKind().at(1))
		pExpress = new Fragile(senderUsername, receiverUsername, creatTime, condition, description, courierNumber, amount);
	else if (kind == Express::subKind().at(2))
		pExpress = new Book(senderUsername, receiverUsername, creatTime, condition, description, courierNumber, amount);
	else
		pExpress = NULL;
}

bool Logistics::createLogistics(const string& senderUsername, const string& receiverUsername, 
	const string& condition, const string& description, const unsigned int amount, const string& kind)
{
	if (senderUsername == receiverUsername) {
		throw "Receiver should not be the same as sender";
		return false;
	}

	auto temp = users.find(senderUsername);
	if (temp == users.end()) {
		throw "Sender does not exist";
		return false;
	}
	User* sender = temp->second;
	temp = users.find(receiverUsername);
	if (temp == users.end()) {
		throw "Receiver does not exist";
		return false;
	}
	User* receiver = temp->second;

	//������ݵ���
	if (expresses.size() >= expresses.max_size()) {
		throw "Express amount reached the Maximum";
		return false;
	}
	unsigned int courierNumber = expresses.size();

	Express* express = NULL;
	createExpress(express, sender->getUsername(), receiver->getUsername(), time(0), 
		condition, description, courierNumber, amount, kind);
	if (!express) {
		throw("Unknown kind");
		return false;
	}

	if (sender->getBalance() < express->getPrice()) {
		//����
		throw "Balance is not enough";
		return false;
	}

	expresses.insert({ courierNumber,express });
	createTimeList.insert({ express->getCreatTime(), express});
	notAssignedList.insert(express);

	//���ͷ�ת�˸�����Ա
	sender->subBalance(express->getPrice());
	manager.addBalance(express->getPrice());

	//�޸ķ��ͷ������Ϣ
	sender->addNotSendedList(express);//����δ��������б�
	sender->addReceiverList(express);//��������˲��ұ�

	//�޸Ľ��շ������Ϣ
	receiver->addNotPickedList(express);//����Ϊ���տ���б�
	receiver->addSenderList(express);//���뷢���˲��ұ�

	return true;
}

bool Logistics::assignExpress(Express* pExpress, const string& courierUsername){
	if (pExpress->getSendTime() != NOT_ASSIGNED) {
		throw("Express is already assigned to a courier");
		return false;
	}
	
	auto temp = couriers.find(courierUsername);
	if (temp == couriers.end()) {
		throw "Courier does not exist";
		return false;
	}
	Courier* courier = temp->second;

	notAssignedList.erase(pExpress);//ɾ��δ������
	pExpress->setCourier(courierUsername);
	pExpress->setSendTime(NOT_SENDED);

	//�޸Ŀ��Ա�����Ϣ
	courier->addNotPickedList(pExpress);
	courier->addSenderList(pExpress);
	courier->addReceiverList(pExpress);
	return true;
}

void Logistics::pickUpExpress(Express* pExpress)
{
	if (pExpress->getSendTime() != NOT_SENDED) {
		throw("Express is already picked up");
		return;
	}
	pExpress->setSendTime(time(0));

	//�޸Ŀ��Ա�����Ϣ
	Courier* courier = couriers.find(pExpress->getCourier())->second;
	courier->deleteNotPickedList(pExpress);//ɾ�������տ��
	courier->addNotReceivedList(pExpress);//�����ǩ�տ���б�
	courier->addPickTimeList(pExpress);//���������տ���б�

	//����Աת�˸����Ա
	courier->addBalance((double)pExpress->getPrice() / 2);
	manager.subBalance((double)pExpress->getPrice() / 2);

	//�޸ķ��ͷ������Ϣ
	User* sender = users.find(pExpress->getSender())->second;
	sender->deleteNotSendedList(pExpress);//ɾ��δ�������
	sender->addSendList(pExpress);//�����ѷ�������б�

	//�޸Ľ��շ������Ϣ
	User* receiver = users.find(pExpress->getReceiver())->second;
	receiver->deleteNotPickedList(pExpress);//ɾ��δ���տ��
	receiver->addNotReceivedList(pExpress);//�����ǩ�տ���б�
}

void Logistics::signForExpress(Express* pExpress)
{
	if (pExpress->getSendTime() == NOT_ASSIGNED || pExpress->getSendTime() == NOT_SENDED) {
		throw("Express is not able to be signed for");
		return;
	}
	if (pExpress->getReceiveTime() != NOT_RECEIVED) {
		throw("Express is already signed for");
		return;
	}

	pExpress->setReceiveTime(time(0));

	//�޸Ŀ��Ա�����Ϣ
	Courier* courier = couriers.find(pExpress->getCourier())->second;
	courier->deleteNotReceivedList(pExpress);//ɾ����ǩ�տ��
	courier->addReceivedList(pExpress);//������ǩ�տ���б�

	//�޸Ľ��շ������Ϣ
	User* receiver = users.find(pExpress->getReceiver())->second;
	receiver->deleteNotReceivedList(pExpress);//ɾ����ǩ�տ��
	receiver->addReceiveList(pExpress);//������ǩ�տ���б�
}

void Logistics::displayUser(){
	if (users.size() == 0) {
		throw("No user");
		return;
	}
	unsigned int i = 1;
	for (auto& user : users) {
		cout << "[" << i << "]" << endl;
		cout << *user.second << endl;
		i++;
	}
}

void Logistics::displayCourier()
{
	if (couriers.size() == 0) {
		throw("No courier");
		return;
	}
	unsigned int i = 1;
	for (auto& courier : couriers) {
		cout << "[" << i << "]" << endl;
		cout << *courier.second << endl;
		i++;
	}
}

void Logistics::displayExpress(){
	if (expresses.size() == 0) {
		throw("No express");
		return;
	}
	unsigned int i = 1;
	for (auto& express : expresses) {
		cout << "[" << i << "]" << endl;
		cout << *express.second << endl;
		i++;
	}
}

void Logistics::displayNAExp()
{
	if (notAssignedList.size() == 0) {
		throw("No express not assigned");
		return;
	}
	unsigned int i = 0;
	for (auto temp : notAssignedList) {
		cout << "[" << i++ << "]" << endl;
		cout << *temp;
	}
}

Courier* Logistics::findCourier(const string& username)
{
	auto temp = couriers.find(username);
	if (temp == couriers.end()) {
		throw("No such user");
		return NULL;
	}
	return temp->second;
}

User* Logistics::findUser(const string& username)
{
	auto temp = users.find(username);
	if (temp == users.end()) {
		throw("No such user");
		return NULL;
	}
	return temp->second;
}

Express* Logistics::findExpress(const unsigned int courierNumber)
{
	auto temp = expresses.find(courierNumber);
	if (temp == expresses.end()) {
		throw("No such express");
		return NULL;
	}
	return temp->second;
}

const vector<const Express*> Logistics::searchCreateTime(const time_t& begin, const time_t end)
{
	vector<const Express*> result;
	for (auto temp = createTimeList.lower_bound(begin); temp != createTimeList.end() && temp->first <= end; temp++) {
		result.push_back(temp->second);
	}
	if (result.size() == 0)
		throw("No related express");
	return result;
}

void Logistics::save(const string& savePath){
	ofstream managerFile(savePath + "/Manager", ios::out | ios::trunc);
	managerFile << manager;
	managerFile.close();

	ofstream courierFile(savePath + "/Couriers", ios::out | ios::trunc);
	courierFile << couriers.size() << endl << endl;
	for (auto& courier : couriers) {
		courierFile << *courier.second << endl;
	}
	courierFile.close();

	ofstream userFile(savePath+"/Users", ios::out | ios::trunc);
	userFile << users.size() << endl << endl;
	for (auto& user : users) {
		userFile << *user.second << endl;
	}
	userFile.close();

	ofstream expFile(savePath + "/Expresses", ios::out | ios::trunc);
	expFile << expresses.size() << endl << endl;
	for (auto& express : expresses) {
		expFile << *express.second << endl;
	}
	expFile.close();
}



void loadExpress(istream& Istr, Express*& pExpress) {
	delete(pExpress);

	string kind;
	Istr.ignore(strlen("Kind: "));
	Istr >> kind;
	if (kind == Express::subKind().at(0))
		pExpress = new NormalExpress();
	else if (kind == Express::subKind().at(1))
		pExpress = new Fragile();
	else if (kind == Express::subKind().at(2))
		pExpress = new Book();
	Istr >> *pExpress;
}

void Logistics::load(const string& loadPath){
	ifstream managerFile(loadPath + "/Manager", ios::in);
	if (!managerFile.is_open()) {
		throw("File \"/Manager\" does not exist");
		return;
	}
	managerFile >> manager;
	managerFile.close();

	ifstream courierFile(loadPath + "/Couriers", ios::in);
	if (!courierFile.is_open()) {
		throw("File \"/Couriers\" does not exist");
		return;
	}
	unsigned int num = 0;
	courierFile >> num;
	courierFile.ignore(1);
	for (unsigned int i = 0; i < num; i++) {
		Courier* courier = new Courier();
		courierFile >> *courier;
		couriers.insert({ courier->getUsername(),courier });
		courierFile.ignore(1);
	}
	courierFile.close();

	ifstream userFile(loadPath + "/Users", ios::in);
	if (!userFile.is_open()) {
		throw("File \"/Users\" does not exist");
		return;
	}
	userFile >> num;
	userFile.ignore(1);
	for (unsigned int i = 0; i < num; i++) {
		User* user = new User();
		userFile >> *user;
		users.insert({ user->getUsername(),user});
		userFile.ignore(1);
	}
	userFile.close();

	ifstream expFile(loadPath + "/Expresses", ios::in);
	if (!expFile.is_open()) {
		throw("File \"/Expresses\" does not exist");
		return;
	}
	expFile >> num;
	expFile.ignore(1);
	for (unsigned int i = 0; i < num; i++) {
		Express* express = NULL;
		loadExpress(expFile, express);

		//�������Ա������ұ�
		expresses.insert({ express->getCourierNumber(),express});
		createTimeList.insert({express->getCreatTime(), express});
		if (express->getSendTime() == NOT_ASSIGNED)
			notAssignedList.insert(express);
		else {
			//������Ա������ұ�
			Courier* courier = couriers.find(express->getCourier())->second;
			if (express->getSendTime() == NOT_SENDED) {
				courier->addNotPickedList(express);
			}
			else {
				courier->addPickTimeList(express);
				if (express->getReceiveTime() == NOT_RECEIVED)
					courier->addNotReceivedList(express);
				else
					courier->addReceivedList(express);
			}
			courier->addSenderList(express);
			courier->addReceiverList(express);
		}
		
		//���뷢�ͷ�������ұ�
		User* user = users.find(express->getSender())->second;
		if (express->getSendTime() == NOT_ASSIGNED || express->getSendTime() == NOT_SENDED) {
			user->addNotSendedList(express);
		}
		else
			user->addSendList(express);
		user->addReceiverList(express);


		//������շ�������ұ�
		user = users.find(express->getReceiver())->second;
		if (express->getSendTime() == NOT_ASSIGNED || express->getSendTime() == NOT_SENDED)
			user->addNotPickedList(express);
		else if (express->getReceiveTime() == NOT_RECEIVED)
			user->addNotReceivedList(express);
		else
			user->addReceiveList(express);
		user->addSenderList(express);
		expFile.ignore(1);
	}
	expFile.close();
}

