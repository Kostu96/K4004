add_subdirectory(source)

if (${BUILD_TESTS})
    set(TARGET_EMULATOR_TESTS_NAME ${TARGET_EMULATOR_LIB_NAME}_tests)
    add_subdirectory(tests)
endif()
