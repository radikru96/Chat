#include <thread>
#include <iostream>
#include <vector>
#include <mysql/mysql.h>

#define ARGS 3

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define NLISTEN 1
char *program_name;
#include "libserver.h"
#include "../../TCP_IP/skel/skel.h"
using namespace std;
static void server( dbaccess &db, SOCKET s )
{
    int rc,buf,id=0;
    rc = recv( s, &buf, sizeof(buf), 0 );
    if ( !(rc <= 0) ){    
        buf = ntohl(buf);
        if ( buf == LOGREG ){
            id = login_reg( s, db );
            if ( id )
                cout << "login/registration success\n";
            else cout << "login/registration error\n";
        }
        else{
            cout << "packet type error\n";
        }
        while ( id ){
            rc = recv( s, &buf, sizeof(buf), 0 );
            if ( !(rc <= 0) ){    
                buf = ntohl(buf);
                if ( buf == LOGREG ) id = 0;
                if ( buf == SLCTCHAT){
                    if ( select_chat( s, db ) )
                        cout << "select chat success\n";
                    else cout << "select chat error\n";
                }
                else if ( buf == GTMSG ){
                    if ( get_message( id, s, db ) )
                        cout << "get message success\n";
                    else cout << "get message error\n";
                }
                else if ( buf == SNDMSG ){
                    if ( send_message( id, s, db ) )
                        cout << "send message success\n";
                    else cout << "send message error\n";
                }
                else{
                    id = 0;
                }
            }
        }
    }
    else cout << "recv packet type error\n";
    CLOSE( s );
}
int main( int argc, char **argv )
{
    dbaccess db;
    if ( !db.connect() ) return 1;
    struct sockaddr_in peer;
    char *hname;
    char *sname;
    socklen_t peerlen;
    SOCKET s;
    SOCKET s1;
    INIT();
    if ( argc == 2 )
    {
        hname = NULL;
        sname = argv[ 1 ];
    }
    else
    {
        hname = argv[ 1 ];
        sname = argv[ 2 ];
    }
    s = tcp_server( hname, sname);
    vector<thread> th;
    do
    {
        peerlen = sizeof( peer );
        s1 = accept( s, (struct sockaddr * )&peer, &peerlen );
        if ( !isvalidsock( s1 ) )
            error( 1, errno, (char*)"accept call error");
        th.push_back( thread ( [&] () {server(db,s1);} ) );
    } while (1);
    while ( !th.empty() )
    {
        th.back().join();
        th.pop_back();
    }
    db.~dbaccess();
    EXIT( 0 );
}