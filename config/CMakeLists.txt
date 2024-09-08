cmake_minimum_required(VERSION 3.22)

project(Phonebook VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
# -Wall -Werror -Wextra -Wpedantic -pedantic-errors -Wformat -Wformat-security -Wuninitialized -Wshadow -Wsign-compare -Wconversion  -Wmissing-field-initializers

set(SOURCES
        ../src/main.c
        ../src/utilities/utilities.c
        ../src/utilities/setup.c
        ../src/utilities/core.c
)

set(HEADERS
        ../includes/utilities.h
        ../includes/global.h
        ../includes/setup.h
        ../includes/emojis.h
)

add_executable(main ${HEADERS} ${SOURCES})

target_link_libraries(main PRIVATE m)