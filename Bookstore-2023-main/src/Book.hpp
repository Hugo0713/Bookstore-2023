#ifndef CODE_BOOK_HPP
#define CODE_BOOK_HPP

#include <iostream>
#include "Database/Database_book_ISBN.hpp"

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
    bool operator==(const Book &obj) const
    {
        return ISBN == obj.ISBN;
    }
    bool operator<(const Book &obj) const
    {
        return ISBN < obj.ISBN;
    }

    void show(char *index);
    void buy(char *ISBN, int quantity);
    void select(char *ISBN);
    void modify(char *index);
    void import(int quantity, double Totalcost);
};

#endif