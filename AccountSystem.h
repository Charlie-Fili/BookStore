#ifndef INC_1_6BOOKSTORE_ACCOUNTSYSTEM_H
#define INC_1_6BOOKSTORE_ACCOUNTSYSTEM_H

#include <iostream>
#include <fstream>
//#include <string>
//#include <cstring>
#include <vector>
#include <stack>
#include <unordered_map>
//#include "LogSystem.h"
#include "UnrolledLinkedList.h"

class User {
public:
    using account_type = char[30];
    int tag;
    char UserID[31];
    char Password[31];
    char Username[31];
    char Privilege;

    User() {
        memset(UserID, 0, sizeof(UserID));
        memset(Password, 0, sizeof(Password));
        memset(Username, 0, sizeof(Username));
    }

    User(const int &tag_, const char (&UserID_)[31], const char (&Password_)[31], const char (&Username_)[31],
         const char Privilege_) {
        memset(UserID, 0, sizeof(UserID));
        memset(Password, 0, sizeof(Password));
        memset(Username, 0, sizeof(Username));
        tag = tag_;
        strcpy(UserID, UserID_);
        strcpy(Password, Password_);
        strcpy(Username, Username_);
        Privilege = Privilege_;
    }

    User(const User &rhs) {
        memset(UserID, 0, sizeof(UserID));
        memset(Password, 0, sizeof(Password));
        memset(Username, 0, sizeof(Username));
        tag = rhs.tag;
        strcpy(UserID, rhs.UserID);
        strcpy(Password, rhs.Password);
        strcpy(Username, rhs.Username);
        Privilege = rhs.Privilege;
    }

    User &operator=(const User &rhs) {
        if (this == &rhs) return *this;
        tag = rhs.tag;
        memset(UserID, 0, sizeof(UserID));
        memset(Password, 0, sizeof(Password));
        memset(Username, 0, sizeof(Username));
        strcpy(UserID, rhs.UserID);
        strcpy(Password, rhs.Password);
        strcpy(Username, rhs.Username);
        Privilege = rhs.Privilege;
        return *this;
    }
};

//struct user_select {
//    User user_;
//    int book_tag;
//};

class AccountSystem {
public:
    BlockList<char[31]> user_map;
    std::fstream User_inf;
    std::string filename_ = "User_inf";
    std::stack<User> login_stack;
    std::unordered_map<std::string, int> if_login;
//    std::vector<user_select> User_select;
    std::vector<int> User_select;
    int amount = 0;

    AccountSystem() : user_map("UserIdHead", "UserIdBody") {
        User_inf.open(filename_);
        if (!User_inf.good()) { // 是否成功打开
            User_inf.open(filename_, std::fstream::out); // 新建
            User_inf.close();
            User_inf.open(filename_); // 变为可读可写
            char UserID_[31] = "root";
            char Password_[31] = "sjtu";
            char Username_[31] = "root";
            User root(0, UserID_, Password_, Username_, '7');
            User_inf.seekp(sizeof(amount));
            User_inf.write(reinterpret_cast <char *> (&root), sizeof(User));
        } else {
            User_inf.seekg(0);
            User_inf.read(reinterpret_cast <char *> (&amount), sizeof(amount));
        }
    }

    ~AccountSystem() {
        User_inf.seekp(0);
        User_inf.write(reinterpret_cast <char *> (&amount), sizeof(amount));
        User_inf.close();
    }

    void readFile(User &read_, const int &location) {
        User_inf.seekg(sizeof(User) * location + sizeof(amount));
        User_inf.read(reinterpret_cast<char *>(&read_), sizeof(User));
    }

    void writeFile(User &write_, const int &location) {
        User_inf.seekp(sizeof(User) * location + sizeof(amount));
        User_inf.write(reinterpret_cast<char *>(&write_), sizeof(User));
    }

    void login(const char (&UserID_)[31], const char (&Password_)[31]) { //登录
        int index_ = user_map.search(UserID_);
        if (index_ == -1) {
            std::cout << "Invalid\n";
            return;
        }
        if (login_stack.empty()) {
            User record;
            readFile(record, index_);
            if (strcmp(Password_, record.Password) == 0) {
                login_stack.push(record);
//                User_select.push_back((user_select){record,-1});
                User_select.push_back(-1);
                if_login[UserID_] = 1;
            } else std::cout << "Invalid\n";
        } else {
            User tmp(login_stack.top());
            User record;
            readFile(record, index_);
            if (tmp.Privilege > record.Privilege) {
                login_stack.push(record);
//                User_select.push_back((user_select){record,-1});
                User_select.push_back(-1);
            }
            else {
                if (strcmp(Password_, record.Password) == 0) {
                    login_stack.push(record);
//                    User_select.push_back((user_select){record,-1});
                    User_select.push_back(-1);
                } else std::cout << "Invalid\n";
            }
            if (if_login.count(UserID_)) ++if_login[UserID_];
            else if_login[UserID_] = 1;
        }

    }

    void logout() {
        if (login_stack.empty()) {
            std::cout << "Invalid\n";
        } else {
            if (if_login[login_stack.top().UserID] > 1) --if_login[login_stack.top().UserID];
            else if_login.erase(login_stack.top().UserID);
            login_stack.pop();
            User_select.pop_back();
        }
    }

    void register_(const char (&UserID_)[31], const char (&Password_)[31], const char (&Username_)[31]) { //注册
        int index_ = user_map.search(UserID_);
        if (index_==-1) {
            std::cout << "Invalid\n";
            return;
        }
        user_map.insert(UserID_, amount);
        User tmp(amount, UserID_, Password_, Username_, '1');
        writeFile(tmp, amount);
        ++amount;
    }

    void passwd(const char (&UserID_)[31], const char (&CurrentPassword_)[31], const char (&NewPassword_)[31]) {
        if (login_stack.empty()) {
            std::cout << "Invalid\n";
        } else {
            int index_ = user_map.search(UserID_);
            if (index_==-1) {
                std::cout << "Invalid\n";
                return;
            }
            User tmp(login_stack.top());
            User record;
            readFile(record, index_);
            if (tmp.Privilege == '7') {
                strcpy(record.Password, NewPassword_);
                writeFile(record, index_);
            } else {
                if (strcmp(CurrentPassword_, record.Password) == 0) {
                    strcpy(record.Password, NewPassword_);
                    writeFile(record, index_);
                } else std::cout << "Invalid\n";
            }
        }
    }

    void useradd(const char (&UserID_)[31], const char (&Password_)[31], const char &Privilege_,
                 const char (&Username_)[31]) {
        if (login_stack.empty()) {
            std::cout << "Invalid\n";
        } else {
            User tmp(login_stack.top());
            if (tmp.Privilege == '1') {
                std::cout << "Invalid\n";
                return;
            }
            int index_ = user_map.search(UserID_);
            if (index_==-1) {
                std::cout << "Invalid\n";
                return;
            }
            User record;
            readFile(record, index_);
            if (tmp.Privilege >= record.Privilege) std::cout << "Invalid\n";
            else {
                user_map.insert(UserID_, amount);
                User add(amount, UserID_, Password_, Username_, Privilege_);
                writeFile(add, amount);
                ++amount;
            }
        }
    }

    void delete_(const char (&UserID_)[31]) {
        if (login_stack.empty()) {
            std::cout << "Invalid\n";
        } else {
            User tmp(login_stack.top());
            if (tmp.Privilege != '7') {
                std::cout << "Invalid\n";
                return;
            }
            int index_ = user_map.search(UserID_);
            if (index_==-1) {
                std::cout << "Invalid\n";
                return;
            }
            User record;
            readFile(record, index_);
            if (if_login.count(record.UserID)) std::cout << "Invalid\n";
            else {
                user_map.erase(UserID_, record.tag);
            }
        }
    }
};

#endif //INC_1_6BOOKSTORE_ACCOUNTSYSTEM_H

