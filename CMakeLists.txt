cmake_minimum_required(VERSION 3.10)
project(threader C)
find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

set(SOURCES
    src/main.c
)

set(LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${CURSES_LIBRARIES}
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
