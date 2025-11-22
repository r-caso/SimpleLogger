# SimpleLogger

A minimalist, header-only C++ logging interface designed for libraries that need to expose internal information to applications in a unified way.

Please, take notice that SimpleLogger is not (and will never be) a fully-fledged logging interface.

## Overview

SimpleLogger provides a clean abstract interface for logging with three levels of detail:
- **Info**: General operational messages
- **Debug**: Detailed internal information (includes Info messages when Debug level is set)
- **Trace**: Really verbose internal information (includes both Info and Debug as well)

## Features

- **Header-only**: No compilation required, just include and use
- **Minimal interface**: Only three log levels for simplicity
- **Flexible implementation**: Applications control filtering, formatting, and output
- **Modern C++17**: Uses `std::string_view` for efficient string handling
- **CMake integration**: Easy to consume with `find_package()`

## Requirements

- C++17 or later (for `std::string_view` support)
- CMake 3.15+ (for integration)

## Installation

### Using CMake FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
    SimpleLogger
    GIT_REPOSITORY https://github.com/r-caso/SimpleLogger
    GIT_TAG v0.2.0
)
FetchContent_MakeAvailable(SimpleLogger)

target_link_libraries(your_target PRIVATE SimpleLogger::SimpleLogger)
```

### Manual Installation

```bash
git clone https://github.com/r-caso/SimpleLogger
cd SimpleLogger
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install
cmake --build . --target install
```

Then in your CMakeLists.txt:
```cmake
find_package(SimpleLogger REQUIRED)
target_link_libraries(your_target PRIVATE SimpleLogger::SimpleLogger)
```

## Usage
### In Your Library Code
```cpp
#include <SimpleLogger/simple_logger.hpp>
using namespace iif_sadaf::talk::simple_logger;

void semantic_analysis(SimpleLogger* logger) {
    // call normalize() to handle null pointers
    logger = normalize(logger);
    logger->info("Starting semantic analysis");
    
    // Only do expensive string operations if logger is active
    if (logger->is_active()) {
        std::string expensive_debug_info = compute_detailed_stats();
        logger->debug("Model stats: " + expensive_debug_info);
    }
    
    logger->debug("Loading model weights from cache");
    logger->debug("Initializing tokenizer");
    
    // Process data...
    logger->trace("Processing 1500 tokens");
    logger->trace("Confidence score: 0.87");
    
    logger->info("Analysis complete - 3 entities found");
}
```

### In Your Application
Implement the interface to control logging behavior:
```cpp
#include <SimpleLogger/simple_logger.hpp>
#include <iostream>

class ConsoleLogger : public iif_sadaf::talk::simple_logger::SimpleLogger {
private:
    LogLevel current_level = LogLevel::Info;
public:
    LogLevel get_log_level() const override {
        return current_level;
    }
    
    void set_log_level(LogLevel level) override {
        current_level = level;
    }
    
    void log(LogLevel level, std::string_view message) override {
        // Only log messages at or above current level
        if (level >= current_level) {
            const char* prefix = (level == LogLevel::Debug) ? "[DEBUG] " : "[INFO] ";
            std::cout << prefix << message << std::endl;
        }
    }
};

int main() {
    ConsoleLogger logger;
    
    // Set to Info level - only see general messages
    logger.set_log_level(LogLevel::Info);
    semantic_analysis(&logger);
    
    std::cout << "\n--- Switching to Debug level ---\n\n";
    
    // Set to Debug level - see all messages
    logger.set_log_level(LogLevel::Debug);
    semantic_analysis(&logger);
    
    std::cout << "\n--- No logging (pass nullptr) ---\n\n";
    
    // Pass nullptr - uses built-in null logger (no overhead)
    semantic_analysis(nullptr);
    
    return 0;
}
```

Output with Info level:
```
[INFO] Starting semantic analysis
[INFO] Analysis complete - 3 entities found
```
Output with Debug level:
```
[INFO] Starting semantic analysis
[DEBUG] Loading model weights from cache
[DEBUG] Initializing tokenizer
[INFO] Analysis complete - 3 entities found
```
Output with Trace level:
```
[INFO] Starting semantic analysis
[DEBUG] Loading model weights from cache
[DEBUG] Initializing tokenizer
[TRACE] Processing 1500 tokens
[TRACE] Confidence score: 0.87
[INFO] Analysis complete - 3 entities found
```

### Key Features
- **Null-safe**: Pass `nullptr` to disable logging completely with zero overhead
- **Performance-aware**: Use `is_active()` to avoid expensive string operations when logging is disabled
- **Thread-safe**: Built-in null logger uses safe singleton pattern

## Interface Reference

### LogLevel enum

```cpp
enum class LogLevel {
    Info,   // General operational messages
    Debug,  // Detailed internal information
    Trace   // Verbose internal information
};
```

### SimpleLogger class

```cpp
class SimpleLogger {
public:
    // Get current log level
    virtual LogLevel get_log_level() const = 0;
    
    // Set log level (controls what gets logged)
    virtual void set_log_level(LogLevel level) = 0;
    
    // Core logging method - implement this
    virtual void log(LogLevel level, std::string_view message) = 0;
    
    // Convenience methods
    void info(std::string_view message);   // Calls log(LogLevel::Info, message)
    void debug(std::string_view message);  // Calls log(LogLevel::Debug, message)
    void trace(std::string_view message);  // Calls log(LogLevel::Trace, message)
    
    virtual ~SimpleLogger() = default;
};
```

### NullLogger class
```cpp
class NullLogger : public SimpleLogger {
    virtual LogLevel get_log_level() const override;             // returns LogLevel::Info by default
    virtual void set_log_level(LogLevel level) override;         // does nothing, it's a null logger!
    void log(LogLevel level, std::string_view message) override; // does nothing, it's a null logger!
    bool is_active() const override;                             // returns false, it's a null logger!
};

inline NullLogger* null_logger(); // returns NullLogger instance
```

### Logger normalization function
```cpp
inline SimpleLogger* normalize(SimpleLogger* logger); // returns NullLogger if logger = nullptr,
                                                      // returns logger otherwise
```
## Design Philosophy

SimpleLogger is designed around the principle of **intent separation**:

- **Library authors** express their intent: "this is general information" vs "this is detailed information"
- **Application developers** control the behavior: what gets logged, where it goes, how it's formatted

This separation allows:
- Libraries to be logging-framework agnostic
- Applications to integrate with any logging system
- Complete flexibility in filtering and routing messages
- Easy testing with mock logger implementations

## Implementation Ideas

The interface supports various logging strategies:

```cpp
// Level-based filtering (traditional)
void log(LogLevel level, std::string_view message) override {
    if (level >= current_level) {
        output_stream << message << std::endl;
    }
}

// Separate outputs for different levels
void log(LogLevel level, std::string_view message) override {
    if (level == LogLevel::Trace) {
        debug_file << message << std::endl;
    } else {
        console << message << std::endl;
    }
}

// Integration with existing logging framework
void log(LogLevel level, std::string_view message) override {
    if (level == LogLevel::Trace) {
        spdlog::trace("{}", message);
    }
    else if (level == LogLevel::Debug) {
        spdlog::debug("{}", message);
    }
    else {
        spdlog::info("{}", message);
    }
}
```

## Contributing

Contributions are more than welcome. If you want to contribute, please do the following:

1. Fork the repository.
2. Create a new branch: `git checkout -b feature-name`.
3. Make your changes.
4. Push your branch: `git push origin feature-name`.
5. Create a pull request.

## License

This project is licensed under the [BSD-3-Clause](LICENSE) license.
