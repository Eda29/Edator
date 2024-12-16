cmake_minimum_required(VERSION 3.11)

project(Text_Editor VERSION 1.0)

add_executable(Text_Editor src/main.c src/gap_buffer.c)
target_include_directories(Text_Editor PUBLIC inc)