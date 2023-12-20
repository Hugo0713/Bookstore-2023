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
    Account user;                 // 空对象
    Block<Account> blk(ID, user); // 键插入
    if (!users.ifFind(blk))       // 查找键
    {
        throw std::runtime_error("Invalidab\n");
    }
    user = users.Find(blk); // 查找并填充对象
    // Block<Account> curBlock(ID, user); //操作块
    if (Passward[0] == '\0' && user.getPrivilege() > login_stack.back().Privilege)
    {
        throw std::runtime_error("Invalidtt\n");
    }
    if (strcmp(Passward, user.Passward) != 0)
    {
        throw std::runtime_error("Invalidhh\n");
    }
    user.status = true;
    login_stack.push_back(user);
}

void Account::logout()
{
    if (!login_stack.empty())
    {
        login_stack.back().status = false;
        // Book book;
        // strncpy(login_stack.back().selected, book.getISBN(), sizeof(login_stack.back().selected)); // 选择为空
        // login_stack.back().selected.ifselected = false;
        login_stack.pop_back();
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
    if(users.ifFind(curBlock))
    {
        std::cout <<"shsh";
    }
}

void Account::modify(char *ID, char *Cur_Passward, char *New_Passward)
{
    if (login_stack.empty())
    {
        throw std::runtime_error("Invalid\n");
    }
    Account user;
    Block<Account> blk(ID, user);
    if (!users.ifFind(blk))
    {
        throw std::runtime_error("Invalid\n");
    }
    user = users.Find(blk);
    if (Cur_Passward[0] == '\0' && user.getPrivilege() != 7)
    {
        throw std::runtime_error("Invalid\n");
    }
    if (strcmp(New_Passward, user.Passward) != 0)
    {
        throw std::runtime_error("Invalid\n");
    }
    Block<Account> curBlock(ID, user);
    users.Delete(curBlock);
    strncpy(user.Passward, New_Passward, sizeof(Passward));
    Block<Account> newBlock(ID, user);
    users.Insert(newBlock);
}

void Account::useradd(char *id, char *passward, int privilege, char *username)
{
    if (login_stack.empty() || (login_stack.back().getPrivilege() != 3 && login_stack.back().getPrivilege() != 7) || privilege == 7 || login_stack.back().getPrivilege() < privilege)
    {
        throw std::runtime_error("Invalid\n");
    }
    Account user;
    Block<Account> blk(id, user);
    if (users.ifFind(blk))
    {
        throw std::runtime_error("Invalid\n");
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
    if (login_stack.empty() || login_stack.back().getPrivilege() != 7)
    {
        throw std::runtime_error("Invalid\n");
    }
    Account user;
    Block<Account> blk(ID, user);
    if (!users.ifFind(blk))
    {
        throw std::runtime_error("Invalid\n");
    }
    for (const auto &obj : login_stack)
    {
        if (strcmp(obj.ID, ID) == 0)
        {
            throw std::runtime_error("Invalid\n");
        }
    }
    user = users.Find(blk);
    Block<Account> curBlock(ID, user);
    users.Delete(curBlock);
}