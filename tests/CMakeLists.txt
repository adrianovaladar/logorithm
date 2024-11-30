add_executable(AllTests
        LoggerTests.h
        LoggerTests.cpp
        ../include/logorithm/Logger.h
        ../src/Logger.cpp
        main.cpp)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
target_link_libraries(AllTests gtest gtest_main gmock_main)
