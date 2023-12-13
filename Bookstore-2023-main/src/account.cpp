#include "account.hpp"
#include <cstring>

int account::getPrivilege()
{
    return Privilege;
}

void account::setroot()
{
    Block blk;
    account root;
    strcpy(root.ID, "root");
    strcpy(root.Passward, "sjtu");
    strcpy(root.Username, "root");
    root.Privilege = 7;
    blk.initialise("root", root);
    insert_(blk);
}

void account::login(char *ID, char *Passward)
{
    
}

void account::logout()
{

}

void account::signup(char *ID, char *Passward, char *Username)
{

}

void account::modify(char *ID, char *Cur_Passward, char *New_Passward)
{

}

void account::useradd(char *ID, char *Passward, int privilege, char *Username)
{

}

void account::del(char *ID)
{
    
}