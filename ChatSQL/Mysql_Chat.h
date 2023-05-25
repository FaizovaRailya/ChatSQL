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
    MYSQL mysql; // ���������� ���������� c MySql
    MYSQL_RES* res;
    MYSQL_ROW row;
    Mysql_Chat() = default;

    ~Mysql_Chat() = default;
    /*mysql_api(mysql_api& _arr) = delete;
    mysql_api& operator=(mysql_api& _list) = delete;*/

    std::string msg_data;
    void mysql_start();
};
