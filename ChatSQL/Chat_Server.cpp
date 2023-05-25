#include <iostream>
#include<string>
#include<string.h>
#include "Chat_Server.h"
#include "Sha1.h"
#include <ctime>
#include <chrono>
#pragma warning(disable : 4996)

std::shared_ptr<User> Chat_Server::getUserByLogin(const std::string& login) const {			// указатель на логин пользователя
	/*for (auto& user : UserSpisok) {
		if (login == user.getLogin())
			return std::make_shared<User>(user);
	}
	return nullptr;*/
}

std::shared_ptr<User> Chat_Server::getUserByName(const std::string& name) const {		// указатель на имя пользователя
	/*for (auto& user : UserSpisok) {
		if (name == user.getName())
			return std::make_shared<User>(user);
	}
	return nullptr;*/
}

char* Chat_Server::getCurrentUser() const {
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
				//if (messageList.empty())
				//	sock.send_data((char*)"У вас еще нет входящих сообщений!\n");
				//else
				//	ShowMessage();							//прочитать сообщения	
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



bool Chat_Server::FindLogin(const std::string& login) {								//метод проверки логина
	db.mysql_start();
	std::string log("SELECT login FROM user_spisok WHERE login = \"" + login + "\"");
	mysql_query(&db.mysql, log.c_str()); //Делаем запрос к таблице

	if (db.res = mysql_store_result(&db.mysql)) {
		if (mysql_num_rows(db.res) == 0) //cout << "нет логина" << endl;
			return false;
	}
	return true;
	mysql_close(&db.mysql);
}

void Chat_Server::NewUser() {					//метод создания нового пользователя
	std::string name, login, password;

	sock.send_data((char*)"---- Введите данные для регистрации ----\nИмя: ");
	bool n = true;
	while (n) {
		sock.send_data((char*)"end_receive");
		name = (char*)sock.receive_data();
		std::cout << name << std::endl;
		if (name == "all")
			sock.send_data((char*)"Данное имя уже занято выберите другое!\nВведите имя: ");
		else n = false;
	}
	bool l = true;
	while (l) {
		sock.send_data((char*)"Логин: ");
		sock.send_data((char*)"end_receive");
		login = sock.receive_data();
		std::cout << login << std::endl;
		if (login == "all" || FindLogin(login))
			sock.send_data((char*)"Данный логин уже занят выберите другой!\nВведите логин: ");
		else l = false;

	}
	sock.send_data((char*)"Пароль: ");
	sock.send_data((char*)"end_receive");
	password = sock.receive_data();
	
	db.mysql_start();
	unsigned int* _password = new unsigned int[5];
	std::string _pw_str[5];
	_password = sha1((char*)(password.data()), password.length());
	for (size_t i = 0; i < 5; i++)	{
		_pw_str[i] = std::to_string(_password[i]);
	}
	
	std::string pass(_pw_str[0] + " " + _pw_str[1].c_str() + " " + _pw_str[2] + " " + _pw_str[3] + " " + _pw_str[4]);
	std::string usr_query("INSERT INTO user_spisok(id, name, login, password) VALUES (default, \"" + name + "\", \"" + login + "\", \"" + pass + "\")");
	mysql_query(&db.mysql, usr_query.c_str());
	sock.send_data((char*)"Пользователь зарегистрирован!\n\n");

	mysql_close(&db.mysql);
}

bool Chat_Server::UserSearch(const std::string& login, const std::string& password) {	//метод поиска пользователя по логину и паролю	
	db.mysql_start();
	std::string log("SELECT * FROM user_spisok WHERE login = \"" + login + "\"");
	mysql_query(&db.mysql, log.c_str()); //Делаем запрос к таблице

	unsigned int* _password = new unsigned int[5];
	std::string _pw_str[5];
	_password = sha1((char*)(password.data()), password.length());
	for (size_t i = 0; i < 5; i++){
		_pw_str[i] = std::to_string(_password[i]);
	}
	std::string pass(_pw_str[0] + " " + _pw_str[1].c_str() + " " + _pw_str[2] + " " + _pw_str[3] + " " + _pw_str[4]);

	if (db.res = mysql_store_result(&db.mysql)) {
		while (db.row = mysql_fetch_row(db.res)) {
			for (int i = 0; i < mysql_num_fields(db.res); i++) {
				if (db.row[3] == pass) {
					currentUser = db.row[1];  //указатель на текущего пользователя
					id_currentUser = db.row[0];
					sock.send_data((char*)"\nПользователь: ");
					sock.send_data((char*)currentUser);
					return true;
				}
			}
		}
	}
		return false;
		mysql_close(&db.mysql);
}

void Chat_Server::PrintNamesUsers() {				    //метод получения списка зарегестрированных пользователей
	db.mysql_start();
	mysql_query(&db.mysql, "SELECT name FROM user_spisok"); //Делаем запрос к таблице

	//Выводим все что есть в базе через цикл
	if (db.res = mysql_store_result(&db.mysql)){
		while (db.row = mysql_fetch_row(db.res)){				
			for (size_t i = 0; i < mysql_num_fields(db.res); i++){
				sock.send_data(db.row[i]);
				}
			sock.send_data((char*)"\n");
			}
	}
	mysql_close(&db.mysql);
}

int Chat_Server::FindUserinUserSpisok(const std::string& name) {	//метод проверяет корректно ли введено имя
	/*for (std::vector<User>::iterator it = UserSpisok.begin(); it != UserSpisok.end(); ++it) {
		if (it->getName() == name)
			return 0;
	}
	return -1;*/
}

void Chat_Server::setPrivateShowChat() {				//метод чтения личных сообщений
	//std::string from;
	//std::string to;

	//sock.send_data((char*)"--------------ЧАТ--------------\n");
	//for (auto& message : messageList) {
	//	if (currentUser->getName() == message.getFromMessage() || currentUser->getName() == message.getToMessage()) {//если текущий пользователь
	//		from = (currentUser->getName() == message.getFromMessage()) ? "Меня" : message.getFromMessage();

	//		to = (currentUser->getName() == message.getToMessage()) ? "Мне" : message.getToMessage();
	//		//если текущее имя равно to, то отправляем сообщение самому себе, если нет, то получаем имя пользователя и присваиваем его значение полю to

	//		if (message.getToMessage() != "all") {
	//			sock.send_data((char*)"от ");
	//			std::string strf = from;
	//			char* fr = const_cast<char*>(strf.c_str());
	//			sock.send_data(fr);
	//			sock.send_data((char*)" кому ");
	//			std::string strt = to;
	//			char* t = const_cast<char*>(strt.c_str());
	//			sock.send_data(t);
	//			sock.send_data((char*)": ");
	//			std::string strm = message.getText();
	//			char* mes = const_cast<char*>(strm.c_str());
	//			sock.send_data(mes);
	//			sock.send_data((char*)"\n");
	//		}
	//	}
	//}
	//sock.send_data((char*)"-------------------------------");
}

void Chat_Server::setAllShowChat() {							// метод чтения общих сообщений
	//std::string from;

	//sock.send_data((char*)"-----------ОБЩИЙ ЧАТ-----------\n");
	//for (auto& message : messageList) {
	//	if (currentUser->getName() == message.getFromMessage() || currentUser->getName() == message.getToMessage() || message.getToMessage() == "all") {//если текущий пользователь
	//		from = (currentUser->getName() == message.getFromMessage()) ? "Меня" : message.getFromMessage();
	//		if (message.getToMessage() == "all") { 						//сообщение всем пользователям
	//			sock.send_data((char*)"от ");
	//			std::string strf = from;
	//			char* fr = const_cast<char*>(strf.c_str());
	//			sock.send_data(fr);
	//			sock.send_data((char*)": ");
	//			std::string strm = message.getText();
	//			char* mes = const_cast<char*>(strm.c_str());
	//			sock.send_data(mes);
	//			sock.send_data((char*)"\n");
	//		}
	//	}
	//}
	//sock.send_data((char*)"-------------------------------\n");
}

void Chat_Server::setAddMessage() {						    	//метод добавления сообщения в массив
	std::string id_from, id_to;
	std::chrono::system_clock::time_point value_t = std::chrono::system_clock::now();
	time_t timestamp = std::chrono::system_clock::to_time_t(value_t);
	const tm* ltm = std::localtime(&timestamp);

	char mbstr[15];
	std::strftime(mbstr, 15, "%Y-%m-%d", ltm);
	std::cout << mbstr << std::endl;

	sock.send_data((char*)"\nВведите имя кому отправить сообщение:\n");
	PrintNamesUsers();									  //выводим список пользователей
	sock.send_data((char*)"all - отправить всем\n");
	sock.send_data((char*)"end_receive");
	std::string inputName = sock.receive_data();
	std::string from = currentUser;
	//std::string to = inputName;

	sock.send_data((char*)"\nВведите текст сообщения: \n");
		sock.send_data((char*)"end_receive");
		std::string text = sock.receive_data();
	db.mysql_start();
std:string id_f("SELECT id FROM user_spisok WHERE name = \"" + from + "\" ");
		mysql_query(&db.mysql, id_f.c_str()); //Делаем запрос к таблице
		if (db.res = mysql_store_result(&db.mysql)) {
			while (db.row = mysql_fetch_row(db.res)) {
				for (size_t i = 0; i < mysql_num_fields(db.res); i++) {
					id_from = db.row[i];
				}
			}
		}

	if (inputName == "all") {							  //отправка всем пользователям
		
	
		mysql_query(&db.mysql, "SELECT * FROM message"); //Делаем запрос к таблице
		std::string mes("INSERT INTO message(id, id_from_name, id_to_name, data, status, text) VALUES (default, \""+id_from+"\", '3', \""+ mbstr+"\", '0', \""+text+"\")");
		mysql_query(&db.mysql, mes.c_str());
	}
	else {
	//std:string id_t("SELECT id FROM user_spisok WHERE name = \"" + inputName + "\" ");
	//	mysql_query(&db.mysql, id_t.c_str()); //Делаем запрос к таблице
	//	if (db.res = mysql_store_result(&db.mysql)) {
	//		while (db.row = mysql_fetch_row(db.res)) {
	//			for (size_t i = 0; i < mysql_num_fields(db.res); i++) {
	//				id_to = db.row[i];
	//			}
	//		}
	//	}
	
		id_to = idToMessage(inputName);
		
		mysql_query(&db.mysql, "SELECT * FROM message"); //Делаем запрос к таблице
		std::string mes("INSERT INTO message(id, id_from_name, id_to_name, data, status, text) VALUES (default, \""+id_from+"\", \""+id_to+"\", \""+mbstr+"\", '0', \""+text+"\")");
		std::cout << mes << std::endl;
		mysql_query(&db.mysql, mes.c_str());
	}
		mysql_close(&db.mysql);
		//	messageList.push_back(Message(currentUser->getName(), "all", message, timestamp));
		//	sock.send_data((char*)"Сообщение разослано всем пользователям!\n");
		//}
		//else {													//отправка личных сообщений
		//	int t = -1;
		//	t = FindUserinUserSpisok(inputName);
		//	if (t == -1) {
		//		sock.send_data((char*)"Пользователь с данным именем не найден\n");
		//	}
		//	else {
		//		sock.send_data((char*)"Введите текст сообщения: \n");
		//		sock.send_data((char*)"end_receive");
		//		message = sock.receive_data();
		//		messageList.push_back(Message(currentUser->getName(), inputName, message,timestamp));
		//	}
		//}
	
}

std::string Chat_Server::idToMessage(std::string& name)
{
	//std::string id_to;

std:string id_t("SELECT id FROM user_spisok WHERE name = \"" + name + "\" ");
	mysql_query(&db.mysql, id_t.c_str()); //Делаем запрос к таблице
	if (db.res = mysql_store_result(&db.mysql)) {
		while (db.row = mysql_fetch_row(db.res)) {
			for (size_t i = 0; i < mysql_num_fields(db.res); i++) {
				 char* id_to = db.row[i];
return id_to;

			}
		}
	}
	
}

void Chat_Server::currrenNull() {
	currentUser = nullptr;
}

