#ifndef LIBSERVER
#define LIBSERVER

typedef int SOCKET;

#include "dbaccess.h"
#include "structures.h"
#include <sys/socket.h>
// #include "../../TCP_IP/skel/skel.h"

int login( dbaccess &db, const char *name, const char *pass);

int login_reg( SOCKET s, dbaccess &db );

int select_chat( SOCKET s, dbaccess &db );

bool get_message( const int &id, SOCKET s, dbaccess &db );

int send_message( const int &id, SOCKET s, dbaccess &db );

#endif