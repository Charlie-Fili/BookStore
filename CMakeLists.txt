cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code
        main.cpp
        UnrolledLinkedList.h
        UnrolledLinkedList.cpp
        UnrolledLinkedList_double.h
        UnrolledLinkedList_double.cpp
        AccountSystem.h
        BookSystem.h
        LogSystem.h
        TokenScanner.h
        check.h)
