#include <iostream>
#include <vector>
#include <mysql/mysql.h>
#include "libserver.h"

#define ARGS 3

using namespace std;

int main( int argc, char **argv)
{
    dbaccess db;
    if ( argc == ARGS)
    {
        logreg lr;
        gtmsg gm;
        strcpy(lr.name, argv[1]);
        strcpy(lr.pass, argv[2]);
        gm.uid1=login_reg( db, &lr );
        if ( gm.uid1 )
        {
            slctchat sc;
            cout << "Enter friend name\n";
            cin >> sc.name;
            gm.uid2 = select_chat( db, &sc );
            if ( !gm.uid2 ) cout << "Wrong friend name\n";
            else
            {
                sndmsg sm;
                gm.number=0;
                int lastmsg = (get_message( db, &gm )).num_sndr;
                int i=1;
                while ( i <= lastmsg )
                {
                    gm.number=i;
                    sm=get_message( db, &gm );
                    i++;
                    if (sm.num_sndr==0||sm.sndr_rcpnt==0) continue;
                    if (sm.sndr_rcpnt==gm.uid1) cout << argv[1];
                    else if (sm.sndr_rcpnt==gm.uid2) cout << sc.name;
                    cout << '\t' << sm.num_sndr << '\t' << sm.msg << endl;
                }
            }
        }
        else cout << "wrong name and pass" << endl;
    }
    else if ( argc > ARGS) cout << "Too many arguments!" << endl;
    else cout << "Too few arguments!" << endl;
    gtmsg test;
    sndmsg msg;
    msg.num_sndr=5;
    msg.sndr_rcpnt=4;
    cout << "Enter message\n";
    cin >> msg.msg;
    test.number = send_message(db,&msg);
    if ( test.number > 0 )
    {
        test.uid1=msg.num_sndr;
        test.uid2=msg.sndr_rcpnt;
        msg=get_message(db,&test);
        cout << db.get_user_name(msg.sndr_rcpnt) << '\t' << msg.num_sndr << '\t' << msg.msg << endl;
    }
    db.~dbaccess();
    return 0;
}