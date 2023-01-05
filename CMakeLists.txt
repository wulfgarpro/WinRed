cmake_minimum_required(VERSION 3.21)

project(WinRed)

add_definitions(-DUNICODE -D_UNICODE)

add_subdirectory(ThreadImpersonate)
