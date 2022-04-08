cmake_minimum_required(VERSION 3.18)

option(BUILD_TESTS "Builds tests" ON)

function(assure_out_of_source_builds)
    # make sure the user doesn't play dirty with symlinks
    get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
    get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)
    
    if("${srcdir}" STREQUAL "${bindir}")
        message("Warning: in-source builds are disabled")
        message("Please create a separate build directory and run cmake from there")
        message(FATAL_ERROR "Quitting configuration")
    endif()
endfunction()

assure_out_of_source_builds()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/output/Debug/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/output/Debug/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/output/Debug/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/output/Release/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/output/Release/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/output/Release/lib)

project(K4004 LANGUAGES CXX)

add_subdirectory(third_party)

set(SHARED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shared)

set(TARGET_ASSEMBLER_NAME assembler)
add_subdirectory(assembler)

set(TARGET_EMULATOR_LIB_NAME emulator_core)
add_subdirectory(emulator_core)

add_subdirectory(emulator_apps)
