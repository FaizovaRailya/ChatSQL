#pragma once
#include "Socket.h"
#include "Mysql_Chat.h"
#include <exception>
#include <memory>

class Chat_Server {
	Socket_server sock;
	Mysql_Chat db;

public:
	Chat_Server() = default;
	~Chat_Server() = default;

	char* currentUser = nullptr;
	char* id_currentUser = nullptr;

	void Menu();
	void SignIn();														  //���� �� ������ � ������
	void ShowMessage();													  //���� �� ������ � ������
	bool FindLogin(const std::string& login);							  //�������� ������
	void NewUser();														  //�������� ������ ������������
	bool UserSearch(const std::string& login, const std::string& password);//����� ������������ �� ������ � ������
	void PrintNamesUsers();												  //��������� ������ ������������������ �������������
	bool FindUserinUserSpisok(const std::string& name);					  //�������� ��������� �� ������� ���
	void setUnreadPrivateShowChat();									  //������ �� ����������� ������ ���������
	void setPrivateShowChat();											  //������ ������ ���������
	void setAllShowChat();												  //������ ����� ���������
	void setAddMessage();												  //���������� ��������� � ������
};




