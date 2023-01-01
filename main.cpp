#include <iostream>
#include <unordered_map>
#include "TokenScanner.h"

int main() {
    std::string token;
    AccountSystem accountSystem;
    BookSystem bookSystem;
    LogSystem logSystem;
    while (getline(std::cin, token)) {
        try {
            std::string order;
            TokenScanner scanner(token);
            order = scanner.nextToken();
            if (order.empty()) {
            } else if (order == "quit") {
                if (!scanner.hasMore) break;
                else throw std::string("Invalid\n");
            } else if (order == "exit") {
                if (!scanner.hasMore) break;
                else throw std::string("Invalid\n");
            } else if (order == "su") {
                scanner.su(accountSystem, logSystem);
            } else if (order == "logout") {
                if (!scanner.hasMore) scanner.logout(accountSystem, logSystem);
                else throw std::string("Invalid\n");
            } else if (order == "register") {
                scanner.register_(accountSystem, logSystem);
            } else if (order == "passwd") {
                scanner.passwd(accountSystem, logSystem);
            } else if (order == "useradd") {
                scanner.useradd(accountSystem, logSystem);
            } else if (order == "delete") {
                scanner.delete_(accountSystem, logSystem);
            } else if (order == "buy") {
                if (accountSystem.login_stack.empty()) throw std::string("Invalid\n");
                scanner.buy(accountSystem, bookSystem, logSystem);
            } else if (order == "select") {
                if (accountSystem.login_stack.empty()) throw std::string("Invalid\n");
                scanner.select(accountSystem, bookSystem, logSystem);
            } else if (order == "modify") {
                if (accountSystem.login_stack.empty()) throw std::string("Invalid\n");
                scanner.modify(accountSystem, bookSystem, logSystem);
            } else if (order == "import") {
                if (accountSystem.login_stack.empty()) throw std::string("Invalid\n");
                scanner.import(accountSystem, bookSystem, logSystem);
            } else if (order == "log") {
                if (accountSystem.login_stack.empty()) throw std::string("Invalid\n");
                logSystem.showLog();
            } else if (order == "show") {
                int point_ = scanner.point;
                order = scanner.nextToken();
                if (order == "finance") { // show finance
                    if (accountSystem.login_stack.empty()) throw std::string("Invalid\n");
                    scanner.show_finance(accountSystem, logSystem);
                } else { // show in bookSystem
                    scanner.point = point_;
                    scanner.hasMore = true;
                    if (accountSystem.login_stack.empty()) throw std::string("Invalid\n");
                    scanner.show(accountSystem, bookSystem, logSystem);
                }
            } else {
                throw std::string("Invalid\n");
            }
        }
        catch (std::string error_out) {
            std::cout << error_out;
        }
    }
}