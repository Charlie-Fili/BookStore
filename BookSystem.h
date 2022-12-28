#ifndef INC_1_6BOOKSTORE_BOOKSYSTEM_H
#define INC_1_6BOOKSTORE_BOOKSYSTEM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <iomanip>
#include "AccountSystem.h"
#include "UnrolledLinkedList_double.h"

class Book {
public:
    int tag;
    char ISBN[21];
    char BookName[61];
    char Author[61];
    char Keyword[61];
    int Quantity = 0;
    double Price = 0.0;
    double TotalCost = 0.0;

    Book() {
        tag = -1;
        memset(ISBN, 0, sizeof(ISBN));
        memset(BookName, 0, sizeof(BookName));
        memset(Author, 0, sizeof(Author));
        memset(Keyword, 0, sizeof(Keyword));
    }

    Book(const int &tag_, const char (&ISBN_)[21], const char (&BookName_)[61], const char (&Author_)[61],
         const char (&Keyword_)[61]) {
        tag = tag_;
        memset(ISBN, 0, sizeof(ISBN));
        memset(BookName, 0, sizeof(BookName));
        memset(Author, 0, sizeof(Author));
        memset(Keyword, 0, sizeof(Keyword));
        strcpy(ISBN, ISBN_);
        strcpy(BookName, BookName_);
        strcpy(Author, Author_);
        strcpy(Keyword, Keyword_);
    }

    Book(const Book &rhs) {
        tag = rhs.tag;
        memset(ISBN, 0, sizeof(ISBN));
        memset(BookName, 0, sizeof(BookName));
        memset(Author, 0, sizeof(Author));
        memset(Keyword, 0, sizeof(Keyword));
        strcpy(ISBN, rhs.ISBN);
        strcpy(BookName, rhs.BookName);
        strcpy(Author, rhs.Author);
        strcpy(Keyword, rhs.Keyword);
    }

    Book &operator=(const Book &rhs) {
        if (this == &rhs) return *this;
        tag = rhs.tag;
        memset(ISBN, 0, sizeof(ISBN));
        memset(BookName, 0, sizeof(BookName));
        memset(Author, 0, sizeof(Author));
        memset(Keyword, 0, sizeof(Keyword));
        strcpy(ISBN, rhs.ISBN);
        strcpy(BookName, rhs.BookName);
        strcpy(Author, rhs.Author);
        strcpy(Keyword, rhs.Keyword);
        return *this;
    }

};

class BookSystem {
public:
    BlockList<char[21]> ISBN_map;
    double_BlockList<char[61], char[21]> BookName_map;
    double_BlockList<char[61], char[21]> Author_map;
    double_BlockList<char[61], char[21]> Keyword_map;
    std::fstream Book_inf;
    std::string filename_ = "Book_inf";
    int amount = 0;

    BookSystem() : ISBN_map("ISBNBody", "ISBNHead"), BookName_map("BookNameBody", "BookNameHead"),
                   Author_map("AuthorBody", "AuthorHead"), Keyword_map("KeywordBody", "KeywordHead") {
        Book_inf.open(filename_);
        if (!Book_inf.good()) { // 是否成功打开
            Book_inf.open(filename_, std::fstream::out); // 新建
            Book_inf.close();
            Book_inf.open(filename_); // 变为可读可写
        } else {
            Book_inf.seekg(0);
            Book_inf.read(reinterpret_cast <char *> (&amount), sizeof(amount));
        }
    }

    ~BookSystem() {
        Book_inf.seekp(0);
        Book_inf.write(reinterpret_cast <char *> (&amount), sizeof(amount));
        Book_inf.close();
    }

    void readFile(Book &read_, const int &location) {
        Book_inf.seekg(sizeof(Book) * location + sizeof(amount));
        Book_inf.read(reinterpret_cast<char *>(&read_), sizeof(Book));
    }

    void writeFile(Book &write_, const int &location) {
        Book_inf.seekp(sizeof(Book) * location + sizeof(amount));
        Book_inf.write(reinterpret_cast<char *>(&write_), sizeof(Book));
    }

    void show(AccountSystem &account) {
        for (auto &iter: ISBN_map.HeadList) {
            BlockList<char[21]>::Block tmp_;
            ISBN_map.readFile(tmp_, iter.tag);
            for (int i = 0; i < iter.size; ++i) {
                Book tmp;
                readFile(tmp, tmp_[i].value);
                std::cout << tmp.ISBN << '\t' << tmp.BookName << '\t' << tmp.Author << '\t' << tmp.Keyword << '\t';
                std::cout.setf(std::ios::fixed);
                std::cout << std::setprecision(2) << tmp.Price << '\t';
                std::cout << tmp.Quantity << '\n';
            }
        }
    }

    void ISBN_show(const char (&ISBN_)[21], AccountSystem &account) {
        int index_ = ISBN_map.search(ISBN_);
        if (index_ == -1) {
            std::cout << '\n';
            return;
        } else {
            Book tmp;
            readFile(tmp, index_);
            std::cout << tmp.ISBN << '\t' << tmp.BookName << '\t' << tmp.Author << '\t' << tmp.Keyword << '\t';
            std::cout.setf(std::ios::fixed);
            std::cout << std::setprecision(2) << tmp.Price << '\t';
            std::cout << tmp.Quantity << '\n';
        }
    }

    void BookName_show(const char (&BookName_)[61], AccountSystem &account) {
        BookName_map.search(BookName_);
        if (BookName_map.indexes.empty()) {
            std::cout << '\n';
            return;
        } else {
            for (auto &iter: BookName_map.indexes) {
                Book tmp;
                readFile(tmp, iter);
                std::cout << tmp.ISBN << '\t' << tmp.BookName << '\t' << tmp.Author << '\t' << tmp.Keyword << '\t';
                std::cout.setf(std::ios::fixed);
                std::cout << std::setprecision(2) << tmp.Price << '\t';
                std::cout << tmp.Quantity << '\n';
            }
        }
    }

    void Author_show(const char (&Author_)[61], AccountSystem &account) {
        Author_map.search(Author_);
        if (Author_map.indexes.empty()) {
            std::cout << '\n';
            return;
        } else {
            for (auto &iter: Author_map.indexes) {
                Book tmp;
                readFile(tmp, iter);
                std::cout << tmp.ISBN << '\t' << tmp.BookName << '\t' << tmp.Author << '\t' << tmp.Keyword << '\t';
                std::cout.setf(std::ios::fixed);
                std::cout << std::setprecision(2) << tmp.Price << '\t';
                std::cout << tmp.Quantity << '\n';
            }
        }
    }

    void Keyword_show(const char (&Keyword_)[61], AccountSystem &account) {
        for (int i = 0; i < 61; ++i) {
            if (Keyword_[i] == '|') throw std::string("Invalid\n");
        }
        Keyword_map.search(Keyword_);
        if (Keyword_map.indexes.empty()) {
            std::cout << '\n';
            return;
        } else {
            for (auto &iter: Keyword_map.indexes) {
                Book tmp;
                readFile(tmp, iter);
                std::cout << tmp.ISBN << '\t' << tmp.BookName << '\t' << tmp.Author << '\t' << tmp.Keyword << '\t';
                std::cout.setf(std::ios::fixed);
                std::cout << std::setprecision(2) << tmp.Price << '\t';
                std::cout << tmp.Quantity << '\n';
            }
        }
    }

    void buy(const char (&ISBN_)[21], const int &Quantity_, AccountSystem &account, double &cost) {
        if (Quantity_ <= 0) throw std::string("Invalid\n");
        int index_ = ISBN_map.search(ISBN_);
        if (index_ == -1) throw std::string("Invalid\n");
        else {
            Book tmp;
            readFile(tmp, index_);
            if (tmp.Quantity < Quantity_) throw std::string("Invalid\n");
            else tmp.Quantity -= Quantity_;
            writeFile(tmp, index_);
            cost = tmp.Price * double(Quantity_);
            std::cout.setf(std::ios::fixed);
            std::cout << std::setprecision(2) << cost << '\n';
        }
    }

    void select(const char (&ISBN_)[21], AccountSystem &account) {
        int index_ = ISBN_map.search(ISBN_);
        if (index_ == -1) {
            ISBN_map.insert(ISBN_, amount);
            Book tmp;
            tmp.tag = amount;
            strcpy(tmp.ISBN, ISBN_);
            writeFile(tmp, amount);
            account.User_select.pop();
            account.User_select.push(amount);
            ++amount;
        } else {
            account.User_select.pop();
            account.User_select.push(index_);
        }
    }

    void ISBN_modify(const char (&ISBN_)[21], AccountSystem &account) {
        int index_ = account.User_select.top();
        Book tmp;
        readFile(tmp, index_);
        ISBN_map.erase(tmp.ISBN, tmp.tag);
        strcpy(tmp.ISBN, ISBN_);
        writeFile(tmp, index_);
        ISBN_map.insert(tmp.ISBN, tmp.tag);
    }

    void BookName_modify(const char (&BookName_)[61], AccountSystem &account) {
        int index_ = account.User_select.top();
        Book tmp;
        readFile(tmp, index_);
        char empty[61];
        memset(empty, 0, sizeof(empty));
        if (strcmp(empty, tmp.BookName) != 0) BookName_map.erase(tmp.BookName, tmp.ISBN, tmp.tag);
        strcpy(tmp.BookName, BookName_);
        writeFile(tmp, index_);
        BookName_map.insert(tmp.BookName, tmp.ISBN, tmp.tag);
    }

    void Author_modify(const char (&Author_)[61], AccountSystem &account) {
        int index_ = account.User_select.top();
        Book tmp;
        readFile(tmp, index_);
        char empty[61];
        memset(empty, 0, sizeof(empty));
        if (strcmp(empty, tmp.Author) != 0) Author_map.erase(tmp.Author, tmp.ISBN, tmp.tag);
        strcpy(tmp.Author, Author_);
        writeFile(tmp, index_);
        Author_map.insert(tmp.Author, tmp.ISBN, tmp.tag);
    }

//    void Keyword_modify(const char (Keyword_)[61], AccountSystem &account) {
//        int index_ = account.User_select.top();
//            Book tmp;
//            readFile(tmp, index_);
//            char empty[61];
//            memset(empty, 0, sizeof(empty));
//            if (strcmp(empty, tmp.Keyword) != 0) Keyword_map.erase(tmp.Keyword, tmp.ISBN, tmp.tag);
//            strcpy(tmp.Keyword, Keyword_);
//            writeFile(tmp, index_);
//            Keyword_map.insert(tmp.Keyword, tmp.ISBN, tmp.tag);
//    }

    void Price_modify(const double &Price_, AccountSystem &account) {
        int index_ = account.User_select.top();
        Book tmp;
        readFile(tmp, index_);
        tmp.Price = Price_;
        writeFile(tmp, index_);
    }

    void import(const int &Quantity_, const double &TotalCost_, AccountSystem &account) {
        if (Quantity_ <= 0 || TotalCost_ <= 0) throw std::string("Invalid\n");
        int index_ = account.User_select.top();
        Book tmp;
        readFile(tmp, index_);
        tmp.Quantity += Quantity_;
        tmp.TotalCost += TotalCost_;
        writeFile(tmp, index_);
    }
};

#endif //INC_1_6BOOKSTORE_BOOKSYSTEM_H
