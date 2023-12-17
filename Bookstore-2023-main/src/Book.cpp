#include "Book.hpp"
#include "Account.hpp"
#include <cstring>

void Book::show_ISBN(char *index)
{
    if (index[0] == '\0')
    {
        throw std::runtime_error("Invalid\n");
    }
    Book book;
    Block<Book> blk(index, book);
    books_ISBN.showFind(blk);
}

void Book::show_name(char *index)
{
    if (index[0] == '\0')
    {
        throw std::runtime_error("Invalid\n");
    }
    Book book;
    Block<Book> blk(index, book);
    books_Name.showFind(blk);
}

void Book::show_author(char *index)
{
    if (index[0] == '\0')
    {
        throw std::runtime_error("Invalid\n");
    }
    Book book;
    Block<Book> blk(index, book);
    books_Author.showFind(blk);
}

void Book::show_keyword(char *index)
{
    if (index[0] == '\0')
    {
        throw std::runtime_error("Invalid\n");
    }
    Book book;
    Block<Book> blk(index, book);
    books_Keyword.showFind(blk);
}

void Book::show_all()
{
}

double Book::buy(char *ISBN, int quantity)
{
    if (login_stack.empty())
    {
        throw std::runtime_error("Invalid\n");
    }
    Book book;
    Block<Book> blk1(ISBN, book);
    if (!books_ISBN.ifFind(blk1))
    {
        throw std::runtime_error("Invalid\n");
    }

    book = books_ISBN.Find(blk1); // 取值成功
    if (quantity <= 0 || book.quantity < quantity)
    {
        throw std::runtime_error("Invalid\n");
    }

    Block<Book> blk2(book.BookName, book);
    Block<Book> blk3(book.Author, book);
    Block<Book> blk4(book.Keyword, book);
    blk1.value = book; // 更改值
    books_ISBN.Delete(blk1);
    books_Name.Delete(blk2);
    books_Author.Delete(blk3);
    books_Keyword.Delete(blk4);

    book.quantity -= quantity;
    double money = quantity * book.Price;
    blk1.value = blk2.value = blk3.value = blk4.value = book;
    books_ISBN.Insert(blk1);
    books_Name.Insert(blk2);
    books_Author.Insert(blk3);
    books_Keyword.Insert(blk4);

    return money;
}

void Book::select(char *ISBN)
{
    if (login_stack.empty() || login_stack.back().getPrivilege() < 3)
    {
        throw std::runtime_error("Invalid\n");
    }
    Book book; // 创建空对象
    Block<Book> blk(ISBN, book);
    if (!books_ISBN.ifFind(blk)) // 无书
    {
        strncpy(book.ISBN, ISBN, sizeof(book.ISBN));
        blk.value = book;
        books_ISBN.Insert(blk); // 加入新书
    }
    else
    {
        book = books_ISBN.Find(blk); // 补充值
    }
    book.ifselected = true;
    login_stack.back().selected = book;
}

void Book::modify(char *bookname, char *author, char *keyword, double price)
{
    if (login_stack.empty() || login_stack.back().getPrivilege() < 3)
    {
        throw std::runtime_error("Invalid\n");
    }
    Book cur = login_stack.back().selected;
    if (bookname[0] != '\0') // 修改cur
    {
        strncpy(cur.BookName, bookname, sizeof(cur.BookName));
    }
    if (author[0] != '\0')
    {
        strncpy(cur.Author, author, sizeof(cur.Author));
    }
    if (keyword[0] != '\0')
    {
        strncpy(cur.Keyword, keyword, sizeof(cur.Keyword));
    }
    if (price != 0.0)
    {
        cur.Price = price;
    }

    Book book;
    Block<Book> blk1(cur.ISBN, book);
    book = books_ISBN.Find(blk1); // 寻找值
    Block<Book> blk2(book.BookName, book);
    Block<Book> blk3(book.Author, book);
    Block<Book> blk4(book.Keyword, book);

    blk1.value = book;
    books_ISBN.Delete(blk1);
    if (books_Name.ifFind(blk2)) // 不是新书
    {
        books_Name.Delete(blk2); // 删除原有键值对
    }
    strncpy(blk2.index, bookname, sizeof(blk2.index));
    if (books_Author.ifFind(blk3))
    {
        books_Author.Delete(blk3);
    }
    strncpy(blk3.index, author, sizeof(blk3.index));
    if (books_Keyword.ifFind(blk4))
    {
        books_Keyword.Delete(blk4);
    }
    strncpy(blk4.index, keyword, sizeof(blk4.index));

    blk1.value = blk2.value = blk3.value = blk4.value = cur; // 更新值
    books_ISBN.Insert(blk1);
    books_Name.Insert(blk2);
    books_Author.Insert(blk3);
    books_Keyword.Insert(blk4);
}

void Book::import(int quantity, double Totalcost)
{
    if (login_stack.empty() || login_stack.back().getPrivilege() < 3)
    {
        throw std::runtime_error("Invalid\n");
    }

    Book cur = login_stack.back().selected;
    if (!cur.ifselected || quantity <= 0 || Totalcost <= 0)
    {
        throw std::runtime_error("Invalid\n");
    }
    cur.quantity += quantity; // 更改cur

    Book book; // 创建空对象
    Block<Book> blk1(cur.ISBN, book);
    book = books_ISBN.Find(blk1);          // 寻找值
    Block<Book> blk2(book.BookName, book); // 此时book已经非空
    Block<Book> blk3(book.Author, book);
    Block<Book> blk4(book.Keyword, book);
    blk1.value = book;

    books_ISBN.Delete(blk1); // 删除值
    books_Name.Delete(blk2);
    books_Author.Delete(blk3);
    books_Keyword.Delete(blk4);

    blk1.value = blk2.value = blk3.value = blk4.value = cur;
    books_ISBN.Insert(blk1); // 更新值
    books_Name.Insert(blk2);
    books_Author.Insert(blk3);
    books_Keyword.Insert(blk4);
}