file(GLOB_RECURSE SRC_LIST
        ${PROJECT_SOURCE_DIR}/server/*.h
        ${PROJECT_SOURCE_DIR}/server/*.cpp
        ${PROJECT_SOURCE_DIR}/common/*.h
        ${PROJECT_SOURCE_DIR}/common/*.cpp
)

add_executable(server ${SRC_LIST})

target_include_directories(server PRIVATE ../common)
if (WIN32)
    target_link_libraries(server ws2_32)
endif ()