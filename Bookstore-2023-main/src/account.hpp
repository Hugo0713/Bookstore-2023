#ifndef CODE_ACCOUNT_HPP
#define CODE_ACCOUNT_HPP

#include <iostream>
//#include"Book.hpp"
#include"/mnt/x/VS code/big work/Bookstore-2023-main/Bookstore-2023-main/Database/Database.hpp"


class Account
{
private:
    char ID[31];
    char Passward[31];
    char Username[31];
    int Privilege;
    bool status = false;

public:
    char selected[21]; //选中书的ISBN
    //Book *selected;
    Account() :ID{}, Passward{}, Username{}, Privilege(0), status(false), selected() {}
    Account(char *id) : Passward{}, Username{}, Privilege(0), status(false)
    {
        strncpy(ID, id, sizeof(ID));
    }
    
    bool operator==(const Account &obj) const 
    {
        if(ID == obj.ID && Passward == obj.Passward && Username == obj.Username && Privilege == obj.Privilege)
        {
            return true;
        }
        return false;
    }
    bool operator<(const Account &obj) const 
    {
        if(Privilege != obj.Privilege)
        {
            return ID < obj.ID;
        }
        return Privilege < obj.Privilege;
    }
    Account& operator=(const Account &obj) 
    {
        if (this == &obj)
        {
            return *this;
        }
        strncpy(ID, obj.ID, sizeof(ID));
        strncpy(Passward, obj.Passward, sizeof(Passward));
        strncpy(Username, obj.Username, sizeof(Username));
        Privilege = obj.Privilege;
        status = obj.status;
        strncpy(selected, obj.selected, sizeof(selected));
        return *this;
    }

    int getPrivilege();
    void setroot();
    void login(char *ID, char *Passward);
    void logout();
    void signup(char *ID, char *Passward, char *Username);
    void modify(char *ID, char *Cur_Passward, char *New_Passward);
    void useradd(char *ID, char *Passward, int privilege, char *Username);
    void del(char *ID);
};
static Database<Account> users("File_Account");
static std::vector<Account> login_stack;


#endif