#ifndef INC_1_6BOOKSTORE_TOKENSCANNER_H
#define INC_1_6BOOKSTORE_TOKENSCANNER_H

#include <iostream>
#include <string>
#include <cstring>
#include "LogSystem.h"
#include "check.h"

class TokenScanner {
public:
    std::string tokens;
    bool hasMore = true;
    int point = 0;

    TokenScanner() = default;

    explicit TokenScanner(const std::string &s) {
        tokens = s;
    }

    std::string nextToken() { // 空格分隔下一个字段
        while (tokens[point] == ' ' and point != tokens.length()) ++point;
        if (point == tokens.length()) return "";
        int begin = point;
        int end = point;
        while (tokens[end] != ' ' and end != tokens.length()) ++end;
        std::string tmp;
        tmp = tokens.substr(begin, end - begin);
        point = end;
        while (tokens[point] == ' ' and point != tokens.length()) ++point;
        if (point == tokens.length()) hasMore = false;
        return tmp;
    }

    std::string nextKey() { // |分隔下一个key
        int begin = point;
        int end = point;
        while (tokens[end] != '|' and end != tokens.length()) ++end;
        std::string tmp;
        tmp = tokens.substr(begin, end - begin);
        point = end;
        while (tokens[point] == '|' and point != tokens.length()) ++point;
        if (point == tokens.length()) hasMore = false;
        return tmp;
    }

    /*
    * accountSystem
    */

    void su(AccountSystem &account_, LogSystem &log_) { // 登录账户
        bool ifOmit = false; // 是否有省密码

        std::string tmp = nextToken();
        check_Type1(tmp);
        char UserID_[31];
        memset(UserID_, 0, sizeof(UserID_));
        strcpy(UserID_, tmp.c_str());

        tmp = nextToken();
        char Password_[31];
        memset(Password_, 0, sizeof(Password_));
        if (!tmp.empty()) {
            check_Type1(tmp);
            strcpy(Password_, tmp.c_str());
        } else ifOmit = true;

        if (hasMore) throw std::string("Invalid\n");

        account_.login(UserID_, Password_, ifOmit);

        int index_ = account_.user_map.search(UserID_);
        User record;
        account_.readFile(record, index_);
        log_.AddLog0(record.Privilege, UserID_, 0);
    }

    void logout(AccountSystem &account_, LogSystem &log_) {
        log_.AddLog0(account_.login_stack.top().Privilege, account_.login_stack.top().UserID, 1);

        account_.logout();
    }

    void register_(AccountSystem &account_, LogSystem &log_) {
        std::string tmp = nextToken();
        check_Type1(tmp);
        char UserID_[31];
        memset(UserID_, 0, sizeof(UserID_));
        strcpy(UserID_, tmp.c_str());

        tmp = nextToken();
        check_Type1(tmp);
        char Password_[31];
        memset(Password_, 0, sizeof(Password_));
        strcpy(Password_, tmp.c_str());

        tmp = nextToken();
        check_Type2(tmp);
        char Username_[31];
        memset(Username_, 0, sizeof(Username_));
        strcpy(Username_, tmp.c_str());

        if (hasMore) throw std::string("Invalid\n");

        account_.register_(UserID_, Password_, Username_);

        log_.AddLog0('1', UserID_, 2);
    }

    void passwd(AccountSystem &account_, LogSystem &log_) {
        bool ifOmit = false; // 是否有省密码

        std::string tmp = nextToken();
        check_Type1(tmp);
        char UserID_[31];
        memset(UserID_, 0, sizeof(UserID_));
        strcpy(UserID_, tmp.c_str());

        tmp = nextToken();
        std::string tmp1 = nextToken();
        char CurrentPassword_[31];
        memset(CurrentPassword_, 0, sizeof(CurrentPassword_));
        char NewPassword_[31];
        memset(NewPassword_, 0, sizeof(NewPassword_));
        if (tmp1.empty()) {
            ifOmit = true;
            check_Type1(tmp);
            strcpy(NewPassword_, tmp.c_str());
        } else {
            check_Type1(tmp);
            check_Type1(tmp1);
            strcpy(CurrentPassword_, tmp.c_str());
            strcpy(NewPassword_, tmp1.c_str());
        }

        if (hasMore) throw std::string("Invalid\n");

        account_.passwd(UserID_, CurrentPassword_, NewPassword_, ifOmit);

        int index_ = account_.user_map.search(UserID_);
        User record;
        account_.readFile(record, index_);
        log_.AddLog1(account_.login_stack.top().Privilege, account_.login_stack.top().UserID, 3, record.Privilege,
                     record.UserID);
    }

    void useradd(AccountSystem &account_, LogSystem &log_) {
        std::string tmp = nextToken();
        check_Type1(tmp);
        char UserID_[31];
        memset(UserID_, 0, sizeof(UserID_));
        strcpy(UserID_, tmp.c_str());

        tmp = nextToken();
        check_Type1(tmp);
        char Password_[31];
        memset(Password_, 0, sizeof(Password_));
        strcpy(Password_, tmp.c_str());

        tmp = nextToken();
        check_Type3(tmp);
        char Privilege_ = tmp[0];

        tmp = nextToken();
        check_Type2(tmp);
        char Username_[31];
        memset(Username_, 0, sizeof(Username_));
        strcpy(Username_, tmp.c_str());

        if (hasMore) throw std::string("Invalid\n");

        account_.useradd(UserID_, Password_, Privilege_, Username_);

        log_.AddLog1(account_.login_stack.top().Privilege, account_.login_stack.top().UserID, 4, Privilege_,
                     UserID_);
    }

    void delete_(AccountSystem &account_, LogSystem &log_) {
        std::string tmp = nextToken();
        check_Type1(tmp);
        char UserID_[31];
        memset(UserID_, 0, sizeof(UserID_));
        strcpy(UserID_, tmp.c_str());

        if (hasMore) throw std::string("Invalid\n");

        int index_ = account_.user_map.search(UserID_);
        User record;
        account_.readFile(record, index_);
        log_.AddLog1(account_.login_stack.top().Privilege, account_.login_stack.top().UserID, 10, record.Privilege,
                     record.UserID);

        account_.delete_(UserID_);
    }

    /*
     * bookSystem
     */

    void show(AccountSystem &account_, BookSystem &book_, LogSystem &log_) {
        std::string tmp = nextToken();
        if (tmp.empty()) {
            book_.show(account_);
            log_.AddLog2(account_.login_stack.top().Privilege, account_.login_stack.top().UserID, 6, " ");
            return;
        }
        if (tmp.length() < 6) throw std::string("Invalid\n");

        if (tmp[1] == 'I') {
            char ISBN_[21];
            memset(ISBN_, 0, sizeof(ISBN_));
            strcpy(ISBN_, check_assign_ISBN(tmp).c_str());

            if (hasMore) throw std::string("Invalid\n");

            book_.ISBN_show(ISBN_, account_);
        } else {
            char type[61];
            memset(type, 0, sizeof(type));
            strcpy(type, check_assign(tmp).c_str());

            if (hasMore) throw std::string("Invalid\n");

            if (tmp[1] == 'n') {
                book_.BookName_show(type, account_);
            } else if (tmp[1] == 'a') {
                book_.Author_show(type, account_);
            } else if (tmp[1] == 'k') {
                book_.Keyword_show(type, account_);
            } else throw std::string("Invalid\n");
        }

        log_.AddLog2(account_.login_stack.top().Privilege, account_.login_stack.top().UserID, 6, tmp.c_str());
    }

    void buy(AccountSystem &account_, BookSystem &book_, LogSystem &log_) {
        std::string tmp = nextToken();
        check_Type4(tmp);
        char ISBN_[21];
        memset(ISBN_, 0, sizeof(ISBN_));
        strcpy(ISBN_, tmp.c_str());

        tmp = nextToken();
        int Quantity_ = check_Type6(tmp);
        double cost = 0.0;

        book_.buy(ISBN_, Quantity_, account_, cost);

        log_.AddFinance(true, cost);

        log_.AddLog3(account_.login_stack.top().Privilege, account_.login_stack.top().UserID, 7, ISBN_, Quantity_,
                     cost);
    }

    void select(AccountSystem &account_, BookSystem &book_, LogSystem &log_) {
        if (account_.login_stack.top().Privilege < '3') throw std::string("Invalid\n");

        std::string tmp = nextToken();
        check_Type4(tmp);
        char ISBN_[21];
        memset(ISBN_, 0, sizeof(ISBN_));
        strcpy(ISBN_, tmp.c_str());

        book_.select(ISBN_, account_);

        log_.AddLog4(account_.login_stack.top().Privilege, account_.login_stack.top().UserID, 8, ISBN_);
    }

    void modify(AccountSystem &account_, BookSystem &book_, LogSystem &log_) {
        if (account_.login_stack.top().Privilege < '3') throw std::string("Invalid\n");
        int index_ = account_.User_select.top();
        if (index_ == -1) throw std::string("Invalid\n");
        if (!hasMore) throw std::string("Invalid\n");

        std::string tmp = nextToken();
        bool used[5];
        char ISBN_[21];
        char BookName_[61];
        char Author_[61];
        char Keyword_[61];
        double Price_ = 0.0;
        std::string Price;
        std::string Key;
        std::unordered_map<std::string, bool> hasKey; // 是否修改过
        std::vector<std::string> Key_store; // 先存起来，保证没错误再改变

        memset(used, false, sizeof(used));
        hasKey.clear();
        Key_store.clear();
        while (!tmp.empty()) {
            if (tmp.length() < 6) throw std::string("Invalid\n");
            if (tmp[1] == 'I') {
                if (used[0]) throw std::string("Invalid\n");

                memset(ISBN_, 0, sizeof(ISBN_));
                strcpy(ISBN_, check_assign_ISBN(tmp).c_str());
                int index = book_.ISBN_map.search(ISBN_);
                if (index != -1) throw std::string("Invalid\n");

                used[0] = true;
            } else if (tmp[1] == 'k') {
                if (used[3]) throw std::string("Invalid\n");

                memset(Keyword_, 0, sizeof(Keyword_));
                if (tmp.length() < 11) throw std::string("Invalid\n");
                std::string temp = tmp.substr(1, 9);
                if (temp != "keyword=\"" || tmp[tmp.size() - 1] != '\"') throw std::string("Invalid\n");
                temp = tmp.substr(10, tmp.length() - 11);
                check_key(temp);
                strcpy(Keyword_, temp.c_str());
                Key=temp;

                // 拆分keyword
                TokenScanner scanner_key(temp);
                std::string key = scanner_key.nextKey();
                while (!key.empty()) {
                    if (hasKey.count(key)) throw std::string("Invalid\n");
                    hasKey[key] = true;
                    Key_store.push_back(key);
                    key = scanner_key.nextKey();
                }

                used[3] = true;
            } else {
                char type[61];
                memset(type, 0, sizeof(type));
                strcpy(type, check_assign(tmp).c_str());

                if (tmp[1] == 'n') {
                    if (used[1]) throw std::string("Invalid\n");
                    used[1] = true;
                    memset(BookName_, 0, sizeof(BookName_));
                    strcpy(BookName_, type);
                } else if (tmp[1] == 'a') {
                    if (used[2]) throw std::string("Invalid\n");
                    used[2] = true;
                    memset(Author_, 0, sizeof(Author_));
                    strcpy(Author_, type);
                } else if (tmp[1] == 'p') {
                    if (used[4]) throw std::string("Invalid\n");
                    used[4] = true;
                    Price=type;
                    Price_ = atof(type);
                } else throw std::string("Invalid\n");
            }
            tmp = nextToken();
        }

        // 检验没有语法问题后统一进行变换
        Book tmpBook;
        book_.readFile(tmpBook, index_);

        if (used[0]) {
            std::string order = ISBN_;
            order = "change ISBN to " + order;
            log_.AddLog5(account_.login_stack.top().Privilege, account_.login_stack.top().UserID, 9, tmpBook.ISBN, order);
            book_.ISBN_modify(ISBN_, account_);
        }
        if (used[1]) {
            book_.BookName_modify(BookName_, account_);
            std::string order = BookName_;
            order = "change BookName to " + order;
            log_.AddLog5(account_.login_stack.top().Privilege, account_.login_stack.top().UserID, 9, tmpBook.ISBN, order);
        }
        if (used[2]) {
            book_.Author_modify(Author_, account_);
            std::string order = Author_;
            order = "change Author " + order;
            log_.AddLog5(account_.login_stack.top().Privilege, account_.login_stack.top().UserID, 9, tmpBook.ISBN, order);
        }
        if (used[4]) {
            book_.Price_modify(Price_, account_);
            std::string order = "change BookName to " + Price;
            log_.AddLog5(account_.login_stack.top().Privilege, account_.login_stack.top().UserID, 9, tmpBook.ISBN, order);
        }
        if (used[3]) {
            Book tmp_;
            book_.Book_inf.seekg(sizeof(Book) * index_ + sizeof(int));
            book_.Book_inf.read(reinterpret_cast<char *>(&tmp_), sizeof(Book));

            // 删除原来的key
            char empty[61];
            memset(empty, 0, sizeof(empty));
            if (strcmp(empty, tmp_.Keyword) != 0) {
                TokenScanner scanner_key_(tmp_.Keyword);
                std::string key_ = scanner_key_.nextKey();
                while (!key_.empty()) {
                    char key_c[61];
                    strcpy(key_c, key_.c_str());
                    book_.Keyword_map.erase(key_c, tmp_.ISBN, tmp_.tag);
                    key_ = scanner_key_.nextKey();
                }
            }

            strcpy(tmp_.Keyword, Keyword_);
            book_.Book_inf.seekp(sizeof(Book) * index_ + sizeof(int));
            book_.Book_inf.write(reinterpret_cast<char *>(&tmp_), sizeof(Book));

            // 一个个插入新key
            for (auto &iter: Key_store) {
                char key_c[61];
                strcpy(key_c, iter.c_str());
                book_.Keyword_map.insert(key_c, tmp_.ISBN, tmp_.tag);
            }
            std::string order = "change Keyword to " + Key;
            log_.AddLog5(account_.login_stack.top().Privilege, account_.login_stack.top().UserID, 9, tmpBook.ISBN, order);
        }
    }

    void import(AccountSystem &account_, BookSystem &book_, LogSystem &log_) {
        if (account_.login_stack.top().Privilege < '3') throw std::string("Invalid\n");

        int index_ = account_.User_select.top();
        if (index_ == -1) throw std::string("Invalid\n");

        std::string tmp = nextToken();
        int Quantity_ = check_Type6(tmp);

        tmp = nextToken();
        check_Type7(tmp);
        double TotalCost_ = atof(tmp.c_str());

        book_.import(Quantity_, TotalCost_, account_);

        log_.AddFinance(false, TotalCost_);

        Book tmpBook;
        book_.readFile(tmpBook, index_);
        log_.AddLog3(account_.login_stack.top().Privilege, account_.login_stack.top().UserID, 7, tmpBook.ISBN, Quantity_,
                     TotalCost_);
    }

    void show_finance(AccountSystem &account_, LogSystem &log_) {
        if (account_.login_stack.top().Privilege < '7') throw std::string("Invalid\n");
        std::string tmp = nextToken();
        if (tmp.empty()) {
            log_.ShowFinance(-1);
            return;
        }
        int Quantity_ = check_Type6(tmp);
        log_.ShowFinance(Quantity_);
    }

};

#endif //INC_1_6BOOKSTORE_TOKENSCANNER_H
