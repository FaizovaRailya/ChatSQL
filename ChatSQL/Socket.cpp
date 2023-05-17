#include "Socket.h"

Socket_server::~Socket_server()
{
    closesocket(socket_id);
    WSACleanup();
}


Socket_server::Socket_server() {                                // Создадим сокет
    tcpserveraddress.sin_addr.s_addr = htonl(INADDR_ANY);       // Установим адрес сервера
    tcpserveraddress.sin_port = htons(PORT);                    // Зададим номер порта
    tcpserveraddress.sin_family = AF_INET;                      // Используем IPv4

    WSADATA wsaData;
    int result_code = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result_code != NO_ERROR) {
        std::cout << "Winsock Initialize failed with error: " << result_code << std::endl;   //111Не удалось инициализировать Winsock с ошибкой11111111
        return;
    }
    socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_id == INVALID_SOCKET) {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }
    result_code = bind(socket_id, (SOCKADDR*)&tcpserveraddress, sizeof(tcpserveraddress));
    if (result_code != NO_ERROR) {
        std::cout << "Binding error: " << result_code << std::endl;
        exit(1);
    }
    listen_socket();
}

bool Socket_server::listen_socket() {               // Поставим сервер на прием данных
    int result_code = listen(socket_id, 5);
    if (result_code != NO_ERROR) {
        std::cout << "Listening error: " << result_code;
        exit(1);
    }
    else {
        std::cout << "Server is listening for new connection..." << std::endl;
    }
    socklen_t length = sizeof(tcpclient);
    accept_socket = accept(socket_id, (struct sockaddr*)&tcpclient, &length);
    if (accept_socket == INVALID_SOCKET) {
        std::cout << "accept failed with error: " << WSAGetLastError() << std::endl;
        closesocket(socket_id);
        WSACleanup();
        return false;
    }
    std::cout << "Client connected.\n";
    return true;
}

char* Socket_server::receive_data() {                   //получение данных на сервер
    int result = recv(accept_socket, message, MESSAGE_LENGTH, 0);
    if (result > 0) {
        message[result] = '\0';
    }
    else if (result == 0) {
        std::cout << "Connection closed\n";
        return (char*)"end_chat";
    }
    else {
        std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
        return (char*)"end_chat";
    }
    if (!strncmp("end_chat", message, 8)) {
        std::cout << "Client Exited." << std::endl;
        std::cout << "Server is Exiting..!" << std::endl;
        return (char*)"end_chat";
    }
    return message;
}

int Socket_server::send_data(char* msg) {               //посылаем данные из сервера
    memset((message), '\0', (MESSAGE_LENGTH)), (void)0;
    strcpy_s(message, msg);
    int result = send(accept_socket, message, (int)strlen(message), 0);

    if (result == SOCKET_ERROR) {
        std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(accept_socket);
        WSACleanup();
        return result;
    }
    if (strncmp("end_receive", message, 11) && strncmp("end_chat", message, 8)) {
        std::cout << ">>> " << message << std::endl;
    }
    return result;
}

Socket_client::Socket_client() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != NO_ERROR) {
        std::cout << "Winsock Initialize failed with error : " << result << std::endl;
        return;
    }
    socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_id == INVALID_SOCKET) {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }
    inet_pton(AF_INET, "127.0.0.1", &tcpserveraddress.sin_addr.s_addr);
    tcpserveraddress.sin_port = htons(PORT);
    tcpserveraddress.sin_family = AF_INET;

    result = connect(socket_id, (struct sockaddr*)&tcpserveraddress, sizeof(tcpserveraddress));
    if (result != NO_ERROR) {
        std::cout << "Connection error: " << result;
        exit(1);
    }
}

Socket_client::~Socket_client() {
    closesocket(socket_id);
    WSACleanup();
}


char* Socket_client::receive_data() {
    int result = recv(socket_id, message, MESSAGE_LENGTH, 0);
    if (result > 0) {
        message[result] = '\0';
    }
    else if (result == 0) {
        std::cout << "Connection closed\n";
        return (char*)"end_chat";
    }
    else {
        std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
        return (char*)"end_chat";
    }

    if (!strncmp("end_chat", message, 8)) {
        std::cout << "Client Exited." << std::endl;
        std::cout << "Server is Exiting..!" << std::endl;
    }
    return message;
}
int Socket_client::send_data(char* msg) {
    memset((message), '\0', (MESSAGE_LENGTH)), (void)0; //bzero(message2, MESSAGE_LENGTH);
    strcpy_s(message, msg);
    int result = send(socket_id, message, (int)strlen(message), 0);
    if (result == SOCKET_ERROR) {
        std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(socket_id);
        WSACleanup();
    }
    return result;
}
