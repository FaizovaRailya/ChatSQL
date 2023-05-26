#include "Mysql_Chat.h"
#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

void Mysql_Chat::mysql_start(){
    mysql_init(&mysql); // Получаем дескриптор соединения

    if (&mysql == NULL) {       // Если дескриптор не получен — выводим сообщение об ошибке
        std::cout << "Error: can't create MySQL-descriptor" << std::endl;
    }

    if (!mysql_real_connect(&mysql, db_address, db_login, db_pw, db_name, 0, NULL, 0)) {        // Подключаемся к серверу
        std::cout << "Error: can't connect to database " << mysql_error(&mysql) << std::endl;
    }
    else {
        std::cout << "Success!" << std::endl;
    }

    mysql_set_character_set(&mysql, "cp1251");
    std::cout << "connection characterset: " << mysql_character_set_name(&mysql) << std::endl;
}
