if (${BUILD_TESTS})
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    add_subdirectory(gtest)
    
    set_target_properties(gtest gtest_main PROPERTIES
        FOLDER third_party
        CXX_CLANG_TIDY ""
    )
endif()
