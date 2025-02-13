# logorithm

[![CMake](https://github.com/adrianovaladar/logorithm/actions/workflows/cmake.yml/badge.svg)](https://github.com/adrianovaladar/logorithm/actions/workflows/cmake.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=adrianovaladar_logorithm&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=adrianovaladar_logorithm)

LOGorithm is a C++ library that provides a simple and efficient logging system. It features a singleton logger with various log levels (Trace, Debug, Info, Warning, Error, Fatal), thread-safe logging, and automatic file handling.

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

## Creators

This project was created by [**Adriano Valadar**](https://github.com/adrianovaladar).
