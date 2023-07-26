#include <iostream>
#include "Chat_Server.h"
#include "Chat_Client.h"
#include <windows.h>

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	try {
		bool f = true;
		while (f) {
			char choise;
			std::cout << "\n------Введите действиe:------\n1 - Вызов сервера\n2 - Вызов клиента\n0 - Выход\n";
			std::cin >> choise;

			switch (choise) {
			case '1': {
				Chat_Server chatsrv;
				chatsrv.Menu();
				break; }
			case '2': {
				Chat_Client chat;
				chat.runclient();
				break; }
			case '0': {
				std::cout << "Exit" << std::endl;
				f = false;
				break; }
			default: {
				std::cout << "Неверный ввод." << std::endl;
				break; }
			}
		}
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}