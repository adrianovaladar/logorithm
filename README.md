# logorithm

[![CMake](https://github.com/adrianovaladar/logorithm/actions/workflows/cmake.yml/badge.svg)](https://github.com/adrianovaladar/logorithm/actions/workflows/cmake.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=adrianovaladar_logorithm&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=adrianovaladar_logorithm)

LOGorithm is a C++ library that provides a simple and efficient logging system. It features a singleton logger with various log levels (Trace, Debug, Info, Warning, Error, Fatal), thread-safe logging, and automatic file handling.

## Setup

It is possible to include LOGorithm in a project using CMake:

```cmake
include(FetchContent)
FetchContent_Declare(
        LOGorithm
        GIT_REPOSITORY https://github.com/adrianovaladar/LOGorithm.git
        GIT_TAG main
)
FetchContent_MakeAvailable(LOGorithm)
target_link_libraries(Project PRIVATE LOGorithm)
```

## Usage Example

```cpp
#include <logorithm/Logger.h>

using enum LOGLEVEL;

int main() {
    /* Uncomment the next line to filter logs, allowing only Warning, Error and Fatal levels in this case.
     * The other levels will be ignored.
     * All and none will be discarded.
    */
    // logger.setMinimumLogLevel(Warning);
    logger.log("This is a trace message.", Trace);
    logger.log("This is a debug message.", Debug);
    logger.log("This is an info message.", Info);
    logger.log("This is an info message with additional fields.", Info, {{"value1",std::to_string(1)}, {"value2", "example"}});
    logger.log("This is a warning message.", Warning);
    logger.log("This is an error message.", Error);
    logger.log("This is a fatal message.", Fatal);
    return 0;
}
```

## Log Format

Log messages are formatted as follows:

```
[Level] YYYY-MM-DD HH:MM:SS | filename:function:line | message
```

Example:
```
[Info] 2024-02-01 12:00:00 | main.cpp:main:10 | This is an informational message.
```

### Additional fields

It is possible to pass additional fields through a pair of strings. The message will be formatted as follows:

```
[Level] YYYY-MM-DD HH:MM:SS | filename:function:line | message | fields
```

Example:

```
[Info] 2025-02-15 23:59:51 | example.cpp:int main():14 | This is an info message with additional fields. | value1 = 1 | value2 = example
```

## Log Filtering

It is possible to filter logs using `setMinimumLogLevel()`, ensuring only specific log levels are written to the log file. The available filter levels are:

- **None**;
- **Fatal**;
- **Error**;
- **Warning**;
- **Info**;
- **Debug**;
- **Trace**;
- **All**.

### Log Level Hierarchy
Each log level includes all the logs from the levels below it:
- **Debug** logs **Debug**, **Info**, **Warning**, **Error**, and **Fatal** messages;
- **Trace** logs **Trace**, **Debug**, **Info**, **Warning**, **Error**, and **Fatal** messages;
- **All** logs **everything**, including **Trace**, **Debug**, **Info**, **Warning**, **Error**, and **Fatal**.

## Log File Management
- Log files are stored in the logs directory;
- Log files are named log_YYYY-MM-DD_XXX.txt where XXX is a counter (000-999);
- If 1000 log files are reached, a default file log_YYYY-MM-DD.txt is used;
- Each log file has a 10MB size limit before rolling over to a new file (if the program exceeds this limit while running, logging continues in the same file until restart).

## Creators

This project was created by [**Adriano Valadar**](https://github.com/adrianovaladar).
