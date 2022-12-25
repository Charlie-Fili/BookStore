#ifndef INC_1_6BOOKSTORE_TOKENSCANNER_H
#define INC_1_6BOOKSTORE_TOKENSCANNER_H

#include <iostream>
#include <string>
#include <cstring>
//#include "AccountSystem.h"
#include "BookSystem.h"
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

    std::string nextToken() {
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

    std::string nextKey() {
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

    void su(AccountSystem &account_) {
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
        }

        if (hasMore) throw std::string("Invalid\n");

        account_.login(UserID_, Password_);
    }

    void register_(AccountSystem &account_) {
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
    }

    void passwd(AccountSystem &account_) {
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
            check_Type1(tmp);
            strcpy(NewPassword_, tmp.c_str());
        } else {
            check_Type1(tmp);
            check_Type1(tmp1);
            strcpy(CurrentPassword_, tmp.c_str());
            strcpy(NewPassword_, tmp.c_str());
        }

        if (hasMore) throw std::string("Invalid\n");

        account_.passwd(UserID_, CurrentPassword_, NewPassword_);
    }

    void useradd(AccountSystem &account_) {
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
    }

    void delete_(AccountSystem &account_) {
        std::string tmp = nextToken();
        check_Type1(tmp);
        char UserID_[31];
        memset(UserID_, 0, sizeof(UserID_));
        strcpy(UserID_, tmp.c_str());

        if (hasMore) throw std::string("Invalid\n");

        account_.delete_(UserID_);
    }

    /*
     * book system
     */

    void show(AccountSystem &account_, BookSystem &book_) {
        std::string tmp = nextToken();
        if (!hasMore) {
            book_.show(account_);
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

            if (type[1] == 'n') {
                book_.BookName_show(type, account_);
            } else if (type[1] == 'a') {
                book_.Author_show(type, account_);
            } else if (type[1] == 'k') {
                book_.Keyword_show(type, account_);
            } else throw std::string("Invalid\n");
        }
    }

    void buy(AccountSystem &account_, BookSystem &book_) {
        std::string tmp = nextToken();
        check_Type4(tmp);
        char ISBN_[21];
        memset(ISBN_, 0, sizeof(ISBN_));
        strcpy(ISBN_, tmp.c_str());

        tmp = nextToken();
        int Quantity_ = check_Type6(tmp);

        book_.buy(ISBN_, Quantity_, account_);
    }

    void select(AccountSystem &account_, BookSystem &book_) {
        if (account_.login_stack.top().Privilege < '3') throw std::string("Invalid\n");
        std::string tmp = nextToken();
        check_Type4(tmp);
        char ISBN_[21];
        memset(ISBN_, 0, sizeof(ISBN_));
        strcpy(ISBN_, tmp.c_str());

        book_.select(ISBN_, account_);
    }

    void modify(AccountSystem &account_, BookSystem &book_) {
        if (account_.login_stack.top().Privilege < '3') throw std::string("Invalid\n");
        int index_=account_.User_select[account_.User_select.size() - 1];
        if (index_ == -1) throw std::string("Invalid\n");
        if (!hasMore) throw std::string("Invalid\n");
        std::string tmp = nextToken();
        bool used[4];
        while (!tmp.empty()) {
            if (tmp.length() < 6) throw std::string("Invalid\n");
            if (tmp[1] == 'I') {
                if (used[0]) throw std::string("Invalid\n");
                char ISBN_[21];
                memset(ISBN_, 0, sizeof(ISBN_));
                strcpy(ISBN_, check_assign_ISBN(tmp).c_str());

                used[0] = true;

                book_.ISBN_modify(ISBN_, account_);
            } else if (tmp[1] == 'k') {
                if (used[3]) throw std::string("Invalid\n");
                char Keyword_[21];
                memset(Keyword_, 0, sizeof(Keyword_));
                if(tmp.length()<11) throw std::string("Invalid\n");
                std::string temp=tmp.substr(1,9);
                if(temp!="keyword=\"" || tmp[tmp.size()-1]!='\"') throw std::string("Invalid\n");
                temp=tmp.substr(10,tmp.length()-11);
                check_key(temp);
                used[3] = true;
                TokenScanner scanner_key(tmp);
                std::string key=scanner_key.nextToken();
                std::unordered_map<std::string,bool> hasKey;
                std::vector<std::string> Key_store;
                while(!key.empty()){
                    if(hasKey.count(key)) throw std::string("Invalid\n");
                    hasKey[key]=true;
                    Key_store.push_back(key);
                }
                for(auto &iter:Key_store){
                    book_.Keyword_modify(iter.c_str(), account_);
                }
            } else {
                char type[61];
                memset(type, 0, sizeof(type));
                strcpy(type, check_assign(tmp).c_str());

                if (type[1] == 'n') {
                    if (used[1]) throw std::string("Invalid\n");
                    used[1]=true;
                    book_.BookName_show(type, account_);
                } else if (type[1] == 'a') {
                    if (used[2]) throw std::string("Invalid\n");
                    used[2]=true;
                    book_.Author_show(type, account_);
                } else if (type[1] == 'p') {
                    if (used[4]) throw std::string("Invalid\n");
                    used[4]=true;
                    book_.Keyword_show(type, account_);
                } else throw std::string("Invalid\n");
            }
            tmp = nextToken();
        }
    }


    void import(AccountSystem &account_, BookSystem &book_) {
        if (account_.login_stack.top().Privilege < '3') throw std::string("Invalid\n");
        int index_=account_.User_select[account_.User_select.size() - 1];
        if (index_ == -1) throw std::string("Invalid\n");

        std::string tmp = nextToken();
        int Quantity_ = check_Type6(tmp);

        tmp = nextToken();
        int TotalCost_ = check_Type6(tmp);

        book_.import(Quantity_,TotalCost_,account_);
    }

    void show_finance(AccountSystem &account_, BookSystem &book_) {
        if (account_.login_stack.top().Privilege < '7') throw std::string("Invalid\n");

    }

    void log(AccountSystem &account_, BookSystem &book_) {
        if (account_.login_stack.top().Privilege < '7') throw std::string("Invalid\n");

    }

};

#endif //INC_1_6BOOKSTORE_TOKENSCANNER_H
