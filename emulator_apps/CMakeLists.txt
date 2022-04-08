set(MCS4_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(mcs4_emulator ${MCS4_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${MCS4_SOURCES})

target_include_directories(${TARGET_ASSEMBLER_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(mcs4_emulator PRIVATE
    ${TARGET_EMULATOR_LIB_NAME}
)

if(MSVC)
    set_target_properties(mcs4_emulator PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT mcs4_emulator)
endif()
