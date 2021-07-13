// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <stdarg.h>
// #include <string.h>
// #include <errno.h>
// #include <netdb.h>
// #include <fcntl.h>
// #include <sys/time.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// char *program_name;
// #include "skel.h"
// #include "structures.h"
#include <ncurses.h>
#include <vector>
#define LENGZ 255
// static void client(SOCKET s, struct sockaddr_in *peerp)
// {
//     int rc;
//     int len;
//     char buf[ 256 ];
//     char msg[]="Hello user2";
//     for(;;)
//     {
//     }
// }

int main( int argc, char **argv )
{
    // struct sockaddr_in peer;
    // char *hname;
    // char *sname;
    // SOCKET s;
    // INIT();
    // if ( argc == 2 )
    // {
    //     hname = NULL;
    //     sname = argv[ 1 ];
    // }
    // else
    // {
    //     hname = argv[ 1 ];
    //     sname = argv[ 2 ];
    // }
    // s = tcp_client( hname, sname);
    // client( s, &peer );
    // EXIT( 0 );
    char name[LENGZ+1];
    WINDOW *messages,*text,*minibox,*miniwnd;
    int x,y;
    initscr();
	cbreak();
	curs_set(1);
    refresh();
	getmaxyx( stdscr, y, x );
    if ( y < 24) return 1;
    messages = newwin( y-3, x, 0, 0 );
    box( messages, '|', '-' );
    text = newwin( 3, x, y-3, 0 );
    box( text, '+', '+' );
    minibox = newwin( 7, x/2, (y-7)/2, x/4 );
    box( minibox, '#', '#' );
    miniwnd = derwin( minibox, 5, x/2-2, 1, 1 );
    wrefresh( messages );
    wrefresh ( minibox );
    wrefresh( text );
    wgetnstr( miniwnd, name, LENGZ );
    name[LENGZ] = 0;
    wprintw( miniwnd, name );
    wrefresh( minibox );
    // wrefresh( text );
    getch();
    delwin( messages );
    delwin( text );
    delwin( minibox );
    delwin( miniwnd );
	// nodelay( stdscr, 1 );
	// keypad( stdscr, true );
	// timeout(500);
	
    nocbreak();
	endwin();
    return 0;
}