#include "Book.hpp"
#include <sstream>
// #include "Account.hpp"
#include <cstring>

void Book::show_ISBN(char *index)
{
    if (index[0] == '\0' || login_stack.empty())
    {
        throw std::runtime_error("Invalid\n");
    }
    Book book;
    Block<Book> blk(index, book);
    if (!books_ISBN.ifFind(blk))
    {
        std::cout << "\n";
    }
    else
    {
        books_ISBN.showFind(blk);
    }
}

void Book::show_name(char *index)
{
    if (index[0] == '\0' || login_stack.empty())
    {
        throw std::runtime_error("Invalid\n");
    }
    Book book;
    Block<Book> blk(index, book);
    if (!books_Name.ifFind(blk))
    {
        std::cout << "\n";
    }
    else
    {
        books_Name.showFind(blk);
    }
}

void Book::show_author(char *index)
{
    if (index[0] == '\0' || login_stack.empty())
    {
        throw std::runtime_error("Invalid\n");
    }
    Book book;
    Block<Book> blk(index, book);
    if (!books_Author.ifFind(blk))
    {
        std::cout << "\n";
    }
    else
    {
        books_Author.showFind(blk);
    }
}

void Book::show_keyword(char *index)
{
    if (index[0] == '\0' || login_stack.empty())
    {
        throw std::runtime_error("Invalid\n");
    }
    Book book;
    Block<Book> blk(index, book);
    if (!books_Keyword.ifFind(blk))
    {
        std::cout << "\n";
    }
    else
    {
        books_Keyword.showFind(blk);
    }
}

void Book::show_all()
{
    if(login_stack.empty())
    {
        throw std::runtime_error("Invalid\n");
    }
    books_ISBN.showAll();
}

void Book::buy(char *isbn, int quantity)
{
    if (login_stack.empty())
    {
        throw std::runtime_error("Invalid\n");
    }
    Book book;
    Block<Book> blk1(isbn, book);
    if (!books_ISBN.ifFind(blk1))
    {
        throw std::runtime_error("Invalid\n");
    }

    book = books_ISBN.Find(blk1); // 取值成功 同一个ISBN仅有一本书
    if (quantity <= 0 || book.quantity < quantity)
    {
        throw std::runtime_error("Invalid\n");
    }
    Block<Book> blk2(book.BookName, book);
    Block<Book> blk3(book.Author, book);
    blk1.value = book;

    std::string str(book.Keyword); // 分割keyword
    std::stringstream ss(str);
    std::vector<std::string> vec;
    std::string temp;
    while (std::getline(ss, temp, '|'))
    {
        vec.push_back(temp);
    }
    Block<Book> blk_key[60];
    for (int k = 0; k < vec.size(); ++k)
    {
        strncpy(blk_key[k].index, vec[k].data(), sizeof(blk_key[k].index));
        blk_key[k].value = book;
    }

    books_ISBN.Delete(blk1);
    books_Name.Delete(blk2);
    books_Author.Delete(blk3);
    for (int k = 0; k < vec.size(); ++k)
    {
        books_Keyword.Delete(blk_key[k]);
    }

    book.quantity -= quantity;
    double money = quantity * book.Price;
    blk1.value = blk2.value = blk3.value = book;
    for (int k = 0; k < vec.size(); ++k)
    {
        blk_key[k].value = book;
        books_Keyword.Insert(blk_key[k]);
    }
    books_ISBN.Insert(blk1);
    books_Name.Insert(blk2);
    books_Author.Insert(blk3);

    int total;
    logg.File_finance.get_info(total, 1);
    logg.File_finance.write(money, 4 + sizeof(double) * total, 1);
    // double hhh;
    // logg.File_finance.read(hhh, 4 + sizeof(double) * total, 1);
    // std::cout << hhh << "\n";
    ++total;
    logg.File_finance.write_info(total, 1);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << money << "\n";
}

void Book::select(char *isbn)
{
    if (login_stack.empty() || login_stack.back().getPrivilege() < 3)
    {
        throw std::runtime_error("Invalid\n");
    }
    Book book; // 创建空对象
    Block<Book> blk(isbn, book);
    if (!books_ISBN.ifFind(blk)) // 无书
    {
        strncpy(book.ISBN, isbn, sizeof(book.ISBN));
        blk.value = book;       // 不小心注释掉导致加入的所有新书isbn为空
        books_ISBN.Insert(blk); // 加入新书
    }
    // else //似乎没用？
    // {
    //     book = books_ISBN.Find(blk); // 补充值
    // }
    strncpy(login_stack.back().selected, isbn, sizeof(login_stack.back().selected));
}

void Book::modify(char *isbn, char *bookname, char *author, char *keyword, double price)
{
    if (login_stack.empty() || login_stack.back().getPrivilege() < 3 || login_stack.back().selected[0] == '\0')
    {
        throw std::runtime_error("Invalid\n");
    }
    Book book;
    Block<Book> tmp(login_stack.back().selected);

    Book cur;
    cur = books_ISBN.Find(tmp);                         // 选中的书
    if (strcmp(isbn, cur.ISBN) == 0 && isbn[0] != '\0') // isbn不能改为原isbn
    {
        throw std::runtime_error("Invalid\n");
    }

    Block<Book> f(isbn);// isbn不能改成已有的isbn
    if (books_ISBN.ifFind(f)) 
    {
        throw std::runtime_error("Invalid\n");
    }

    Block<Book> blk1(cur.ISBN, book); // 空book
    book = books_ISBN.Find(blk1);     // 寻找值
    Block<Book> blk2(book.BookName, book);
    Block<Book> blk3(book.Author, book);
    blk1.value = book;

    std::string str(book.Keyword); // 分割值的keyword，得到完整键值对
    std::stringstream ss(str);
    std::vector<std::string> vec;
    std::string temp;
    while (std::getline(ss, temp, '|'))
    {
        vec.push_back(temp);
    }
    Block<Book> blk_key[60];
    for (int k = 0; k < vec.size(); ++k)
    {
        strncpy(blk_key[k].index, vec[k].data(), sizeof(blk_key[k].index));
        blk_key[k].value = book;
    }

    
    std::string str1(keyword); // 分割keyword，为空也没关系
    std::stringstream ss1(str1);
    std::vector<std::string> vec1;
    std::string temp1;
    while (std::getline(ss1, temp1, '|'))
    {
        vec1.push_back(temp1);
    }
    Block<Book> blk_key1[60];
    for (int k = 0; k < vec1.size(); ++k)
    {
        for(int j = 0; j < vec1.size(); ++j)//检查重复信息段
        {
            if(j != k && vec1[k] == vec1[j])
            {
                throw std::runtime_error("Invalid\n");
            }
        }
        strncpy(blk_key1[k].index, vec1[k].data(), sizeof(blk_key1[k].index));
    }
    //确定没有重复信息段才能删除
    //删除键值对后改键
    books_ISBN.Delete(blk1);
    if (isbn[0] != '\0') // 改ISBN
    {
        strncpy(blk1.index, isbn, sizeof(blk1.index));
        for(std::size_t i = 0; i < login_stack.size(); ++i)
        {
            if(strcmp(login_stack[i].selected, book.ISBN) == 0)
            {
                strncpy(login_stack[i].selected, isbn, sizeof(login_stack[i].selected));
            }
        }
        
    }

    if (cur.BookName[0] != '\0') // 不是新书
    {
        books_Name.Delete(blk2); // 删除原有键值对
    }
    if (bookname[0] != '\0')
    {
        strncpy(blk2.index, bookname, sizeof(blk2.index));
    }

    if (cur.Author[0] != '\0')
    {
        books_Author.Delete(blk3);
    }
    if (author[0] != '\0')
    {
        strncpy(blk3.index, author, sizeof(blk3.index));
    }


    if (cur.Keyword[0] != '\0')
    {
        for (int k = 0; k < vec.size(); ++k)
        {
            books_Keyword.Delete(blk_key[k]); //删除所有keyword的键值对
        }
    }
    // 修改cur
    if (isbn[0] != '\0') 
    {
        strncpy(cur.ISBN, isbn, sizeof(cur.ISBN));
    }
    if (bookname[0] != '\0')
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

    blk1.value = blk2.value = blk3.value = cur; // 更新值
    //重新插入键值对进入各个数据库
    //对于keyword数据库要按所有keyword插入
    if (blk1.index[0] != '\0')
    {
        books_ISBN.Insert(blk1);
    }
    if (blk2.index[0] != '\0')
    {
        books_Name.Insert(blk2);
    }
    if (blk3.index[0] != '\0')
    {
        books_Author.Insert(blk3);
    }

    for (int k = 0; k < vec1.size(); ++k)
    {
        if(blk_key1[k].index[0] != '\0')
        {
            blk_key1[k].value = cur;
            books_Keyword.Insert(blk_key1[k]);
        }
    }
    for(int k = 0; k < vec.size(); ++k)
    {
        if(keyword[0] == '\0')
        {
            blk_key[k].value = cur;
            books_Keyword.Insert(blk_key[k]);
        }
    }
}

void Book::import(int quantity, double Totalcost)
{
    if (login_stack.empty() || login_stack.back().getPrivilege() < 3 || login_stack.back().selected[0] == '\0')
    {
        throw std::runtime_error("Invalid\n");
    }
    if (quantity <= 0 || Totalcost <= 0)
    {
        throw std::runtime_error("Invalid\n");
    }

    Book book; // 创建空对象
    Block<Book> tmp(login_stack.back().selected, book);
    Book cur = books_ISBN.Find(tmp);

    cur.quantity += quantity; // 更改cur

    Block<Book> blk1(cur.ISBN, book);
    book = books_ISBN.Find(blk1);          // 寻找值
    Block<Book> blk2(book.BookName, book); // 此时book已经非空
    Block<Book> blk3(book.Author, book);
    blk1.value = book;

    std::string str(book.Keyword);
    std::stringstream ss(str);
    std::vector<std::string> vec;
    std::string temp;
    while (std::getline(ss, temp, '|'))
    {
        vec.push_back(temp);
    }
    Block<Book> blk_key[60];
    for (int k = 0; k < vec.size(); ++k)
    {
        strncpy(blk_key[k].index, vec[k].data(), sizeof(blk_key[k].index));
        blk_key[k].value = book;
    }

    books_ISBN.Delete(blk1); // 删除值
    books_Name.Delete(blk2);
    books_Author.Delete(blk3);
    for (int k = 0; k < vec.size(); ++k)
    {
        books_Keyword.Delete(blk_key[k]);
    }

    blk1.value = blk2.value = blk3.value = cur;
    for (int k = 0; k < vec.size(); ++k)
    {
        blk_key[k].value = cur;
        books_Keyword.Insert(blk_key[k]);
    }
    books_ISBN.Insert(blk1); // 更新值
    books_Name.Insert(blk2);
    books_Author.Insert(blk3);

    int total;
    logg.File_finance.get_info(total, 1);
    double money = -Totalcost;
    logg.File_finance.write(money, 4 + sizeof(double) * total, 1);
    ++total;
    logg.File_finance.write_info(total, 1);
}