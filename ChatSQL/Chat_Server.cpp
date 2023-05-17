#include <iostream>
#include<string>
#include "Chat_Server.h"
#include "Sha1.h"

std::shared_ptr<User> Chat_Server::getUserByLogin(const std::string& login) const {			// указатель на логин пользователя
	for (auto& user : UserSpisok) {
		if (login == user.getLogin())
			return std::make_shared<User>(user);
	}
	return nullptr;
}

std::shared_ptr<User> Chat_Server::getUserByName(const std::string& name) const {		// указатель на имя пользователя
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
		sock.send_data((char*)"------Введите действие:------\n1 - вход\n2 - регистрация\n0 - выход\n");
		sock.send_data((char*)"end_receive");
		char* choise = sock.receive_data();
		switch (choise[0]) {
		case '1':
			SignIn();								//вход по логину и паролю
			break;
		case '2':									// регистрация нового пользователя
			try {
				NewUser();					//вызываем метод добавления нового пользователя			
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
			sock.send_data((char*)"Неверный ВВОД!\n");
			sock.send_data((char*)"end_chat");
			f = sock.listen_socket();
			break;
		}
	}
	std::cout << "Chat session terminated! See you again!" << std::endl;
}

void Chat_Server::SignIn() {								//вход по логину и паролю
	std::string log, pas;
	sock.send_data((char*)"----Введите----\nЛогин: ");
	sock.send_data((char*)"end_receive");
	log = sock.receive_data();

	sock.send_data((char*)"Пароль: ");
	sock.send_data((char*)"end_receive");
	pas = sock.receive_data();

	if (UserSearch(log, pas)) {
		bool f1 = true;
		while (f1) {
			sock.send_data((char*)"\n------Введите действиe:------\n1 - Прочитать сообщениe\n2 - Написать сообщение\n0 - Назад\n");

			sock.send_data((char*)"end_receive");
			char* b = sock.receive_data();
			switch (b[0]) {
			case '1':
				if (messageList.empty())
					sock.send_data((char*)"У вас еще нет входящих сообщений!\n");
				else
					ShowMessage();							//прочитать сообщения	
				break;
			case '2':										//написать сообщение
				setAddMessage();
				break;
			case '0':										//назад
				currrenNull();
				f1 = false;
				break;
			default:
				sock.send_data((char*)"Неверный ВВОД!\n");
				break;
			}
		}
	}
	else
		sock.send_data((char*)"\nНеверный логин или пароль!!!\n");
}

void Chat_Server::ShowMessage() {									//прочитать сообщения
	sock.send_data((char*)"\n------Введите действиe:------\n1 - Личные\n2 - Общие\n0 - Назад\n");
	sock.send_data((char*)"end_receive");
	char* d = sock.receive_data();

	switch (d[0]) {
	case '1':
		setPrivateShowChat();   //вызываем метод вывода личных сообщений  
		break;
	case '2':
		setAllShowChat();		 //вызываем метод вывода общих сообщений
	case '0':
		break;
	default:
		sock.send_data((char*)"Неверный ВВОД!\n");
		break;
	}
}

bool Chat_Server::FindName(const std::string& name) {								 //метод проверки имени
	for (std::vector<User>::iterator it = UserSpisok.begin(); it != UserSpisok.end(); ++it) {
		if (it->getName() == name)
			return true;
	}
	return false;
}

bool Chat_Server::FindLogin(const std::string& login) {								//метод проверки логина
	for (std::vector<User>::iterator it = UserSpisok.begin(); it != UserSpisok.end(); ++it) {
		if (it->getLogin() == login)
			return true;
	}
	return false;
}

void Chat_Server::NewUser() {					//метод создания нового пользователя
	std::string name, login, password;

	sock.send_data((char*)"---- Введите данные для регистрации ----\nИмя: ");
	bool n = true;
	while (n) {
		sock.send_data((char*)"end_receive");
		name = sock.receive_data();

		/*if (name == " ") {sock.send_data("Вы не ввели имя"); }*/
		/*std::cin >> name;*/
		if (name == "all" || FindName(name))
			sock.send_data((char*)"Данное имя уже занято выберите другое!\nВведите имя:");
		else n = false;
	}
	bool l = true;
	while (l) {
		sock.send_data((char*)"Логин: ");
		sock.send_data((char*)"end_receive");
		login = sock.receive_data();

		if (login == "all" || FindLogin(login))
			sock.send_data((char*)"Данный логин уже занят выберите другой!\nВведите логин: ");
		else l = false;

	}
	sock.send_data((char*)"Пароль: ");
	sock.send_data((char*)"end_receive");
	password = sock.receive_data();

	User user(name, login, sha1((char*)password.data(), password.length()));				//создаем объект класа User
	UserSpisok.push_back(user);						// добавляем пользователя в массив
	currentUser = std::make_shared<User>(user);		//создаем указатель на текущего пользователя
	sock.send_data((char*)"Пользователь зарегистрирован!\n\n");
}

bool Chat_Server::UserSearch(const std::string& login, const std::string& password) {	//метод поиска пользователя по логину и паролю	
	int i = 0;
	while (i < UserSpisok.size()) {
		for (auto& user : UserSpisok) {
			if (login == user.getLogin() && memcmp(user.getPassword(), sha1((char*)password.data(), password.length()), SHA1HASHLENGTHBYTES) == 0) {
				currentUser = getUserByLogin(login);   //указатель на текущего пользователя

				std::string cur = currentUser->getName();
				char* c = const_cast<char*>(cur.c_str());

				sock.send_data((char*)"\nПользователь: ");
				sock.send_data(c);
				return true;
			}
			else { ++i; }
		}
	}
	return false;
}

void Chat_Server::PrintNamesUsers() {				    //метод получения списка зарегестрированных пользователей
	for (std::vector<User>::iterator it = UserSpisok.begin(); it != UserSpisok.end(); ++it) {
		std::string str = it->getName();
		char* c = const_cast<char*>(str.c_str());
		sock.send_data(c);
		sock.send_data((char*)"\n");
	}
}

int Chat_Server::FindUserinUserSpisok(const std::string& name) {	//метод проверяет корректно ли введено имя
	for (std::vector<User>::iterator it = UserSpisok.begin(); it != UserSpisok.end(); ++it) {
		if (it->getName() == name)
			return 0;
	}
	return -1;
}

void Chat_Server::setPrivateShowChat() {				//метод чтения личных сообщений
	std::string from;
	std::string to;

	sock.send_data((char*)"--------------ЧАТ--------------\n");
	for (auto& message : messageList) {
		if (currentUser->getName() == message.getFromMessage() || currentUser->getName() == message.getToMessage()) {//если текущий пользователь
			from = (currentUser->getName() == message.getFromMessage()) ? "Меня" : message.getFromMessage();

			to = (currentUser->getName() == message.getToMessage()) ? "Мне" : message.getToMessage();
			//если текущее имя равно to, то отправляем сообщение самому себе, если нет, то получаем имя пользователя и присваиваем его значение полю to

			if (message.getToMessage() != "all") {
				sock.send_data((char*)"от ");
				std::string strf = from;
				char* fr = const_cast<char*>(strf.c_str());
				sock.send_data(fr);
				sock.send_data((char*)" кому ");
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

void Chat_Server::setAllShowChat() {							// метод чтения общих сообщений
	std::string from;

	sock.send_data((char*)"-----------ОБЩИЙ ЧАТ-----------\n");
	for (auto& message : messageList) {
		if (currentUser->getName() == message.getFromMessage() || currentUser->getName() == message.getToMessage() || message.getToMessage() == "all") {//если текущий пользователь
			from = (currentUser->getName() == message.getFromMessage()) ? "Меня" : message.getFromMessage();
			if (message.getToMessage() == "all") { 						//сообщение всем пользователям
				sock.send_data((char*)"от ");
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

void Chat_Server::setAddMessage() {						    	//метод добавления сообщения в массив
	std::string inputName;
	std::string message;
	sock.send_data((char*)"\nВведите имя кому отправить сообщение:\n");
	PrintNamesUsers();									  //выводим список пользователей
	sock.send_data((char*)"all - отправить всем\n");
	sock.send_data((char*)"end_receive");
	inputName = sock.receive_data();

	if (inputName == "all") {							  //отправка всем пользователям
		sock.send_data((char*)"\nВведите текст сообщения: \n");
		sock.send_data((char*)"end_receive");
		message = sock.receive_data();
		messageList.push_back(Message(currentUser->getName(), "all", message));
		sock.send_data((char*)"Сообщение разослано всем пользователям!\n");
	}
	else {													//отправка личных сообщений
		int t = -1;
		t = FindUserinUserSpisok(inputName);
		if (t == -1) {
			sock.send_data((char*)"Пользователь с данным именем не найден\n");
		}
		else {
			sock.send_data((char*)"Введите текст сообщения: \n");
			sock.send_data((char*)"end_receive");
			message = sock.receive_data();
			messageList.push_back(Message(currentUser->getName(), inputName, message));
		}
	}
}

void Chat_Server::currrenNull() {
	currentUser = nullptr;
}

