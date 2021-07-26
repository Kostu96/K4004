set(K4004_APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/app.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/app.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/olc/olcPixelGameEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/olc/olcPixelGameEngine.h
)

add_executable(${TARGET_APP_NAME} ${K4004_APP_SOURCES})
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${K4004_APP_SOURCES})

target_include_directories(${TARGET_ASSEMBLER_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${TARGET_APP_NAME} PRIVATE
    ${TARGET_EMULATOR_LIB_NAME}
)

if(MSVC)
	set_target_properties(${TARGET_APP_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()
