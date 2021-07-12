#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "libserver.h"

int login( dbaccess &db, const char *name, const char *pass)
{
    int id=db.get_user_id(name);
    if ( id )
    {
        if ( pass == db.get_user_pass(id) ) return id;
        else return 0;
    }
    return db.reg_user(name,pass);
}

int login_reg( SOCKET s, dbaccess &db )
{
    int len,rc;
    logreg lr;
    rc = recv( s, &len, sizeof(len), 0 );       if ( rc <= 0 ) return 0;
    len = ntohl(len);
    rc = recv( s, &lr.name, len, 0 );           if ( rc <= 0 ) return 0;
    rc = recv( s, &len, sizeof(len), 0 );       if ( rc <= 0 ) return 0;
    len = ntohl(len);
    rc = recv( s, &lr.pass, len, 0 );           if ( rc <= 0 ) return 0;
    len = login( db, lr.name, lr.pass );
    len = htonl(len);
    rc = send( s, &len, sizeof(len), 0 ); if ( rc <= 0 ) return 0;
    len = ntohl(len);
    return len;
}

int select_chat( SOCKET s, dbaccess &db )
{
    int len,rc;
    char name[256];
    rc = recv( s, &len, sizeof(len), 0 );       if ( rc <= 0 ) return 0;
    len = ntohl(len);
    rc = recv( s, &name, len, 0 );              if ( rc <= 0 ) return 0;
    len = db.get_user_id(name);
    len = htonl(len);
    rc = send( s, &len, sizeof(len), 0 ); if ( rc <= 0 ) return 0;
    len = ntohl(len);
    return len;
}

bool get_message( const int &id, SOCKET s, dbaccess &db )
{
    sender_n_message snm;
    int rc;
    rc = recv( s, &snm.sender, sizeof(snm.sender), 0 );
    if ( rc <= 0 )
        return 0;
    rc = recv( s, &snm.number, sizeof(snm.number), 0 );
    if ( rc <= 0 )
        return 0;
    snm.sender = ntohl(snm.sender);
    snm.number = ntohl(snm.number);
    if ( snm.number <= 0 )
    {
        snm = db.get_message( snm.sender, id, (db.get_msg_numbers( snm.sender, id ) ).back() );
    }
    else snm = db.get_message( snm.sender, id, snm.number );
    snm.number = htonl(snm.number);
    rc = send( s, &snm.number, sizeof(snm.number), 0 );
    if ( rc <= 0 )
        return 0;
    snm.sender = htonl(snm.sender);
    rc = send( s, &snm.sender, sizeof(snm.sender), 0 );
    if ( rc <= 0 )
        return 0;
    snm.sender = htonl(snm.message.size());
    rc = send( s, &snm.sender, sizeof(snm.sender), 0 );
    if ( rc <= 0 )
        return 0;
    rc = send( s, snm.message.c_str(), snm.sender, 0 );
    if ( rc <= 0 )
        return 0;
    return 1;
}

int send_message( const int &id, SOCKET s, dbaccess &db )
{
    sender_n_message snm;
    int rc,len;
    rc = recv( s, &snm.sender, sizeof(snm.sender), 0 );     if ( rc <= 0 ) return 0;
    rc = recv( s, &len, sizeof(len), 0 );                   if ( rc <= 0 ) return 0;
    len = ntohl(len);
    rc = recv( s, (void*)snm.message.c_str(), len, 0 );
    cout << snm.message << endl;
    if ( rc <= 0 ) return 0;
    snm.sender = ntohl(snm.sender);
    len = db.add_message( id, snm.sender, snm.message.c_str() );
    snm.number = htonl(len);
    rc = send( s, &snm.number, sizeof(snm.number), 0 );     if ( rc <= 0 ) return 0;
    return len;
}
