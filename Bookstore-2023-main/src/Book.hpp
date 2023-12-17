#ifndef CODE_BOOK_HPP
#define CODE_BOOK_HPP

#include <iostream>
#include "Account.hpp"
//#include "/mnt/x/VS code/big work/Bookstore-2023-main/Bookstore-2023-main/Database/Database.hpp"

//class Account;
class Book
{
private:
    char ISBN[21];
    char BookName[61];
    char Author[61];
    char Keyword[61];
    int quantity;
    double Price;

public:
    bool ifselected = false;
    Book() : ISBN{}, BookName{}, Author{}, Keyword{}, quantity(0), Price(0.0) {}

    bool operator==(const Book &obj) const
    {
        return ISBN == obj.ISBN;
    }
    bool operator<(const Book &obj) const
    {
        return ISBN < obj.ISBN;
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
        ifselected = obj.ifselected;
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &out, const Book &obj)
    {
        out << obj.ISBN << "\t" << obj.BookName << "\t" << obj.Keyword << "\t" << obj.Price << "\t" << obj.quantity << "\n";
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
    double buy(char *ISBN, int quantity);
    void select(char *ISBN);
    void modify(char *bookname, char *author, char *keyword, double price);
    void import(int quantity, double Totalcost);
};

static Database<Book> books_ISBN("File_ISBN");
static Database<Book> books_Name("File_Name");
static Database<Book> books_Author("File_Author");
static Database<Book> books_Keyword("File_Keyword");

#endif