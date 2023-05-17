#pragma once
#include "Socket.h"

class Chat_Client {
private:
    Socket_client sock;

public:
    Chat_Client() = default;
    virtual ~Chat_Client() = default;
    Chat_Client(Chat_Client& _arr) = delete;
    Chat_Client& operator=(Chat_Client& _list) = delete;

    void runclient();
};

