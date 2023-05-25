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
	//std::vector<User> UserSpisok;				 //список зарегистрированных пользователей
	//std::vector<Message> messageList;			 //создаем вектор, который хранит сообщения в чате
	//std::shared_ptr<User> currentUser = nullptr; //указатель указывает на текущего пользователя

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
	void SignIn();														  //вход по логину и паролю
	void ShowMessage();													  //вход по логину и паролю
	bool FindLogin(const std::string& login);							  //проверка логина
	void NewUser();														  //создание нового пользователя
	bool UserSearch(const std::string& login, const std::string& password);//поиск пользователя по логину и паролю
	void PrintNamesUsers();												  //получение списка зарегестрированных пользователей
	int FindUserinUserSpisok(const std::string& name);					  //проверка корректно ли введено имя
	void setPrivateShowChat();											  //чтение личных сообщений
	void setAllShowChat();												  //чтение общих сообщений
	void setAddMessage();												  //добавление сообщения в массив
	std::string idToMessage(std::string& name);								//отправка личных сообщений
	void currrenNull();
};




