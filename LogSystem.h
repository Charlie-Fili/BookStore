#ifndef INC_1_6BOOKSTORE_LOGSYSTEM_H
#define INC_1_6BOOKSTORE_LOGSYSTEM_H

#include <iostream>
#include <fstream>
#include <vector>

#include "BookSystem.h"


struct Log_finance {
    bool if_earn;
    double value;

    Log_finance() = default;

    Log_finance(const bool &if_earn_, const double &value_) {
        if_earn = if_earn_;
        value = value_;
    }
};

struct Log {
    char privilege1 = ' ';
    char account[31];
    int behaviour;
//  Login, Logout, Register, Passwd, Useradd, Delete, Show, Buy, Select, Modify, Import
    char privilege2 = ' ';
    char acceptor[31];
    char ISBN[21];
    char show[71];
    int num = 0;
    double amount = 0.0;

    Log() = default;

    Log(const char &privilege1_, const char (&account_)[31], const int &behavior_) {
        privilege1 = privilege1_;
        memset(account, 0, sizeof(account));
        strcpy(account, account_);
        behaviour = behavior_;
        memset(acceptor, 0, sizeof(acceptor));
        memset(ISBN, 0, sizeof(ISBN));
        memset(show, 0, sizeof(show));
    }

    Log(const char &privilege1_, const char (&account_)[31], const int &behavior_, const char &privilege2_,
        const char (&acceptor_)[31]) {
        privilege1 = privilege1_;
        memset(account, 0, sizeof(account));
        strcpy(account, account_);
        behaviour = behavior_;
        privilege2 = privilege2_;
        memset(acceptor, 0, sizeof(acceptor));
        strcpy(acceptor, acceptor_);
        memset(ISBN, 0, sizeof(ISBN));
        memset(show, 0, sizeof(show));
    }

    Log(const char &privilege1_, const char (&account_)[31], const int &behavior_, const std::string &show_) {
        privilege1 = privilege1_;
        memset(account, 0, sizeof(account));
        strcpy(account, account_);
        behaviour = behavior_;
        memset(acceptor, 0, sizeof(acceptor));
        memset(ISBN, 0, sizeof(ISBN));
        memset(show, 0, sizeof(show));
        strcpy(show, show_.c_str());
    }

    Log(const char &privilege1_, const char (&account_)[31], const int &behavior_, const char (&ISBN_)[21],
        const int &num_, const double &amount_) {
        privilege1 = privilege1_;
        memset(account, 0, sizeof(account));
        strcpy(account, account_);
        behaviour = behavior_;
        memset(acceptor, 0, sizeof(acceptor));
        memset(ISBN, 0, sizeof(ISBN));
        strcpy(ISBN, ISBN_);
        num = num_;
        amount = amount_;
    }

    Log(const char &privilege1_, const char (&account_)[31], const int &behavior_, const char (&ISBN_)[21]) {
        privilege1 = privilege1_;
        memset(account, 0, sizeof(account));
        strcpy(account, account_);
        behaviour = behavior_;
        memset(acceptor, 0, sizeof(acceptor));
        memset(ISBN, 0, sizeof(ISBN));
        strcpy(ISBN, ISBN_);
    }


    Log(const char &privilege1_, const char (&account_)[31], const int &behavior_, const char (&ISBN_)[21],
        const std::string &show_) {
        privilege1 = privilege1_;
        memset(account, 0, sizeof(account));
        strcpy(account, account_);
        behaviour = behavior_;
        memset(acceptor, 0, sizeof(acceptor));
        memset(ISBN, 0, sizeof(ISBN));
        strcpy(ISBN, ISBN_);
        memset(show, 0, sizeof(show));
        strcpy(show, show_.c_str());
    }
    //    Log(const char &privilege1_, const char (&account_)[31], const int &behavior_, const char &privilege2_,
//        const char (&acceptor_)[31], const char (&ISBN_)[21], const std::string &show_,
//        const int &num_, const double &amount_) {
//        privilege1 = privilege1_;
//        memset(account, 0, sizeof(account));
//        strcpy(account, account_);
//        behaviour = behavior_;
//        privilege2 = privilege2_;
//        memset(acceptor, 0, sizeof(acceptor));
//        strcpy(acceptor, acceptor_);
//        memset(ISBN, 0, sizeof(ISBN));
//        strcpy(ISBN, ISBN_);
//        memset(show, 0, sizeof(show));
//        strcpy(show, show_.c_str());
//        num = num_;
//        amount = amount_;
//    }

};

class LogSystem {
public:
    std::fstream log_finance;
    std::string filename1_ = "log_finance";
    int finance_amount = 0;
    std::fstream log;
    std::string filename2_ = "log";
    int amount = 0;
public:
    LogSystem() {
        log_finance.open(filename1_);
        if (!log_finance.good()) { // 是否成功打开
            log_finance.open(filename1_, std::fstream::out); // 新建
            log_finance.close();
            log_finance.open(filename1_); // 变为可读可写
        } else {
            log_finance.seekg(0);
            log_finance.read(reinterpret_cast <char *> (&finance_amount), sizeof(finance_amount));
        }
        log.open(filename2_);
        if (!log.good()) { // 是否成功打开
            log.open(filename2_, std::fstream::out); // 新建
            log.close();
            log.open(filename2_); // 变为可读可写
        } else {
            log.seekg(0);
            log.read(reinterpret_cast <char *> (&amount), sizeof(finance_amount));
        }
    }

    ~LogSystem() {
        log_finance.seekp(0);
        log_finance.write(reinterpret_cast <char *> (&finance_amount), sizeof(finance_amount));
        log_finance.close();
        log.seekp(0);
        log.write(reinterpret_cast <char *> (&amount), sizeof(amount));
        log.close();
    }

    void readFile(Log_finance &read_, const int &location) {
        log_finance.seekg(sizeof(Log_finance) * location + sizeof(int));
        log_finance.read(reinterpret_cast<char *>(&read_), sizeof(Log_finance));
    }

    void writeFile(Log_finance &write_, const int &location) {
        log_finance.seekp(sizeof(Log_finance) * location + sizeof(int));
        log_finance.write(reinterpret_cast<char *>(&write_), sizeof(Log_finance));
    }

    void AddFinance(const bool &if_earn_, const double &value_) {
        Log_finance tmp(if_earn_, value_);
        writeFile(tmp, finance_amount);
        ++finance_amount;
    }

    void ShowFinance(const int &count_) {
        if (count_ > finance_amount) throw std::string("Invalid\n");
        double earn = 0.0;
        double expense = 0.0;
        if (count_ == -1) {
            for (int i = finance_amount - 1; i >= 0; --i) {
                Log_finance tmp;
                readFile(tmp, i);
                if (tmp.if_earn) earn += tmp.value;
                else expense += tmp.value;
            }
        } else {
            for (int i = finance_amount - 1; i >= finance_amount - count_; --i) {
                Log_finance tmp;
                readFile(tmp, i);
                if (tmp.if_earn) earn += tmp.value;
                else expense += tmp.value;
            }
        }
        std::cout.setf(std::ios::fixed);
        std::cout << "+ ";
        std::cout << std::setprecision(2) << earn;
        std::cout << " - ";
        std::cout << std::setprecision(2) << expense << std::endl;
    }

    void readLog(Log &read_, const int &location) {
        log.seekg(sizeof(Log) * location + sizeof(int));
        log.read(reinterpret_cast<char *>(&read_), sizeof(Log));
    }

    void writeLog(Log &write_, const int &location) {
        log.seekp(sizeof(Log) * location + sizeof(int));
        log.write(reinterpret_cast<char *>(&write_), sizeof(Log));
    }

    void AddLog0(const char &privilege1_, const char (&account_)[31], const int &behavior_) {
        Log tmp(privilege1_, account_, behavior_);
        writeLog(tmp, amount);
        ++amount;
    }

    void AddLog1(const char &privilege1_, const char (&account_)[31], const int &behavior_, const char &privilege2_,
                 const char (&acceptor_)[31]) {
        Log tmp(privilege1_, account_, behavior_, privilege2_, acceptor_);
        writeLog(tmp, amount);
        ++amount;
    }

    void AddLog2(const char &privilege1_, const char (&account_)[31], const int &behavior_, const std::string &show_) {
        Log tmp(privilege1_, account_, behavior_, show_);
        writeLog(tmp, amount);
        ++amount;
    }

    void AddLog3(const char &privilege1_, const char (&account_)[31], const int &behavior_, const char (&ISBN_)[21],
                 const int &num_, const double &amount_) {
        Log tmp(privilege1_, account_, behavior_, ISBN_, num_, amount_);
        writeLog(tmp, amount);
        ++amount;
    }

    void AddLog4(const char &privilege1_, const char (&account_)[31], const int &behavior_, const char (&ISBN_)[21]) {
        Log tmp(privilege1_, account_, behavior_, ISBN_);
        writeLog(tmp, amount);
        ++amount;
    }

    void AddLog5(const char &privilege1_, const char (&account_)[31], const int &behavior_, const char (&ISBN_)[21],
                 const std::string &show_) {
        Log tmp(privilege1_, account_, behavior_, ISBN_, show_);
        writeLog(tmp, amount);
        ++amount;
    }
//    void AddLog0(const char &privilege1_, const char (&account_)[31], const int &behavior_, const char &privilege2_,
//                 const char (&acceptor_)[31], const char (&ISBN_)[21], const char (&bookName_)[61],
//                 const std::string &show_,
//                 const int &num_, const double &amount_) {
//        Log tmp(privilege1_, account_, behavior_, privilege2_, acceptor_, ISBN_, bookName_, show_, num_, amount_);
//        writeLog(tmp, amount);
//        ++amount;
//    }

    void showLog() {
        if (amount == 0) throw std::string("Invalid\n");
        for (int i = 0; i < amount; ++i) {
            Log tmp;
            readLog(tmp, i);
            int operation = tmp.behaviour;
            //  Login, Logout, Register, Passwd, Useradd, Delete, Show, Buy, Select, Modify, Import
            if (operation == 0) {
                showPrivilege(tmp.privilege1 - '0');
                std::cout << tmp.account << '\t' << "\033[32mLogin\n";
            } else if (operation == 1) {
                showPrivilege(tmp.privilege1 - '0');
                std::cout << tmp.account << '\t' << "\033[32mLogout\n";
            } else if (operation == 2) {
                showPrivilege(tmp.privilege1 - '0');
                std::cout << tmp.account << '\t' << "\033[32mRegister\n";
            } else if (operation == 3) {
                showPrivilege(tmp.privilege1 - '0');
                std::cout << tmp.account << '\t' << "\033[32mPasswd\t";
                showPrivilege(tmp.privilege2 - '0');
                std::cout << tmp.acceptor << '\n';
            } else if (operation == 4) {
                showPrivilege(tmp.privilege1 - '0');
                std::cout << tmp.account << '\t' << "\033[32mUseradd\t";
                showPrivilege(tmp.privilege2 - '0');
                std::cout << tmp.acceptor << '\n';
            } else if (operation == 5) {
                showPrivilege(tmp.privilege1 - '0');
                std::cout << tmp.account << '\t' << "\033[32mDelete\t";
                showPrivilege(tmp.privilege2 - '0');
                std::cout << tmp.acceptor << '\n';
            } else if (operation == 6) {
                showPrivilege(tmp.privilege1 - '0');
                std::cout << tmp.account << '\t' << "\033[32mShow\t" << tmp.show << '\n';
            } else if (operation == 7) {
                showPrivilege(tmp.privilege1 - '0');
                std::cout << tmp.account << '\t' << "\033[32mBuy\t" << tmp.ISBN << '\t'
                          << tmp.num << "本\t\033[33mCost:" << tmp.amount << '\n';
            } else if (operation == 8) {
                showPrivilege(tmp.privilege1 - '0');
                std::cout << tmp.account << '\t' << "\033[32mSelect\t" << tmp.ISBN << '\n';
            } else if (operation == 9) {
                showPrivilege(tmp.privilege1 - '0');
                std::cout << tmp.account << '\t' << "\033[32mModify\t" << tmp.ISBN
                          << "\t\033[32mBy\t" << tmp.show << '\n';
            } else if (operation == 10) {
                showPrivilege(tmp.privilege1 - '0');
                std::cout << tmp.account << '\t' << "\033[32mImport\t" << tmp.ISBN << '\t'
                          << tmp.num << "本\t\033[33mCost:" << tmp.amount << '\n';
            }
        }
    }

    void showPrivilege(const int &Privilege) {
        if (Privilege == 1) std::cout << "\033[34m顾客";
        else if (Privilege == 3) std::cout << "\033[34m员工";
        else if (Privilege == 7) std::cout << "\033[34m店主";
    }

};

#endif //INC_1_6BOOKSTORE_LOGSYSTEM_H
