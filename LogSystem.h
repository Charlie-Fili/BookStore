#ifndef INC_1_6BOOKSTORE_LOGSYSTEM_H
#define INC_1_6BOOKSTORE_LOGSYSTEM_H

#include <iostream>
#include <fstream>
#include <vector>

#include "BookSystem.h"

struct Log_finance{
    bool if_earn;
    double value;

    Log_finance()=default;
    Log_finance(const bool &if_earn_,const double &value_){
        if_earn=if_earn_;
        value=value_;
    }
};

class LogSystem{
public:
    std::fstream log_finance;
    std::string filename_ = "log_finance";
    int finance_amount = 0;
public:
    LogSystem(){
        log_finance.open(filename_);
        if (!log_finance.good()) { // 是否成功打开
            log_finance.open(filename_, std::fstream::out); // 新建
            log_finance.close();
            log_finance.open(filename_); // 变为可读可写
        } else {
            log_finance.seekg(0);
            log_finance.read(reinterpret_cast <char *> (&finance_amount), sizeof(finance_amount));
        }
    }

    ~LogSystem() {
        log_finance.seekp(0);
        log_finance.write(reinterpret_cast <char *> (&finance_amount), sizeof(finance_amount));
        log_finance.close();
    }

    void readFile(Log_finance &read_, const int &location) {
        log_finance.seekg(sizeof(Log_finance) * location + sizeof(int));
        log_finance.read(reinterpret_cast<char *>(&read_), sizeof(Log_finance));
    }

    void writeFile(Log_finance &write_, const int &location) {
        log_finance.seekp(sizeof(Log_finance) * location + sizeof(int));
        log_finance.write(reinterpret_cast<char *>(&write_), sizeof(Log_finance));
    }

    void AddFinance(const bool &if_earn_,const double &value_){
        Log_finance tmp(if_earn_,value_);
        writeFile(tmp,finance_amount);
        ++finance_amount;
    }

    void ShowFinance(const int &count_){
        if(count_>finance_amount) throw std::string("Invalid\n");
        double earn=0.0;
        double expense=0.0;
        if(count_==-1){
            for(int i=finance_amount-1;i>=0;--i){
                Log_finance tmp;
                readFile(tmp,i);
                if(tmp.if_earn) earn+=tmp.value;
                else expense+=tmp.value;
            }
        }else{
            for(int i=finance_amount-1;i>=finance_amount-count_;--i){
                Log_finance tmp;
                readFile(tmp,i);
                if(tmp.if_earn) earn+=tmp.value;
                else expense+=tmp.value;
            }
        }
        std::cout.setf(std::ios::fixed);
        std::cout << "+ ";
        std::cout << std::setprecision(2) << earn;
        std::cout << " - ";
        std::cout << std::setprecision(2) << expense << std::endl;
    }

};
#endif //INC_1_6BOOKSTORE_LOGSYSTEM_H
