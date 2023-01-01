#ifndef INC_1_6BOOKSTORE_ACCOUNTSYSTEM_H
#define INC_1_6BOOKSTORE_ACCOUNTSYSTEM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <unordered_map>
#include "UnrolledLinkedList.h"

class User {
public:
    int tag = -1;
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

class AccountSystem {
public:
    BlockList<char[31]> user_map;
    std::fstream User_inf;
    std::string filename_ = "User_inf";
    std::stack<User> login_stack;
    std::stack<int> User_select;
    std::unordered_map<std::string, int> if_login;
    int amount = 0;

    AccountSystem() : user_map("UserIdBody", "UserIdHead") {
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
            user_map.insert(UserID_, 0);
            ++amount;
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

    void login(const char (&UserID_)[31], const char (&Password_)[31], const bool &ifOmit) { //登录账户
        int index_ = user_map.search(UserID_);
        if (index_ == -1) throw std::string("Invalid\n");

        if (login_stack.empty()) {
            User record;
            readFile(record, index_);
            if (strcmp(Password_, record.Password) == 0) {
                login_stack.push(record);
                User_select.push(-1);
                if_login[UserID_] = 1;
            } else throw std::string("Invalid\n");
        } else {
            User tmp(login_stack.top());
            User record;
            readFile(record, index_);
            if (tmp.Privilege > record.Privilege and ifOmit) {
                login_stack.push(record);
                User_select.push(-1);
            } else {
                if (strcmp(Password_, record.Password) == 0) {
                    login_stack.push(record);
                    User_select.push(-1);
                } else throw std::string("Invalid\n");
            }
            if (if_login.count(UserID_)) ++if_login[UserID_];
            else if_login[UserID_] = 1;
        }
    }

    void logout() { // 注销账户
        if (login_stack.empty()) throw std::string("Invalid\n");
        else {
            if (if_login[login_stack.top().UserID] > 1) --if_login[login_stack.top().UserID];
            else if_login.erase(login_stack.top().UserID);
            login_stack.pop();
            User_select.pop();
        }
    }

    void register_(const char (&UserID_)[31], const char (&Password_)[31], const char (&Username_)[31]) { //注册账户
        int index_ = user_map.search(UserID_);
        if (index_ != -1) throw std::string("Invalid\n");
        user_map.insert(UserID_, amount);
        User tmp(amount, UserID_, Password_, Username_, '1');
        writeFile(tmp, amount);
        ++amount;
    }

    void passwd(const char (&UserID_)[31], const char (&CurrentPassword_)[31], const char (&NewPassword_)[31],
                const bool &ifOmit) { // 修改密码
        if (login_stack.empty()) throw std::string("Invalid\n");
        else {
            int index_ = user_map.search(UserID_);
            if (index_ == -1) throw std::string("Invalid\n");
            User tmp(login_stack.top());
            User record;
            readFile(record, index_);
            if (tmp.Privilege == '7' and ifOmit) {
                strcpy(record.Password, NewPassword_);
                writeFile(record, index_);
            } else {
                if (strcmp(CurrentPassword_, record.Password) == 0) {
                    strcpy(record.Password, NewPassword_);
                    writeFile(record, index_);
                } else throw std::string("Invalid\n");
            }
        }
    }

    void useradd(const char (&UserID_)[31], const char (&Password_)[31], const char &Privilege_,
                 const char (&Username_)[31]) { // 创建账户
        if (login_stack.empty()) throw std::string("Invalid\n");
        else {
            User tmp(login_stack.top());
            if (tmp.Privilege == '1') throw std::string("Invalid\n");
            int index_ = user_map.search(UserID_);
            if (index_ != -1) throw std::string("Invalid\n");
            if (tmp.Privilege <= Privilege_) throw std::string("Invalid\n");
            else {
                user_map.insert(UserID_, amount);
                User add(amount, UserID_, Password_, Username_, Privilege_);
                writeFile(add, amount);
                ++amount;
            }
        }
    }

    void delete_(const char (&UserID_)[31]) { // 删除账户
        if (login_stack.empty()) throw std::string("Invalid\n");
        else {
            User tmp(login_stack.top());
            if (tmp.Privilege != '7') throw std::string("Invalid\n");
            int index_ = user_map.search(UserID_);
            if (index_ == -1) throw std::string("Invalid\n");
            User record;
            readFile(record, index_);
            if (if_login.count(record.UserID)) throw std::string("Invalid\n");
            else {
                user_map.erase(UserID_, record.tag);
            }
        }
    }
};

#endif //INC_1_6BOOKSTORE_ACCOUNTSYSTEM_H

