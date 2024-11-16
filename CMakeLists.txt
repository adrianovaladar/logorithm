cmake_minimum_required(VERSION 3.10)

project(LOGorithm VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_library(LOGorithm STATIC
        src/Logger.cpp
        include/logorithm/Logger.h
        examples/example.cpp
)

target_include_directories(LOGorithm
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(example examples/example.cpp)

target_link_libraries(example PRIVATE LOGorithm)

install(TARGETS LOGorithm DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
