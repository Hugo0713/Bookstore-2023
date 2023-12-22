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

void Account::login(char *id, char *Passward)
{
    Account user;                 // 空对象
    Block<Account> blk(id); // 键插入

    if (!users.ifFind(blk))       // 查找键
    {
        throw std::runtime_error("Invalid\n");
    }
    user = users.Find(blk); // 查找并填充对象
    // Block<Account> curBlock(ID, user); //操作块
    if (Passward[0] == '\0' && user.getPrivilege() >= login_stack.back().Privilege)
    {
        throw std::runtime_error("Invalid\n");
    }
    if (strcmp(Passward, user.Passward) != 0)
    {
        throw std::runtime_error("Invalid\n");
    }
    user.status = true;
    login_stack.push_back(user);
}

void Account::logout()
{
    if (!login_stack.empty())
    {
        login_stack.back().status = false;
        strncpy(login_stack.back().selected, "", sizeof(login_stack.back().selected));
        login_stack.pop_back();
    }
    else
    {
        throw std::runtime_error("Invalid\n");
    }
}

void Account::signup(char *id, char *passward, char *username)
{
    Account user;
    Block<Account> blk(id, user);
    if (users.ifFind(blk))
    {
        throw std::runtime_error("Invalid\n");
    }
    strncpy(user.ID, id, sizeof(ID));
    strncpy(user.Passward, passward, sizeof(Passward));
    strncpy(user.Username, username, sizeof(Username));
    user.Privilege = 1;
    Block<Account> curBlock(id, user);
    users.Insert(curBlock);
}

void Account::modify(char *id, char *Cur_Passward, char *New_Passward)
{
    if (login_stack.empty())
    {
        throw std::runtime_error("Invalid\n");
    }
    Account user;
    Block<Account> blk(id, user);
    if (!users.ifFind(blk))
    {
        throw std::runtime_error("Invalid\n");
    }
    user = users.Find(blk);
    if (New_Passward[0] == '\0' && login_stack.back().getPrivilege() != 7)//若省略cur，传入参数new为空
    {
        throw std::runtime_error("Invalid\n");//省略不合法
    }
    if (New_Passward[0] != '\0' && strcmp(Cur_Passward, user.Passward) != 0)//完全传参，原密码错误
    {
        throw std::runtime_error("Invalid\n");
    }
    Block<Account> curBlock(id, user);
    users.Delete(curBlock);
    if(New_Passward[0] == '\0' && login_stack.back().getPrivilege() == 7)//参数cur为改的密码
    {
        if(strcmp(Cur_Passward, user.Passward) == 0)
        {
            throw std::runtime_error("Invalid\n");
        }
        strncpy(user.Passward, Cur_Passward, sizeof(Passward));
    }
    else
    {
        if(strcmp(New_Passward, user.Passward) == 0)
        {
            throw std::runtime_error("Invalid\n");
        }
        strncpy(user.Passward, New_Passward, sizeof(Passward));
    }
    Block<Account> newBlock(id, user);
    users.Insert(newBlock);
}

void Account::useradd(char *id, char *passward, int privilege, char *username)
{
    if (login_stack.empty() || (login_stack.back().getPrivilege() != 3 && login_stack.back().getPrivilege() != 7) || privilege == 7 || login_stack.back().getPrivilege() <= privilege)
    {
        throw std::runtime_error("Invalid\n");
    }
    Account user;
    Block<Account> blk(id);
    if (users.ifFind(blk))
    {
        throw std::runtime_error("Invalid\n");
    }
    strncpy(user.ID, id, sizeof(ID));
    strncpy(user.Passward, passward, sizeof(Passward));
    strncpy(user.Username, username, sizeof(Username));
    user.Privilege = privilege;
    Block<Account> curBlock(id, user); //将ID改为id，细节决定成败！
    users.Insert(curBlock);
}

void Account::del(char *id)
{
    if (login_stack.empty() || login_stack.back().getPrivilege() != 7)
    {
        throw std::runtime_error("Invalid\n");
    }
    Account user;
    Block<Account> blk(id, user);
    if (!users.ifFind(blk))
    {
        throw std::runtime_error("Invalid\n");
    }
    for (const auto &obj : login_stack)
    {
        if (strcmp(obj.ID, id) == 0)
        {
            throw std::runtime_error("Invalid\n");
        }
    }
    user = users.Find(blk);
    Block<Account> curBlock(id, user);
    users.Delete(curBlock);
}