#include <iostream>
#include <ctime>
#include "Utils/tokenScanner.hpp"
// #include "src/Account.hpp"
#include "src/Book.hpp"
#include "src/Log.hpp"
std::vector<Account> login_stack;
//Log logg;


/*
特别鸣谢助教蒋松霖学长、陈一星学长、江语学长、李兴阳学长，他们在本次大作业编译调试过程中给予我巨大帮助
以及codemate的资料支持
*/

bool check(int k, const std::string token[]) // 恰好k个切片
{
    for (int i = 2; i < 2 + k; ++i)
    {
        if (token[i] == "")
        {
            return false;
        }
    }
    if (token[k + 2] != "")
    {
        return false;
    }
    return true;
}

void log_add(std::string line, char *buffer)
{
    Block<Log> blk;
    Log log;
    strncpy(log.ID, login_stack.back().getID(), sizeof(log.ID));
    log.op = line;
    strncpy(blk.index, buffer, sizeof(blk.index));
    File_log.Insert(blk);
    if(login_stack.back().getPrivilege() == 3)
    {
        File_employee.Insert(blk);
    }
}

void processLine(std::string line, Account &user, Book &book, Log &logg)
{
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    // scanner.scanNumbers();
    scanner.setInput(line);

    std::time_t currentTime = std::time(nullptr);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

    std::string token[20]{};
    int i = 1;
    while (scanner.hasMoreTokens())
    {
        token[i] = scanner.nextToken();
        if (token[i].substr(0, 1) == "-" && token[i].find("=") != std::string::npos)
        {
            size_t equal = token[i].find("=");
            std::string part1 = token[i].substr(1, equal - 1);
            std::string part2 = token[i].substr(equal + 1);
            if (part2.length() >= 2 && part2.front() == '"' && part2.back() == '"')
            {
                part2 = part2.substr(1, part2.length() - 2);
            }
            token[i] = part1;
            token[i + 1] = part2;
            ++i;
        }
        ++i;
    }
    // int j = 1;
    // while(token[j] != "")
    // {
    //     std::cout << token[j] << " " << j << " " << "\n";
    //     ++j;
    // }

    bool flag = false;
    if (token[1] == "quit" || token[1] == "exit")
    {
        exit(0); // 是否需要析构？？？
    }
    if (token[1] == "su")
    {
        if (!check(2, token) && !check(1, token))
        {
            throw std::runtime_error("Invalid\n");
        }
        if (token[2].length() > 30 || token[3].length() > 30)
        {
            throw std::runtime_error("Invalid\n");
        }
        Account::login(token[2].data(), token[3].data());
        flag = true;
        log_add(line, buffer);
    }
    if (token[1] == "logout")
    {
        user.logout();
        flag = true;
    }
    if (token[1] == "register")
    {
        if (!check(3, token))
        {
            throw std::runtime_error("Invalid\n");
        }
        if (token[2].length() > 30 || token[3].length() > 30 || token[4].length() > 30)
        {
            throw std::runtime_error("Invalid\n");
        }
        user.signup(token[2].data(), token[3].data(), token[4].data());
        flag = true;
        log_add(line, buffer);
    }
    if (token[1] == "passwd")
    {
        if (!check(3, token) && !check(2, token))
        {
            throw std::runtime_error("Invalid\n");
        }
        if (token[2].length() > 30 || token[3].length() > 30 || token[4].length() > 30)
        {
            throw std::runtime_error("Invalid\n");
        }
        user.modify(token[2].data(), token[3].data(), token[4].data());
        flag = true;
        log_add(line, buffer);
    }
    if (token[1] == "useradd")
    {
        if (!check(4, token))
        {
            throw std::runtime_error("Invalid\n");
        }
        if (token[2].length() > 30 || token[3].length() > 30 || !isdigit(token[4][0]) || token[5].length() > 30)
        {
            throw std::runtime_error("Invalid\n");
        }
        int p = stoi(token[4]);
        if (p != 1 && p != 3 && p != 7)
        {
            throw std::runtime_error("Invalid\n");
        }
        user.useradd(token[2].data(), token[3].data(), stoi(token[4]), token[5].data());
        flag = true;
        log_add(line, buffer);
    }
    if (token[1] == "delete")
    {
        if (!check(1, token))
        {
            throw std::runtime_error("Invalid\n");
        }
        if (token[2].length() > 30)
        {
            throw std::runtime_error("Invalid\n");
        }
        user.del(token[2].data());
        flag = true;
        log_add(line, buffer);
    }

    if (token[1] == "show" && token[2] != "finance")
    {
        if (!check(2, token) && !check(0, token))
        {
            throw std::runtime_error("Invalid2\n");
        }
        if (token[3].length() > 60)
        {
            throw std::runtime_error("Invalid\n");
        }
        if (token[2] == "ISBN")
        {
            if (token[3].length() > 20)
            {
                throw std::runtime_error("Invalid\n");
            }
            book.show_ISBN(token[3].data());
        }
        if (token[2] == "name")
        {
            book.show_name(token[3].data());
        }
        if (token[2] == "author")
        {
            book.show_author(token[3].data());
        }
        if (token[2] == "keyword")
        {
            book.show_keyword(token[3].data());
        }
        if (token[2].empty())
        {
            book.show_all();
        }
        flag = true;
        log_add(line, buffer);
    }
    if (token[1] == "buy")
    {
        if (!check(2, token))
        {
            throw std::runtime_error("Invalid\n");
        }
        if (token[2].length() > 20 || !isdigit(token[3][0]))
        {
            throw std::runtime_error("Invalid\n");
        }
        book.buy(token[2].data(), stoi(token[3]));
        flag = true;
        log_add(line, buffer);
    }
    if (token[1] == "select")
    {
        if (!check(1, token))
        {
            throw std::runtime_error("Invalid\n");
        }
        if (token[2].length() > 20)
        {
            throw std::runtime_error("Invalid\n");
        }
        book.select(token[2].data());
        flag = true;
        log_add(line, buffer);
    }
    if (token[1] == "modify")
    {
        if (!check(2, token) && !check(4, token) && !check(6, token) && !check(8, token) && !check(10, token))
        {
            throw std::runtime_error("Invalid\n");
        }
        std::string tmp[5]{};
        int j = 2;
        while (j < 9)
        {
            if (token[j] == "ISBN")
            {
                tmp[0] = token[j + 1];
            }
            if (token[j] == "name")
            {
                tmp[1] = token[j + 1];
            }
            if (token[j] == "author")
            {
                tmp[2] = token[j + 1];
            }
            if (token[j] == "keyword")
            {
                tmp[3] = token[j + 1];
            }
            if (token[j] == "price")
            {
                tmp[4] = token[j + 1];
            }
            j += 2;
        }
        // 重复附加参数 keyword包含重复信息？
        double value;
        if (!isdigit(tmp[4][0]) && tmp[4] != "")
        {
            throw std::runtime_error("Invalid\n");
        }
        if (tmp[4] == "")
        {
            value = 0.0;
        }
        else
        {
            value = stod(tmp[4]);
        }
        if (tmp[0].length() > 20 && tmp[0] != "")
        {
            throw std::runtime_error("Invalid\n");
        }
        for (int m = 1; m < 4; ++m)
        {
            if (tmp[m].length() > 60 && tmp[m] != "")
            {
                throw std::runtime_error("Invalid\n");
            }
        }
        book.modify(tmp[0].data(), tmp[1].data(), tmp[2].data(), tmp[3].data(), value);
        flag = true;
        log_add(line, buffer);
    }
    if (token[1] == "import")
    {
        if (!check(2, token))
        {
            throw std::runtime_error("Invalid\n");
        }
        if (!isdigit(token[2][0]) || !isdigit(token[3][0]))
        {
            throw std::runtime_error("Invalid\n");
        }
        book.import(stoi(token[2]), stod(token[3]));
        flag = true;
        log_add(line, buffer);
    }

    if (token[1] == "show" && token[2] == "finance")
    {
        if (!isdigit(token[3][0]) && token[3] != "")
        {
            throw std::runtime_error("Invalid\n");
        }
        if (token[3] == "")
        {
            logg.show_finance(0);
        }
        else
        {
            if (stoi(token[3]) == 0)
            {
                std::cout << "\n";
            }
            else
            {
                logg.show_finance(stoi(token[3]));
            }
        }
        flag = true;
        log_add(line, buffer);
    }
    if(token[1] == "report" && token[2] == "finance")
    {
        flag = true;
        logg.report_finance();
        log_add(line, buffer);
    }
    if(token[1] == "report" && token[2] == "employee")
    {
        flag = true;
        logg.report_employee();
        log_add(line, buffer);
    }
    if(token[1] == "log")
    {
        flag = true;
        logg.log();
        log_add(line, buffer);
    }

    if (!flag)
    {
        throw std::runtime_error("Invalid\n");
    }
}

int main()
{
    //std::freopen("/mnt/x/VS code/big work/Bookstore-2023-main/Bookstore-2023-main/bookstore-testcases/complex/testcase1/1.in", "r", stdin);
    //std::freopen("output.txt", "w", stdout);
    Account user;
    Book book;
    Log logg;
    Account::setroot();
    while (true)
    {
        try
        {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
            {
                return 0;
            }
            processLine(input, user, book, logg);
        }
        catch (const std::exception &e)
        {
            std::cout << e.what();
        }
    }

    return 0;
}
