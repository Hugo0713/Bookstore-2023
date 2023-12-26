#ifndef CODE_BOOK_HPP
#define CODE_BOOK_HPP

#include <iostream>
#include "account.hpp"
#include "Log.hpp"
// #include "/mnt/x/VS code/big work/Bookstore-2023-main/Bookstore-2023-main/Database/Database.hpp"

// class Account;
class Book
{
private:
    char ISBN[21];
    char BookName[61];
    char Author[61];
    char Keyword[61];
    // std::vector<std::string> Keyword;
    int quantity;
    double Price;

public:
    Book() : ISBN{}, BookName{}, Author{}, Keyword{}, quantity(0), Price(0.0) {}

    bool operator==(const Book &obj) const
    {
        return (strcmp(ISBN, obj.ISBN) == 0 && strcmp(BookName, obj.BookName) == 0 && strcmp(Author, obj.Author) == 0 && strcmp(Keyword, obj.Keyword) == 0 && quantity == obj.quantity && Price == obj.Price);
    }
    bool operator<(const Book &obj) const
    {
        if (strcmp(ISBN, obj.ISBN) != 0)
        {
            return (strcmp(ISBN, obj.ISBN) < 0);
        }
        else if (strcmp(BookName, obj.BookName) != 0)
        {
            return (strcmp(BookName, obj.BookName) < 0);
        }
        else if (strcmp(Author, obj.Author) != 0)
        {
            return (strcmp(Author, obj.Author) < 0);
        }
        else if (strcmp(Keyword, obj.Keyword) != 0)
        {
            return (strcmp(Keyword, obj.Keyword) < 0);
        }
        else if (quantity != obj.quantity)
        {
            return (quantity < obj.quantity);
        }
        else
        {
            return (Price < obj.Price);
        }
    }

    Book &operator=(const Book &obj)
    {
        if (this == &obj)
        {
            return *this;
        }
        strncpy(ISBN, obj.ISBN, sizeof(ISBN));
        strncpy(BookName, obj.BookName, sizeof(BookName));
        strncpy(Author, obj.Author, sizeof(Author));
        strncpy(Keyword, obj.Keyword, sizeof(Keyword));
        quantity = obj.quantity;
        Price = obj.Price;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &out, const Book &obj)
    {
        std::cout << std::fixed << std::setprecision(2);
        out << obj.ISBN << "\t" << obj.BookName << "\t" << obj.Author << "\t" << obj.Keyword << "\t" << obj.Price << "\t" << obj.quantity;
        return out;
    }

    const char *getISBN() const
    {
        return ISBN;
    }
    const char *getName() const
    {
        return BookName;
    }
    const char *getAuthor() const
    {
        return Author;
    }
    const char *getKeyword() const
    {
        return Keyword;
    }

    void show_ISBN(char *index);
    void show_name(char *index);
    void show_author(char *index);
    void show_keyword(char *index);
    void show_all();
    void buy(char *ISBN, int quantity);
    void select(char *ISBN);
    void modify(char *isbn, char *bookname, char *author, char *keyword, double price);
    void import(int quantity, double Totalcost);
};

static Database<Book> books_ISBN("File_ISBN");
static Database<Book> books_Name("File_Name");
static Database<Book> books_Author("File_Author");
static Database<Book> books_Keyword("File_Keyword");
extern std::vector<Account> login_stack;
//extern Log logg;

#endif