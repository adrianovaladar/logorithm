# logorithm

[![CMake](https://github.com/adrianovaladar/logorithm/actions/workflows/cmake.yml/badge.svg)](https://github.com/adrianovaladar/logorithm/actions/workflows/cmake.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=adrianovaladar_logorithm&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=adrianovaladar_logorithm)

LOGorithm is a C++ library that provides a simple and efficient logging system. It features a singleton logger with various log levels (Info, Warning, Error), thread-safe logging, and automatic file handling.

## Usage Example

```cpp
#include <logorithm/Logger.h>

using enum LOGLEVEL;

int main() {
    logger.log("This is an info message.", Info);
    logger.log("This is a warning message.", Warning);
    logger.log("This is an error message.", Error);
    return 0;
}
```

## Log Format

Log messages are formatted as follows:

```
[LEVEL] YYYY-MM-DD HH:MM:SS | filename:function:line | message
```

Example:
```
[I] 2024-02-01 12:00:00 | main.cpp:main:10 | This is an informational message.
```

## Creators

This project was created by [**Adriano Valadar**](https://github.com/adrianovaladar).
