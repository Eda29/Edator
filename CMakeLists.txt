cmake_minimum_required(VERSION 3.11)

project(Edator VERSION 1.0)

add_executable(Edator src/main.c src/gap_buffer.c src/config.c inc/gap_buffer.h inc/config.h inc/editor_state.h)
target_include_directories(Edator PUBLIC inc)

if (UNIX)
    set(CURSES_NEEDS_NCURSES TRUE)
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})
    target_link_libraries(Edator ${CURSES_LIBRARY})
endif()
