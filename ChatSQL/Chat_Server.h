#pragma once
#include "User.h"
#include "Message.h"
#include "Socket.h"
#include "Mysql_Chat.h"
#include <vector>
#include <exception>
#include <memory>
//using chrono::system_clock;

class Chat_Server {
	//std::vector<User> UserSpisok;				 //������ ������������������ �������������
	//std::vector<Message> messageList;			 //������� ������, ������� ������ ��������� � ����
	//std::shared_ptr<User> currentUser = nullptr; //��������� ��������� �� �������� ������������

	Socket_server sock;
	Mysql_Chat db;

public:
	Chat_Server() = default;
	~Chat_Server() = default;

	char* currentUser = nullptr;
	char* id_currentUser = nullptr;

	char* getCurrentUser() const;
	std::shared_ptr<User>getUserByName(const std::string& name) const;
	std::shared_ptr<User>getUserByLogin(const std::string& login) const;

	void Menu();
	void SignIn();														  //���� �� ������ � ������
	void ShowMessage();													  //���� �� ������ � ������
	bool FindLogin(const std::string& login);							  //�������� ������
	void NewUser();														  //�������� ������ ������������
	bool UserSearch(const std::string& login, const std::string& password);//����� ������������ �� ������ � ������
	void PrintNamesUsers();												  //��������� ������ ������������������ �������������
	int FindUserinUserSpisok(const std::string& name);					  //�������� ��������� �� ������� ���
	void setPrivateShowChat();											  //������ ������ ���������
	void setAllShowChat();												  //������ ����� ���������
	void setAddMessage();												  //���������� ��������� � ������
	std::string idToMessage(std::string& name);								//�������� ������ ���������
	void currrenNull();
};




