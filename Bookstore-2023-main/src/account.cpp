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
    root.Privilege = 7;
    Block<Account> blk("root", root);
    users.Insert(blk);
}

void Account::login(char *ID, char *Passward)
{
    Account user; //空对象
    Block<Account> blk(ID, user); //键插入
    if(!users.ifFind(blk)) //查找键
    {
        return;
    }
    user = users.Find(blk); //查找并填充对象
    //Block<Account> curBlock(ID, user); //操作块
    if(Passward[0] == '\0' && user.getPrivilege() > login_stack.back().Privilege)
    {
        return;
    }
    if(Passward != user.Passward)
    {
        return;
    }
    user.status = true;
    login_stack.push_back(user);
}

void Account::logout()
{
    if(!login_stack.empty())
    {
        login_stack.back().status = false;
        login_stack.pop_back();
    }
}

void Account::signup(char *id, char *passward, char *username)
{
    Account user;
    Block<Account> blk(id, user);
    if(users.ifFind(blk))
    {
        return;
    }
    strncpy(user.ID, id, sizeof(ID));
    strncpy(user.Passward, passward, sizeof(Passward));
    strncpy(user.Username, username, sizeof(Username));
    user.Privilege = 1;
    Block<Account> curBlock(id, user);
    users.Insert(curBlock);
}

void Account::modify(char *ID, char *Cur_Passward, char *New_Passward)
{
    Account user;
    Block<Account> blk(ID, user);
    if(!users.ifFind(blk))
    {
        return;
    }
    user = users.Find(blk);
    if(Cur_Passward[0] == '\0' && user.getPrivilege() != 7)
    {
        return;
    }
    if(New_Passward != user.Passward)
    {
        return;
    }
    Block<Account> curBlock(ID, user);
    users.Delete(curBlock);
    strncpy(user.Passward, New_Passward, sizeof(Passward));
    Block<Account> curBlock(ID, user);
    users.Insert(curBlock);
}

void Account::useradd(char *id, char *passward, int privilege, char *username)
{
    if(login_stack.back().getPrivilege() < 3 || login_stack.back().getPrivilege() < privilege)
    {
        return;
    }
    Account user;
    Block<Account> blk(id, user);
    if(users.ifFind(blk))
    {
        return;
    }
    strncpy(user.ID, id, sizeof(ID));
    strncpy(user.Passward, passward, sizeof(Passward));
    strncpy(user.Username, username, sizeof(Username));
    user.Privilege = privilege;
    Block<Account> curBlock(ID, user);
    users.Insert(curBlock);
}

void Account::del(char *ID)
{
    if(login_stack.back().getPrivilege() != 7)
    {
        return;
    }
    Account user;
    Block<Account> blk(ID, user);
    if(!users.ifFind(blk))
    {
        return;
    }
    for(const auto& obj : login_stack)
    {
        if(strcmp(obj.ID, ID) == 0)
        {
            return;
        }
    }
    user = users.Find(blk);
    Block<Account> curBlock(ID, user);
    users.Delete(curBlock);
}