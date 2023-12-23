cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 17)

add_executable(code
        Bookstore-2023-main/main.cpp
        Bookstore-2023-main/src/account.cpp
        Bookstore-2023-main/src/Book.cpp
        Bookstore-2023-main/Utils/error.cpp
        Bookstore-2023-main/Utils/strlib.cpp
        Bookstore-2023-main/Utils/tokenScanner.cpp
        )
