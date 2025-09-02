cmake_minimum_required(VERSION 3.15)
project(SimpleLogger
    VERSION 0.2.0
    DESCRIPTION "A simple logger interface for exposing library internal information"
    HOMEPAGE_URL "https://github.com/r-caso/SimpleLogger"
    LANGUAGES CXX
)

add_library(SimpleLogger INTERFACE)
add_library(SimpleLogger::SimpleLogger ALIAS SimpleLogger)

target_compile_features(SimpleLogger INTERFACE cxx_std_17)

target_include_directories(SimpleLogger
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# ============ Installation ============
include(CMakePackageConfigHelpers)

install(DIRECTORY include/ DESTINATION include)

install(TARGETS SimpleLogger
    EXPORT SimpleLoggerTargets
)

install(EXPORT SimpleLoggerTargets
    FILE SimpleLoggerTargets.cmake
    NAMESPACE SimpleLogger::
    DESTINATION lib/cmake/SimpleLogger
)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/SimpleLoggerConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/SimpleLoggerConfig.cmake
    INSTALL_DESTINATION lib/cmake/SimpleLogger
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/SimpleLoggerConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/SimpleLoggerConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SimpleLoggerConfigVersion.cmake
    DESTINATION lib/cmake/SimpleLogger
)
