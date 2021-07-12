#ifndef STRUCTURES
#define STRUCTURES
#include <string>
#include <string.h>
struct sender_n_message //sender, number, message
{
    int sender;
    int number;
    std::string message;
};
enum Packtype   //LOGREG, SNDMSG, SLCTCHAT, GTMSG, INTRTRN
{
    LOGREG,
    SNDMSG,
    SLCTCHAT,
    GTMSG,
    INTRTRN
};
struct logreg   //name, pass
{
    char name[256];
    char pass[256];
};
struct packet
{
    int type;
    char buf[0];
};
struct sndmsg
{
    int num_sndr;
    int sndr_rcpnt;
    char msg[256];
};
struct slctchat
{
    char name[256];
};
struct gtmsg
{
    int uid1;
    int uid2;
    int number;
};
struct intrtrn
{
    int id;
};
#endif