cmake_minimum_required(VERSION 3.23)
project(Home_task1)

set(CMAKE_CXX_STANDARD 20)


add_executable(ht-1 ht-1.c)
add_executable(ht-2 ht-2.c)
add_executable(ht-3 ht-3.c)
add_executable(ht-4 ht-4.c)
target_link_libraries(ht-4 ${CMAKE_DL_LIBS})