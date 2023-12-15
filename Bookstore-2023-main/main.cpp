#include<iostream>
#include"Utils/tokenScanner.hpp"
#include"src/account.hpp"
#include"src/Book.hpp"
//#include"Database/Database_account.hpp"
#include"Database/Database_book_ISBN.hpp"







void processLine(std::string line, Account &user)
{
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    std::string token[5]{}; 

    while(scanner.hasMoreTokens())
    {
        int i = 1;
        token[i] = scanner.nextToken();
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
                processLine(input, user);
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        } 
    }
    return 0;
}