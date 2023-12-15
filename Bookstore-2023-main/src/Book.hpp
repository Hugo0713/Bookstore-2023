#ifndef CODE_BOOK_HPP
#define CODE_BOOK_HPP

#include <iostream>
#include"/mnt/x/VS code/big work/Bookstore-2023-main/Bookstore-2023-main/Database/Database.hpp"

class Book
{
private:
    char ISBN[21];
    char BookName[61];
    char Author[61];
    char Keyword[61];
    int quantity;
    double price;

public:
    Book() = default;

    bool operator==(const Book &obj) const
    {
        return ISBN == obj.ISBN;
    }
    bool operator<(const Book &obj) const
    {
        return ISBN < obj.ISBN;
    }

    const char* getISBN() const
    {
        return ISBN;
    }
    const char* getName() const
    {
        return BookName;
    }
    const char* getAuthor() const
    {
        return Author;
    }
    const char* getKeyword() const
    {
        return Keyword;
    }
    
    void show(char *index);
    void buy(char *ISBN, int quantity);
    void select(char *ISBN);
    void modify(char *index);
    void import(int quantity, double Totalcost);
};

Database<Book> books_ISBN("File_ISBN");
Database<Book> books_Name("File_Name");
Database<Book> books_Author("File_Author");
Database<Book> books_Keyword("File_Keyword");

#endif