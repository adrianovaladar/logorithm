cmake_minimum_required(VERSION 3.28)

project(LOGorithm VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_library(LOGorithm STATIC
        src/Logger.cpp
        include/logorithm/Logger.h
)

target_include_directories(LOGorithm
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

install(TARGETS LOGorithm DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_executable(example examples/example.cpp)
    target_link_libraries(example PRIVATE LOGorithm)
    add_subdirectory(tests)
endif()
