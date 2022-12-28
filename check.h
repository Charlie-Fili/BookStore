#ifndef INC_1_6BOOKSTORE_CHECK_H
#define INC_1_6BOOKSTORE_CHECK_H

#include <iostream>
#include <string>
#include <cctype>
#include <cstring>

//[UserID], [Password], [CurrentPassword], [NewPassword]
//合法字符集：数字，字母，下划线；
//最大长度：30。
void check_Type1(std::string &check) {
    if (check.length() > 30 || check.empty()) throw std::string("Invalid\n");
    for (char tmp: check) {
        if ((!isdigit(tmp)) && (!isalpha(tmp)) && (tmp != '_'))
            throw std::string("Invalid\n");
    }
}

//[Username]
//合法字符集：除不可见字符以外 ASCII 字符；
//最大长度：30。
void check_Type2(std::string &check) {
    if (check.length() > 30 || check.empty()) throw std::string("Invalid\n");
    for (char i: check) {
        if (!(isascii(i) && isprint(i)))
            throw std::string("Invalid\n");
    }
}

//[Privilege]
//合法字符集：数字；
//最大长度：1。
void check_Type3(const std::string &check) {
    if (check.length() > 1 || check.empty()) throw std::string("Invalid\n");
    if (!isdigit(check[0])) throw std::string("Invalid\n");
    char tmp = check[0];
    if (!(tmp == '0' || tmp == '1' || tmp == '3' || tmp == '7')) throw std::string("Invalid\n");
}

//[ISBN]
//合法字符集：除不可见字符以外 ASCII 字符；
//最大长度：20；
void check_Type4(const std::string &check) {
    if (check.length() > 20 || check.empty()) throw std::string("Invalid\n");
    for (char i: check) {
        if (!(isascii(i) && isprint(i)))
            throw std::string("Invalid\n");
    }
}

//[BookName], [Author],[Keyword]
//合法字符集：除不可见字符和英文双引号以外 ASCII 字符；
//最大长度：60。
void check_Type5(const std::string &check) {
    if (check.length() > 60 || check.empty()) throw std::string("Invalid\n");
    for (char i: check) {
        if ((!isascii(i)) || (!isprint(i)) || i == '"')
            throw std::string("Invalid\n");
    }
}

void check_key(const std::string &check) {
    if (check.length() > 60 || check.empty()) throw std::string("Invalid\n");
    if (check[0] == '|' or check[check.length() - 1] == '|') throw std::string("Invalid\n");
    for (int i = 0; i < check.length(); ++i) {
        if ((!isascii(check[i])) || (!isprint(check[i])) || check[i] == '"')
            throw std::string("Invalid\n");
        if (check[i] == '|' && check[i + 1] == '|') throw std::string("Invalid\n");
    }
}

//[Quantity]
//合法字符集：数字；
//最大长度：10；
int check_Type6(const std::string &check) {
    if (check.length() > 10 || check.empty()) throw std::string("Invalid\n");
    if (!isdigit(check[0]) or check[0] == '0') throw std::string("Invalid\n");
    for (char i: check) {
        if (!isdigit(i))
            throw std::string("Invalid\n");
    }
    long long int tmp = atoi(check.c_str());
    if (tmp > 2147483647) throw std::string("Invalid\n");
    else return (int) tmp;
}

//[Price], [TotalCost]
//合法字符集：数字和 .；
//最大长度：13；
//特殊说明：本系统中浮点数输入输出精度固定
void check_Type7(const std::string &check) {
    if (check.length() > 13 || check.empty()) throw std::string("Invalid\n");
    if (check[0] == '0' and check.length() > 1 and check[1] != '.') throw std::string("Invalid\n");
    int num = 0;
    int point = -1;
    for (int i = 0; i < check.length(); ++i) {
        if ((!isdigit(check[i])) && check[i] != '.')
            throw std::string("Invalid\n");
        if (check[i] == '.') {
            ++num;
            point = i;
        }
        if (num > 1) throw std::string("Invalid\n");
    }
    if (point == 0 or point == check.length() - 1) throw std::string("Invalid\n");
}

std::string check_assign_ISBN(const std::string &check) {
    if (check[0] == '-') {
        if (check.length() < 7) throw std::string("Invalid\n");
        std::string tmp = check.substr(1, 5);
        if (tmp != "ISBN=") throw std::string("Invalid\n");
        tmp = check.substr(6, check.length() - 6);
        check_Type4(tmp);
        return tmp;
    } else throw std::string("Invalid\n");
}

std::string check_assign(const std::string &check) {
    if (check[0] == '-') {
        if (check[1] == 'n') {
            if (check.length() < 8) throw std::string("Invalid\n");
            std::string tmp = check.substr(1, 6);
            if (tmp != "name=\"" || check[check.size() - 1] != '\"') throw std::string("Invalid\n");
            tmp = check.substr(7, check.length() - 8);
            check_Type5(tmp);
            return tmp;
        } else if (check[1] == 'a') {
            if (check.length() < 10) throw std::string("Invalid\n");
            std::string tmp = check.substr(1, 8);
            if (tmp != "author=\"" || check[check.size() - 1] != '\"') throw std::string("Invalid\n");
            tmp = check.substr(9, check.length() - 10);
            check_Type5(tmp);
            return tmp;
        } else if (check[1] == 'k') {
            if (check.length() < 11) throw std::string("Invalid\n");
            std::string tmp = check.substr(1, 9);
            if (tmp != "keyword=\"" || check[check.size() - 1] != '\"') throw std::string("Invalid\n");
            tmp = check.substr(10, check.length() - 11);
            check_Type5(tmp);
            return tmp;
        } else if (check[1] == 'p') {
            if (check.length() < 8) throw std::string("Invalid\n");
            std::string tmp = check.substr(1, 6);
            if (tmp != "price=") throw std::string("Invalid\n");
            tmp = check.substr(7, check.length() - 7);
            check_Type7(tmp);
            return tmp;
        } else throw std::string("Invalid\n");
    } else throw std::string("Invalid\n");
}

#endif //INC_1_6BOOKSTORE_CHECK_H
