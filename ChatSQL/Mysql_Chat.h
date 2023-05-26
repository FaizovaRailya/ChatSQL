#pragma once
#include "mysql.h"
#include <string>
#define db_address "localhost"
#define db_login "root"
#define db_pw "root"
#define db_name "dbchat"

class Mysql_Chat{
private:

public:
    MYSQL mysql; // Дескриптор соединения c MySql
    MYSQL_RES* res;
    MYSQL_ROW row;
    Mysql_Chat() = default;
    ~Mysql_Chat() = default;

    std::string msg_data;
    void mysql_start();
};
