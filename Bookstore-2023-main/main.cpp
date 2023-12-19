#include <iostream>
#include "Utils/tokenScanner.hpp"
// #include"src/Account.hpp"
#include "src/Book.hpp"
#include "src/Log.hpp"

void processLine(std::string line, Account &user, Book &book)
{
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    //scanner.scanNumbers();
    scanner.setInput(line);
    std::string token[9]{};
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
    int j = 1;
    while(token[j] != "")
    {
        std::cout << token[j] << " " << j ;
        ++j;
    }

    bool flag = false;
    if (token[1] == "quit" && token[1] == "exit")
    {
        exit(0); // 是否需要析构？？？
    }
    if (token[1] == "su")
    {
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
        user.signup(token[2].data(), token[3].data(), token[4].data());
        flag = true;
    }
    if (token[1] == "passwd")
    {
        user.modify(token[2].data(), token[3].data(), token[4].data());
        flag = true;
    }
    if (token[1] == "useradd")
    {
        user.useradd(token[2].data(), token[3].data(), stoi(token[4]), token[5].data());
        flag = true;
    }
    if (token[1] == "delete")
    {
        user.del(token[2].data());
        flag = true;
    }

    if (token[1] == "show" && token[2] != "finance")
    {
        if (token[2] == "ISBN")
        {
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
        book.buy(token[2].data(), stoi(token[3]));
        flag = true;
    }
    if (token[1] == "select")
    {
        book.select(token[2].data());
        flag = true;
    }
    if (token[1] == "modify")
    {
        std::string tmp[4]{};
        int j = 2;
        while (j < 9)
        {
            if (token[j] == "name")
            {
                tmp[0] = token[j + 1];
            }
            if (token[j] == "author")
            {
                tmp[1] = token[j + 1];
            }
            if (token[j] == "keyword")
            {
                tmp[2] = token[j + 1];
            }
            if (token[j] == "price")
            {
                tmp[3] = token[j + 1];
            }
            ++j;
        }
        book.modify(tmp[0].data(), tmp[1].data(), tmp[2].data(), stod(tmp[3]));
        flag = true;
    }
    if (token[1] == "import")
    {
        book.import(stoi(token[2]), stod(token[3]));
        flag = true;
    }

    if (token[1] == "show" && token[2] == "finance")
    {
        flag = true;
    }

    if(!flag)
    {
        throw std::runtime_error("Invalid\n");
    }
}

int main()
{
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
            std::cerr << e.what();
        }
    }
    return 0;
}