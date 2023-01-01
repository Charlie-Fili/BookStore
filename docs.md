# Bookstore开发文档
- 项目名称 ：Bookstore 2022/ 文档作者：杨茗焱
- 
## 程序功能概述 

-   设计并实现一个书店管理系统，具有实现基础指令、账户系统指令、图书系统指令、日志系统指令的功能。
-   具体参考[业务需求文档](requirements.md)

##  主体逻辑说明

- 较大需求为管理账号、图书信息和日志记录，创建三个类AccountSystem、BookSystem和LogSystem辅助处理
- 交互方式利用token scanner逐行读入分析
- 
  ### 指令操作
- 基础指令-直接执行
- 账户系统指令
	- 存储顾客所有信息（UserID、Password、Username、Privilege）
	- 用类和派生类来体现不同权限
	- 存储从UserID对应到相应信息的索引（可以使用比如unordered_map或块状链表)
	- 用栈记录登录账号
	- 记录选择的书
- 图书系统指令
	- 存储图书所有信息（ISBN、BookName、Author、Keyword、stock、Price)
	- 存储从ISBN对应到相应信息的索引以及从BookName、Author、Keyword对应到ISBN的链表（可以使用比如unordered_map或块状链表)
- 日志系统指令
	- 记录交易、收支
	- 日志文件


## 代码文件结构

### main.cpp
- include下面所有类
- 读取指令
- 检测合法性
- 切片并调用相应类与函数
### AccountSystem.h
	(此处及以下省略iostream及fstream等库)
    #include <vector>
    #include <stack>
    #include "LogSystem.h"
    #include "BookSystem.h"
    

- class tourist{
	- 构建函数不设变量
	- void register（string UserID,string Pass,string Username）；//注册
	- void login（string UserID, string Password）；//登录
	- virtual int priority;
 }
- class customer：public tourist{
	-  void login（string UserID, string Password）；
	 - void logout（）；
	 - void passwd（string UserID，String CurrentPassword，String NewPassword）
	 - virtual int priority;
- }
- class employee：public customer{
	-  void login（string UserID, string Password）；//登录 注意判断是否为顾客类型变量
	- void useradd（string UserID,string Pass,string Username）；//注意判断是否为顾客类型变量
	- virtual int priority;
- }
- class owner: public employee{
	-  void login（string UserID）；//登录
	- void passwd（string UserID，String NewPassword）
	- void useradd（string UserID,string Pass,string Username）；
	- void delete（string UserID）；
	- void logPring();
	- virtual int priority;
- }

  -  struct information{
	   string Password
	   string Username
	   string Privilege
   }
- unordered_map<string,information> user;
- struct log_account {
	  string ID
	  int selected_ISDN;
	};
- std::stack<log_account>log_

- // other class or variables

### BookSystem.h
    #include <vector>
    #include "LogSystem.h"
   
- class book{
	- string ISBN
	- string BookName
	- string Author
	- string Keyword
	- int stock
	- double Price
- }
- unordered_map<string,book> book_ISDN;
-  unordered_map<string,book> book_name;
- unordered_map<string,book> book_author;
- unordered_map<string,book> book_keyword;
- class bookmanagement{
	- void show (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]")?
	 - void buy（string ISBN,int Quantity）；
	 - void select (string ISBN,AccountSystem);// 判断权限
	 - void modify (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+;
	 -  import [Quantity] [TotalCost];
	
- } 

### LogSystem.h{
  # include "BookSystem.h"
  # include "AccountSystem.h"
  void record(); // 记录用户干了什么
  void trade(); // 记录每一笔收入
  void show finance ([Count])；// 财务记录查询
  void log(); // 输出日志
}
### error.h 
//负责报错
### chech.h 
//用于检查输入是否合法
### TokenScanner.h 
//切片，参考StanfordCPP


## 文件存储说明
- 专门为日志分出文件存储

## 其他补充说明
- 暂时无