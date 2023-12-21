file(GLOB_RECURSE SRC_LIST
        ${PROJECT_SOURCE_DIR}/client/*.h
        ${PROJECT_SOURCE_DIR}/client/*.cpp
        ${PROJECT_SOURCE_DIR}/common/*.h
        ${PROJECT_SOURCE_DIR}/common/*.cpp
)

add_executable(client ${SRC_LIST})

target_include_directories(client PRIVATE ../common)
if (WIN32)
    target_link_libraries(client ws2_32)
endif ()