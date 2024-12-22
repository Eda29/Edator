cmake_minimum_required(VERSION 3.11)

project(Text_Editor VERSION 1.0)

add_executable(Text_Editor src/main.c src/gap_buffer.c src/config.c inc/gap_buffer.h inc/config.h inc/editor_state.h)
target_include_directories(Text_Editor PUBLIC inc)

if (UNIX)
    set(CURSES_NEEDS_NCURSES TRUE)
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})
    target_link_libraries(Text_Editor ${CURSES_LIBRARY})
endif()


if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()