#include "dbaccess.h"

#define HOST "localhost"
#define PORT 3306
#define USER "user"
#define PASS "123"
#define DATABASE "chat_messages_db"

dbaccess::dbaccess()
{
    if ( !mysql_init( &mysql ) ) cout << "Init error\n";
}

dbaccess::~dbaccess()
{
    mysql_close( &mysql );
}

bool dbaccess::connect()
{
    if ( !mysql_real_connect(&mysql, HOST, USER, PASS, DATABASE, PORT, NULL, 0 ) && mysql_select_db( &mysql, DATABASE ) )
        cout << "Connection error - " << mysql_error(&mysql) << endl;
    else return 1;
    return 0;
}

int dbaccess::get_user_id( const char *name )
{
    string command="SELECT id FROM user WHERE name='";
    command+=name;
    command+="'";
    int rtrn;
    mute.lock();
    if ( mysql_query( &mysql, command.c_str() ) )
    {
        cout << "select return error" << endl;
        mute.unlock();
        return 0;
    }
    if ( !( res = mysql_store_result( &mysql ) ) )
    {
        cout << "res is NULL" << endl;
        mute.unlock();
        return 0;
    }
    if (( row = mysql_fetch_row( res ) ))
    {
        rtrn = atoi( row[0] );
        mute.unlock();
        return rtrn;
    }
    mute.unlock();
    return 0;
}

string dbaccess::get_user_name( const int &uid )
{
    string str_val="SELECT name FROM user WHERE id=";
    str_val+=to_string(uid);
    mute.lock();
    if ( mysql_query( &mysql, str_val.c_str() ) )
    {
        str_val.clear();
        cout << "select return error" << endl;
        mute.unlock();
        return str_val;
    }
    str_val.clear();
    if ( !( res = mysql_store_result( &mysql ) ) )
    {
        cout << "res is NULL" << endl;
        mute.unlock();
        return str_val;
    }
    if (( row = mysql_fetch_row( res ) ))
    {
        str_val = row[0];
        mute.unlock();
        return str_val;
    }
    mute.unlock();
    return str_val;
}

string dbaccess::get_user_pass( const int &uid )
{
    string str_val="SELECT pass FROM user WHERE id=";
    str_val+=to_string(uid);
    mute.lock();
    if ( mysql_query( &mysql, str_val.c_str() ) )
    {
        str_val.clear();
        cout << "select return error" << endl;
        mute.unlock();
        return str_val;
    }
    str_val.clear();
    if ( !( res = mysql_store_result( &mysql ) ) )
    {
        cout << "res is NULL" << endl;
        mute.unlock();
        return str_val;
    }
    if (( row = mysql_fetch_row( res ) ))
    {
        str_val = row[0];
        mute.unlock();
        return str_val;
    }
    mute.unlock();
    return str_val;
}

vector<int> dbaccess::get_msg_numbers( const int &uid1, const int &uid2 )
{
    vector<int> numbers;
    string command="SELECT number FROM message WHERE sender=";
    command+=to_string(uid1);
    command+=" AND recipient=";
    command+=to_string(uid2);
    command+=" OR sender=";
    command+=to_string(uid2);
    command+=" AND recipient=";
    command+=to_string(uid1);
    mute.lock();
    if ( mysql_query( &mysql, command.c_str() ) )
    {
        cout << "select return error" << endl;
        mute.unlock();
        return numbers;
    }
    if ( !( res = mysql_store_result( &mysql ) ) )
    {
        cout << "res is NULL" << endl;
        mute.unlock();
        return numbers;
    }
    while ( ( row = mysql_fetch_row( res ) ) )
    {
        numbers.push_back( atoi(row[0]) );
    }
    mute.unlock();
    return numbers;
}

sender_n_message dbaccess::get_message( const int &uid1, const int &uid2, const int &msgnum )
{
    sender_n_message val;
    val.message="SELECT sender, text FROM message WHERE sender=";
    val.message+=to_string(uid1);
    val.message+=" AND recipient=";
    val.message+=to_string(uid2);
    val.message+=" AND number=";
    val.message+=to_string(msgnum);
    val.message+=" OR sender=";
    val.message+=to_string(uid2);
    val.message+=" AND recipient=";
    val.message+=to_string(uid1);
    val.message+=" AND number=";
    val.message+=to_string(msgnum);
    val.number=0;
    val.sender=0;
    mute.lock();
    if ( mysql_query( &mysql, val.message.c_str() ) )
    {
        cout << "get_message select return error" << endl;
        val.message.clear();
        mute.unlock();
        return val;
    }
    val.message.clear();
    if ( !( res = mysql_store_result( &mysql ) ) )
    {
        cout << "res is NULL" << endl;
        mute.unlock();
        return val;
    }
    if ( ( row = mysql_fetch_row( res ) ) )
    {
        val.sender=atoi(row[0]);
        val.message=row[1];
        mute.unlock();
        val.number=msgnum;
        return val;
    }
    mute.unlock();
    return val;
}

int dbaccess::add_message( const int &sender, const int &recipient, const char *message )
{
    string command="INSERT message(number,sender,recipient,text) VALUES (";
    vector<int> msg_num=get_msg_numbers( sender, recipient );
    if ( msg_num.size() ) msg_num.back()++;
    else
    {
        msg_num.push_back(1);
    }
    command+=to_string( msg_num.back() );
    command+=",";
    command+=to_string(sender);
    command+=",";
    command+=to_string(recipient);
    command+=",'";
    command+=message;
    command+="')";
    mute.lock();
    if ( mysql_query( &mysql, command.c_str() ) )
    {
        cout << "INSERT return error" << endl;
        mute.unlock();
        return 0;
    }
    else
    {
        mute.unlock();
        return msg_num.back();
    }
}

int dbaccess::reg_user( const char *name, const char *pass )
{
    string command="INSERT user(name,pass) VALUES ('";
    command+=name;
    command+="','";
    command+=pass;
    command+="')";
    mute.lock();
    if ( mysql_query( &mysql, command.c_str() ) )
    {
        cout << "INSERT return error" << endl;
        mute.unlock();
        return 0;
    }
    else
    {
        mute.unlock();
        return get_user_id(name);
    }
}
