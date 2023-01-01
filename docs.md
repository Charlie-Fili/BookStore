# Bookstore开发文档
- 项目名称 ：Bookstore 2022/ 文档作者：杨茗焱
- 
## 程序功能概述 

-   设计并实现一个书店管理系统，具有实现基础指令、账户系统指令、图书系统指令、日志系统指令的功能。
-   具体参考[业务需求文档](requirements.md)

##  主体逻辑说明

- 较大需求为管理账号、图书信息和日志记录，创建三个类AccountSystem、BookSystem和LogSystem辅助处理
- 交互方式利用类似token scanner的方法逐行读入分析
- 
  ### 指令操作
- 基础指令-直接执行
- 账户系统指令
	- 存储顾客所有信息（UserID、Password、Username、Privilege）
	- 用类和派生类来体现不同权限 *(写代码过程中发现派生类很累赘，于是决定舍弃派生类)*
	- 存储从UserID对应到相应信息的索引（使用块状链表 tag->UserID)
	- 用栈记录登录账号
	- 记录选择的书
- 图书系统指令
	- 存储图书所有信息（ISBN、BookName、Author、Keyword、Quantity、Price、TotalCost)
	- 存储从ISBN对应到相应信息的索引（使用块状链表 tag->ISBN)
    - 存储从BookName、Author、Keyword对应到ISBN的链表(这里采用了双键块状链表，tag->(,ISBN))
- 日志系统指令
	- 记录交易、收支
	- 日志文件


## 代码文件结构

### main.cpp
```
  #include <iostream>
  #include <unordered_map>
  #include "TokenScanner.h"
```
- 读取指令
```
  AccountSystem accountSystem;
  BookSystem bookSystem;
  LogSystem logSystem;
```
### TokenScanner.h
//切片，参考StanfordCPP
```
#include <iostream>
#include <string>
#include <cstring>
#include "LogSystem.h"
#include "check.h"
```
- 切片并调用相应类与函数
```
class TokenScanner{
	std::string nextToken();
	std::string nextKey();
	void su(AccountSystem &account_, LogSystem &log_);
	void logout(AccountSystem &account_, LogSystem &log_);
	void register_(AccountSystem &account_, LogSystem &log_);
	void passwd(AccountSystem &account_, LogSystem &log_);
	void useradd(AccountSystem &account_, LogSystem &log_);
	void delete_(AccountSystem &account_, LogSystem &log_);
	void show(AccountSystem &account_, BookSystem &book_, LogSystem &log_);
	void buy(AccountSystem &account_, BookSystem &book_, LogSystem &log_);
	void select(AccountSystem &account_, BookSystem &book_, LogSystem &log_);
	void modify(AccountSystem &account_, BookSystem &book_, LogSystem &log_);
	void show_finance(AccountSystem &account_, LogSystem &log_);
}
```

### AccountSystem.h
```
#include <iostream>
#include <string>
#include <cstring>
#include "LogSystem.h"
#include "check.h"
```
- 细化登录、登出、注册、修改密码、删除的函数
```
class User {
public:
    int tag = -1;
    char UserID[31];
    char Password[31];
    char Username[31];
    char Privilege;
}
class AccountSystem {
public:
    BlockList<char[31]> user_map;
    std::fstream User_inf;
    std::string filename_ = "User_inf";
    std::stack<User> login_stack;
    std::stack<int> User_select;
    std::unordered_map<std::string, int> if_login;
    int amount = 0;
    ……细化函数
}
```

### BookSystem.h
```
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <iomanip>
#include "AccountSystem.h"
#include "UnrolledLinkedList_double.h"
```
- 细化检索、购买、选择、修改、进货的函数
```
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
}
class BookSystem {
public:
    BlockList<char[21]> ISBN_map;
    double_BlockList<char[61], char[21]> BookName_map;
    double_BlockList<char[61], char[21]> Author_map;
    double_BlockList<char[61], char[21]> Keyword_map;
    std::fstream Book_inf;
    std::string filename_ = "Book_inf";
    int amount = 0;
    ……细化函数
}
```

### LogSystem.h{
```
#include <iostream>
#include <fstream>
#include <vector>
#include "BookSystem.h"
```
- 细化登录、登出、注册、修改密码、删除的函数
```
struct Log_finance {
    bool if_earn;
    double value;
};
struct Log {
    char privilege1 = ' ';
    char account[31];
    int behaviour;
//从0到10分别为 Login, Logout, Register, Passwd, Useradd, Delete, Show, Buy, Select, Modify, Import
    char privilege2 = ' ';
    char acceptor[31];
    char ISBN[21];
    char show[71];
    int num = 0;
    double amount = 0.0;
}
class LogSystem {
public:
    std::fstream log_finance;
    std::string filename1_ = "log_finance";
    int finance_amount = 0;
    std::fstream log;
    std::string filename2_ = "log";
    int amount = 0;
    ……细化函数
}
```
### chech.h 
- 用于检查输入是否合法



## 文件存储说明
- 存储账户信息 User_inf
- 存储书本信息 Book_inf
- 存储财务记录 log_finance
- 存储日志信息 log
- 存储从UserID对应到相应信息的索引 UserIdHead UserIdBody 
- 存储从ISBN对应到相应信息的索引 ISBNHead ISBNBody
- 存储从BookName和ISBN到相应信息的索引 BookNameHead BookNameBody
- 存储从Author和ISBN到相应信息的索引 AuthorHead AuthorBody
- 存储从Keyword和ISBN到相应信息的索引 KeywordHead KeywordBody

## 其他补充说明
- 暂时无