#include <iostream>
#include "Utils/tokenScanner.hpp"
// #include "src/Account.hpp"
#include "src/Book.hpp"
#include "src/Log.hpp"
std::vector<Account> login_stack;
Log logg("File_finance");

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

void processLine(std::string line, Account &user, Book &book)
{
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    // scanner.scanNumbers();
    scanner.setInput(line);
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
    }
    if (token[1] == "delete")
    {
        if (!check(1, token))
        {
            throw std::runtime_error("Invalid\n");
        }
        if (token[1].length() > 30)
        {
            throw std::runtime_error("Invalid\n");
        }
        user.del(token[2].data());
        flag = true;
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
    }

    if (token[1] == "show" && token[2] == "finance")
    {
        if (!isdigit(token[3][0]))
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
    }

    if (!flag)
    {
        throw std::runtime_error("Invalid\n");
    }
}

int main()
{
    // std::freopen("output.txt", "w", stdout);
    Account user;
    Book book;
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
            processLine(input, user, book);
        }
        catch (const std::exception &e)
        {
            std::cout << e.what();
        }
    }

    return 0;
}