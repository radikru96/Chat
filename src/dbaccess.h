#ifndef DBACCESS
#define DBACCESS

#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include "structures.h"
using namespace std;

class dbaccess
{
private:
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    mutex mute;
public:
    dbaccess();
    ~dbaccess();
    bool connect();
    int get_user_id( const char *name );
    string get_user_name( const int &uid );
    string get_user_pass( const int &uid );
    vector<int> get_msg_numbers( const int &uid1, const int &uid2 );
    sender_n_message get_message( const int &uid1, const int &uid2, const int &msgnum );
    int add_message( const int &sender, const int &recipient, const char *message );
    int reg_user( const char *name, const char *pass );
};

#endif