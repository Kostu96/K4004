cmake_minimum_required(VERSION 3.18)

find_program(CMAKE_CXX_CLANG_TIDY NAMES clang-tidy)
if (CMAKE_CXX_CLANG_TIDY)
    set(CLANG_TIDY_CONFIG
        "--checks=bugprone-*,cert-*,cppcoreguidelines-*"
    )
    
    list(APPEND CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_CONFIG})
endif()
