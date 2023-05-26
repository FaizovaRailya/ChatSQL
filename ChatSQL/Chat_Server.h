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
	void SignIn();														  //вход по логину и паролю
	void ShowMessage();													  //вход по логину и паролю
	bool FindLogin(const std::string& login);							  //проверка логина
	void NewUser();														  //создание нового пользователя
	bool UserSearch(const std::string& login, const std::string& password);//поиск пользователя по логину и паролю
	void PrintNamesUsers();												  //получение списка зарегестрированных пользователей
	bool FindUserinUserSpisok(const std::string& name);					  //проверка корректно ли введено имя
	void setUnreadPrivateShowChat();									  //чтение не прочитанных личных сообщений
	void setPrivateShowChat();											  //чтение личных сообщений
	void setAllShowChat();												  //чтение общих сообщений
	void setAddMessage();												  //добавление сообщения в массив
};




