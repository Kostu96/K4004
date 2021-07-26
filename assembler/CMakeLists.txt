add_subdirectory(source)

set(TARGET_ASSEMBLER_LIB_NAME ${TARGET_ASSEMBLER_LIB_NAME} PARENT_SCOPE)

if (${BUILD_TESTS})
    add_subdirectory(tests)
endif()

