#include <iostream>
#include<string>
#include "Chat_Server.h"
#include "Sha1.h"

std::shared_ptr<User> Chat_Server::getUserByLogin(const std::string& login) const {			// ��������� �� ����� ������������
	for (auto& user : UserSpisok) {
		if (login == user.getLogin())
			return std::make_shared<User>(user);
	}
	return nullptr;
}

std::shared_ptr<User> Chat_Server::getUserByName(const std::string& name) const {		// ��������� �� ��� ������������
	for (auto& user : UserSpisok) {
		if (name == user.getName())
			return std::make_shared<User>(user);
	}
	return nullptr;
}

std::shared_ptr<User> Chat_Server::getCurrentUser() const {
	return currentUser;
}

void Chat_Server::Menu() {
	bool f = true;
	while (f) {
		sock.send_data((char*)"------������� ��������:------\n1 - ����\n2 - �����������\n0 - �����\n");
		sock.send_data((char*)"end_receive");
		char* choise = sock.receive_data();
		switch (choise[0]) {
		case '1':
			SignIn();								//���� �� ������ � ������
			break;
		case '2':									// ����������� ������ ������������
			try {
				NewUser();					//�������� ����� ���������� ������ ������������			
				currrenNull();
			}
			catch (std::exception& e) {
				std::cout << e.what() << std::endl;
			}
			break;
		case '0':
			sock.send_data((char*)"Exit");
			sock.send_data((char*)"end_chat");
			f = sock.listen_socket();
			f = false;
			break;
		default:
			sock.send_data((char*)"�������� ����!\n");
			sock.send_data((char*)"end_chat");
			f = sock.listen_socket();
			break;
		}
	}
	std::cout << "Chat session terminated! See you again!" << std::endl;
}

void Chat_Server::SignIn() {								//���� �� ������ � ������
	std::string log, pas;
	sock.send_data((char*)"----�������----\n�����: ");
	sock.send_data((char*)"end_receive");
	log = sock.receive_data();

	sock.send_data((char*)"������: ");
	sock.send_data((char*)"end_receive");
	pas = sock.receive_data();

	if (UserSearch(log, pas)) {
		bool f1 = true;
		while (f1) {
			sock.send_data((char*)"\n------������� �������e:------\n1 - ��������� ��������e\n2 - �������� ���������\n0 - �����\n");

			sock.send_data((char*)"end_receive");
			char* b = sock.receive_data();
			switch (b[0]) {
			case '1':
				if (messageList.empty())
					sock.send_data((char*)"� ��� ��� ��� �������� ���������!\n");
				else
					ShowMessage();							//��������� ���������	
				break;
			case '2':										//�������� ���������
				setAddMessage();
				break;
			case '0':										//�����
				currrenNull();
				f1 = false;
				break;
			default:
				sock.send_data((char*)"�������� ����!\n");
				break;
			}
		}
	}
	else
		sock.send_data((char*)"\n�������� ����� ��� ������!!!\n");
}

void Chat_Server::ShowMessage() {									//��������� ���������
	sock.send_data((char*)"\n------������� �������e:------\n1 - ������\n2 - �����\n0 - �����\n");
	sock.send_data((char*)"end_receive");
	char* d = sock.receive_data();

	switch (d[0]) {
	case '1':
		setPrivateShowChat();   //�������� ����� ������ ������ ���������  
		break;
	case '2':
		setAllShowChat();		 //�������� ����� ������ ����� ���������
	case '0':
		break;
	default:
		sock.send_data((char*)"�������� ����!\n");
		break;
	}
}

bool Chat_Server::FindName(const std::string& name) {								 //����� �������� �����
	for (std::vector<User>::iterator it = UserSpisok.begin(); it != UserSpisok.end(); ++it) {
		if (it->getName() == name)
			return true;
	}
	return false;
}

bool Chat_Server::FindLogin(const std::string& login) {								//����� �������� ������
	for (std::vector<User>::iterator it = UserSpisok.begin(); it != UserSpisok.end(); ++it) {
		if (it->getLogin() == login)
			return true;
	}
	return false;
}

void Chat_Server::NewUser() {					//����� �������� ������ ������������
	std::string name, login, password;

	sock.send_data((char*)"---- ������� ������ ��� ����������� ----\n���: ");
	bool n = true;
	while (n) {
		sock.send_data((char*)"end_receive");
		name = sock.receive_data();

		/*if (name == " ") {sock.send_data("�� �� ����� ���"); }*/
		/*std::cin >> name;*/
		if (name == "all" || FindName(name))
			sock.send_data((char*)"������ ��� ��� ������ �������� ������!\n������� ���:");
		else n = false;
	}
	bool l = true;
	while (l) {
		sock.send_data((char*)"�����: ");
		sock.send_data((char*)"end_receive");
		login = sock.receive_data();

		if (login == "all" || FindLogin(login))
			sock.send_data((char*)"������ ����� ��� ����� �������� ������!\n������� �����: ");
		else l = false;

	}
	sock.send_data((char*)"������: ");
	sock.send_data((char*)"end_receive");
	password = sock.receive_data();

	User user(name, login, sha1((char*)password.data(), password.length()));				//������� ������ ����� User
	UserSpisok.push_back(user);						// ��������� ������������ � ������
	currentUser = std::make_shared<User>(user);		//������� ��������� �� �������� ������������
	sock.send_data((char*)"������������ ���������������!\n\n");
}

bool Chat_Server::UserSearch(const std::string& login, const std::string& password) {	//����� ������ ������������ �� ������ � ������	
	int i = 0;
	while (i < UserSpisok.size()) {
		for (auto& user : UserSpisok) {
			if (login == user.getLogin() && memcmp(user.getPassword(), sha1((char*)password.data(), password.length()), SHA1HASHLENGTHBYTES) == 0) {
				currentUser = getUserByLogin(login);   //��������� �� �������� ������������

				std::string cur = currentUser->getName();
				char* c = const_cast<char*>(cur.c_str());

				sock.send_data((char*)"\n������������: ");
				sock.send_data(c);
				return true;
			}
			else { ++i; }
		}
	}
	return false;
}

void Chat_Server::PrintNamesUsers() {				    //����� ��������� ������ ������������������ �������������
	for (std::vector<User>::iterator it = UserSpisok.begin(); it != UserSpisok.end(); ++it) {
		std::string str = it->getName();
		char* c = const_cast<char*>(str.c_str());
		sock.send_data(c);
		sock.send_data((char*)"\n");
	}
}

int Chat_Server::FindUserinUserSpisok(const std::string& name) {	//����� ��������� ��������� �� ������� ���
	for (std::vector<User>::iterator it = UserSpisok.begin(); it != UserSpisok.end(); ++it) {
		if (it->getName() == name)
			return 0;
	}
	return -1;
}

void Chat_Server::setPrivateShowChat() {				//����� ������ ������ ���������
	std::string from;
	std::string to;

	sock.send_data((char*)"--------------���--------------\n");
	for (auto& message : messageList) {
		if (currentUser->getName() == message.getFromMessage() || currentUser->getName() == message.getToMessage()) {//���� ������� ������������
			from = (currentUser->getName() == message.getFromMessage()) ? "����" : message.getFromMessage();

			to = (currentUser->getName() == message.getToMessage()) ? "���" : message.getToMessage();
			//���� ������� ��� ����� to, �� ���������� ��������� ������ ����, ���� ���, �� �������� ��� ������������ � ����������� ��� �������� ���� to

			if (message.getToMessage() != "all") {
				sock.send_data((char*)"�� ");
				std::string strf = from;
				char* fr = const_cast<char*>(strf.c_str());
				sock.send_data(fr);
				sock.send_data((char*)" ���� ");
				std::string strt = to;
				char* t = const_cast<char*>(strt.c_str());
				sock.send_data(t);
				sock.send_data((char*)": ");
				std::string strm = message.getText();
				char* mes = const_cast<char*>(strm.c_str());
				sock.send_data(mes);
				sock.send_data((char*)"\n");
			}
		}
	}
	sock.send_data((char*)"-------------------------------");
}

void Chat_Server::setAllShowChat() {							// ����� ������ ����� ���������
	std::string from;

	sock.send_data((char*)"-----------����� ���-----------\n");
	for (auto& message : messageList) {
		if (currentUser->getName() == message.getFromMessage() || currentUser->getName() == message.getToMessage() || message.getToMessage() == "all") {//���� ������� ������������
			from = (currentUser->getName() == message.getFromMessage()) ? "����" : message.getFromMessage();
			if (message.getToMessage() == "all") { 						//��������� ���� �������������
				sock.send_data((char*)"�� ");
				std::string strf = from;
				char* fr = const_cast<char*>(strf.c_str());
				sock.send_data(fr);
				sock.send_data((char*)": ");
				std::string strm = message.getText();
				char* mes = const_cast<char*>(strm.c_str());
				sock.send_data(mes);
				sock.send_data((char*)"\n");
			}
		}
	}
	sock.send_data((char*)"-------------------------------\n");
}

void Chat_Server::setAddMessage() {						    	//����� ���������� ��������� � ������
	std::string inputName;
	std::string message;
	sock.send_data((char*)"\n������� ��� ���� ��������� ���������:\n");
	PrintNamesUsers();									  //������� ������ �������������
	sock.send_data((char*)"all - ��������� ����\n");
	sock.send_data((char*)"end_receive");
	inputName = sock.receive_data();

	if (inputName == "all") {							  //�������� ���� �������������
		sock.send_data((char*)"\n������� ����� ���������: \n");
		sock.send_data((char*)"end_receive");
		message = sock.receive_data();
		messageList.push_back(Message(currentUser->getName(), "all", message));
		sock.send_data((char*)"��������� ��������� ���� �������������!\n");
	}
	else {													//�������� ������ ���������
		int t = -1;
		t = FindUserinUserSpisok(inputName);
		if (t == -1) {
			sock.send_data((char*)"������������ � ������ ������ �� ������\n");
		}
		else {
			sock.send_data((char*)"������� ����� ���������: \n");
			sock.send_data((char*)"end_receive");
			message = sock.receive_data();
			messageList.push_back(Message(currentUser->getName(), inputName, message));
		}
	}
}

void Chat_Server::currrenNull() {
	currentUser = nullptr;
}

