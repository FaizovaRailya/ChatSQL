#include "Chat_Client.h"
#include <string>
#include <iostream>

void Chat_Client::runclient() {
    std::string message;
    while (1) {
        while (1) {
            message = sock.receive_data();
            if (message.length() > 7 && (message.substr(message.length() - 8, message.length()) == "end_chat")) {
                std::cout << message.substr(0, message.length() - 8);
                break;
            }
            else if (message.length() > 10 && (message.substr(message.length() - 11, message.length()) == "end_receive")) {
                std::cout << message.substr(0, message.length() - 11);
                break;
            }
            else {
                std::cout << message;
            }
        }
        if (message.length() > 7 && (message.substr(message.length() - 8, message.length()) == "end_chat")) {
            break;
        }
        while (getline(std::cin, message)) {
            if (message != "") break;
        }
        sock.send_data((char*)message.data());
    }
    std::cout << "end of cycle" << std::endl;
}