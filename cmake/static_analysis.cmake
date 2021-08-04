cmake_minimum_required(VERSION 3.18)

set(CLANG_TIDY_CONFIG
	"--dump-config"
)

find_program(CMAKE_CXX_CLANG_TIDY NAMES clang-tidy)
if (CMAKE_CXX_CLANG_TIDY)
    list(APPEND CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_CONFIG})
endif()
