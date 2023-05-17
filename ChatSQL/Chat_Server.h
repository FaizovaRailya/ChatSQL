#pragma once
#include "User.h"
#include "Message.h"
#include "Socket.h"
#include <vector>
#include <exception>
#include <memory>

class Chat_Server {
	std::vector<User> UserSpisok;				 //������ ������������������ �������������
	std::vector<Message> messageList;			 //������� ������, ������� ������ ��������� � ����
	std::shared_ptr<User> currentUser = nullptr; //��������� ��������� �� �������� ������������
	Socket_server sock;

public:
	Chat_Server() = default;
	~Chat_Server() = default;

	std::shared_ptr<User>getCurrentUser() const;
	std::shared_ptr<User>getUserByName(const std::string& name) const;
	std::shared_ptr<User>getUserByLogin(const std::string& login) const;

	void Menu();
	void SignIn();														  //���� �� ������ � ������
	void ShowMessage();													  //���� �� ������ � ������
	bool FindName(const std::string& name);							      //�������� �����
	bool FindLogin(const std::string& login);							  //�������� ������
	void NewUser();														  //�������� ������ ������������
	bool UserSearch(const std::string& login, const std::string& password);//����� ������������ �� ������ � ������
	void PrintNamesUsers();												  //��������� ������ ������������������ �������������
	int FindUserinUserSpisok(const std::string& name);					  //�������� ��������� �� ������� ���
	void setPrivateShowChat();											  //������ ������ ���������
	void setAllShowChat();												  //������ ����� ���������
	void setAddMessage();												  //���������� ��������� � ������
	void currrenNull();
};




