#include "Account.hpp"
#include <cstring>

int Account::getPrivilege()
{
    return Privilege;
}

void Account::setroot()
{
    Account root;
    strcpy(root.ID, "root");
    strcpy(root.Passward, "sjtu");
    strcpy(root.Username, "root");
    Block<Account> blk("root", root);
    root.Privilege = 7;
    users.Insert(blk);
}

void Account::login(char *ID, char *Passward)
{
    
}

void Account::logout()
{

}

void Account::signup(char *ID, char *Passward, char *Username)
{

}

void Account::modify(char *ID, char *Cur_Passward, char *New_Passward)
{

}

void Account::useradd(char *ID, char *Passward, int privilege, char *Username)
{

}

void Account::del(char *ID)
{
    
}