#include<iostream>
#include"Utils/tokenScanner.hpp"
//#include"src/Account.hpp"
#include"src/Book.hpp"
#include"src/Log.hpp"









void processLine(std::string line, Account &user, Book &book)
{
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    std::string token[9]{}; 

    while(scanner.hasMoreTokens())
    {
        int i = 1;
        token[i] = scanner.nextToken();
        if(token[i].substr(0, 1) == "-" && token[i].find("=") != std::string::npos)
        {
            size_t equal = token[i].find("=");
            std::string part1 = token[i].substr(1, equal - 1);
            std::string part2 = token[i].substr(equal + 1);
            if(part2.length() >= 2 && part2.front() == '"' && part2.back() == '"')
            {
                part2 = part2.substr(1, part2.length() - 2);
            }
            token[i] = part1;
            token[i + 1] = part2;
            ++i;
        }
        ++i; 
    }
    
    
    if(token[1] == "quit" && token[1] == "exit")
    {
        exit(0);//是否需要析构？？？
    }
    if(token[1] == "su")
    {
        user.login(token[2].data(), token[3].data());
    }
    if(token[1] == "logout")
    {
        user.logout();
    }
    if(token[1] == "register")
    {
        user.signup(token[2].data(), token[3].data(), token[4].data());
    }
    if(token[1] == "passwd")
    {
        user.modify(token[2].data(), token[3].data(), token[4].data());
    }
    if(token[1] == "useradd")
    {
        user.useradd(token[2].data(), token[3].data(), stoi(token[4]), token[5].data());
    }
    if(token[1] == "delete")
    {
        user.del(token[2].data());
    }

    if(token[1] == "show" && token[2] != "finance")
    {
        if(token[2] == "ISBN")
        {
            book.show_ISBN(token[3].data());
        }
        if(token[2] == "name")
        {
            book.show_name(token[3].data());
        }
        if(token[2] == "author")
        {
            book.show_author(token[3].data());
        }
        if(token[2] == "keyword")
        {
            book.show_keyword(token[3].data());
        }
        if(token[2].empty())
        {
            book.show_all();
        }
    }
    if(token[1] == "buy")
    {
        book.buy(token[2].data(), stoi(token[3]));
    }
    if(token[1] == "select")
    {
        book.select(token[2].data());
    }
    if(token[1] == "modify")
    {
        std::string tmp[4]{};
        int i = 2;
        while(i <= 9)
        {
            if(token[i] == "name")
            {
                tmp[0] = token[i + 1];
            }
            if(token[i] == "author")
            {
                tmp[1] = token[i + 1];
            }
            if(token[i] == "keyword")
            {
                tmp[2] = token[i + 1];
            }
            if(token[i] == "price")
            {
                tmp[3] = token[i + 1];
            }
            ++i;
        }
        book.modify(tmp[0].data(), tmp[1].data(), tmp[2].data(), stod(tmp[3]));
    }
    if(token[1] == "import")
    {
        book.import(stoi(token[2]), stod(token[3]));
    }

    if(token[1] == "show" && token[2] == "finance")
    {

    }
}

int main()
{
    Account user;
    Book book;
    user.setroot();
    while(true)
    {
        try
        {
            {
                std::string input;
                getline(std::cin, input);
                if(input.empty())
                {
                    return 0;
                }
                processLine(input, user, book);
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        } 
    }
    return 0;
}